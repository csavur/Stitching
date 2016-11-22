#include "camerastitchingpage.h"
#include "ui_camerastitchingpage.h"

#include <QResizeEvent>
#include <QGraphicsRectItem>
#include <QDomDocument>
#include <QFile>
#include <QDebug>

#include <iostream>
#include <vector>
#include <string>

using namespace std;

//!
//! \brief generateSoftTriggerConf
//! It generate conf.softTrig file base on given vector
//! vector consist of pair <string, string>
//! first string indicates tag name, second string value
//!
//! There are two special characters ("<" and ">") preallocated
//! for value (second string)
//! For example:
//! <"name", "john" >  ===> <name>john</name>
//! <"name", "<" >     ===> <name>
//! <"name", ">" >     ===> </name>
//!
//! \param std::vector< std::pair<string, string> > list
//! \return -1/1
//!
int generateSoftTriggerConf(std::vector< std::pair<string, string> > list)
{
    // Delete previous configuration
    if(remove("conf.softTrig" ) != 0 ) {
       perror("Error when deleting conf.softTrig!" );
    } else {
       puts("conf.softTrig successfully deleted..." );
    }

    FILE *file = fopen("conf.softTrig", "w");

    if (file == NULL) {
        printf("Error opening file!\n");
        return -1;
    }

    for (unsigned i = 0; i < list.size(); ++i) {
        pair<string, string> p = list.at(i);
        if(p.second == "<") {
            fprintf(file, "<%s>\n", p.first.c_str());
        } else if(p.second == ">") {
            fprintf(file, "</%s>\n", p.first.c_str());
        } else {
            fprintf(file, "<%s>%s</%s>\n", p.first.c_str(), p.second.c_str(), p.first.c_str());
        }
    }

    fclose(file);

    printf("conf.softTrig successfully generated!\n");

    return 1;
}

CameraStitchingPage::CameraStitchingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraStitchingPage)
{
    ui->setupUi(this);

    m_scene = new Scene(this);

    ui->graphicsView->setScene(m_scene);
    m_physicalCamCounter = 1;

    m_scene->setSceneRect(0, 0, 600, 500);

    ui->graphicsView->setCacheMode(QGraphicsView::CacheNone);
    ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // Register to update board automaticly
    GraphicsCameraItem::registerMethod(CameraStitchingPage::updateBoard, this);

    connect(this, SIGNAL(signalUpdateBoard()), this, SLOT(slotUpdateBoard()));
}

CameraStitchingPage::~CameraStitchingPage()
{
    delete ui;
}

QList<QGraphicsItem *> CameraStitchingPage::extractCams(GraphicsCameraItem *parent, QList<QGraphicsItem *> &skipList)
{
    // This is a recursive method that extract all the camera that connected to each other.
    QList<QGraphicsItem *> list = parent->stitched();
    QList<QGraphicsItem *> childs;

    if(list.size() == 0) {
        //qDebug() << parent->name();
        childs << parent;
        return childs;
    }

    for (int i = 0; i < list.size(); ++i) {
        GraphicsCameraItem *item = dynamic_cast<GraphicsCameraItem *>(list[i]);
        if(item) {
            if(!skipList.contains(item)) {
                //qDebug() << item->name();
                childs << item;
                skipList.append(item);
                childs << extractCams(item, skipList);
            }
        }
    }
    return childs;
}

void CameraStitchingPage::updateBoard(void *obj)
{
    ((CameraStitchingPage *)obj)->signalUpdateBoard();
}

void CameraStitchingPage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeView();
}

void CameraStitchingPage::show()
{
    QWidget::show();

    resizeView();
}

void CameraStitchingPage::resizeView()
{
    QRect sceneSize = ui->widgetView->rect() - QMargins(0, 0, 15, 15);
    m_scene->setSceneRect(sceneSize);
}

void CameraStitchingPage::parseXML()
{
        m_allCams.clear();
        QString filename("conf.softTrig");
        QDomDocument doc;
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
            return;

        QDomNodeList virtualCams = doc.elementsByTagName("virtualcam");
        for (int i = 0; i < virtualCams.size(); i++) {
            QDomNodeList cams = virtualCams.item(i).childNodes();
            qDebug() << virtualCams.item(i).nodeName();
            QList<Camera> camList;
            for (int j = 0; j < cams.size(); ++j) {
                qDebug() << cams.item(i).nodeName();
                QDomNode n = cams.item(j);
                QDomElement start = n.firstChildElement("start");
                QDomElement end = n.firstChildElement("end");
                QDomElement stitchdelay = n.firstChildElement("stitchdelay");
                if (start.isNull() || end.isNull() || stitchdelay.isNull())
                    continue;
                qDebug() << start.text() << " " << end.text() << " " << stitchdelay.text();

                Camera c;

                c.name = QString("Camera") + QString::number(m_physicalCamCounter);
                c.start = start.text().toInt();
                c.end = end.text().toInt();
                c.stitchdelay = stitchdelay.text().toInt();

                camList.append(c);
                m_physicalCamCounter++;
            }
            m_allCams.append(camList);
        }


        drawCamerasIntoView();
}

void CameraStitchingPage::drawCamerasIntoView()
{
    for (int i = 0; i < m_allCams.size(); ++i) { // # virtial camera
        // find row
        QPointF center = m_scene->sceneRect().center();
        QPointF targetPos(QPointF(center.x() - CAM_SIZE*m_allCams[i].size()/2, 0) + QPointF(0, i*CAM_SIZE*1.2 + CAM_SIZE/2));
        for (int j = 0; j < m_allCams[i].size(); ++j) { // # physical cameras in given virtual camers
            // add physical Cameras

            GraphicsCameraItem *camera = new GraphicsCameraItem(m_allCams[i][j].name);

            m_scene->addItem(camera);
            m_physicalCams.append(camera);

            camera->setPos(targetPos + QPointF( j*(CAM_SIZE - 2*CAM_SENSITIVITY), 0));
        }
    }

    GraphicsCameraItem::updateVideos();
}

void CameraStitchingPage::on_pushButtonAddCamera_clicked()
{
    GraphicsCameraItem *camera = new GraphicsCameraItem(QString("Camera") + QString::number(m_physicalCamCounter));

    m_scene->addItem(camera);
    m_physicalCams.append(camera);

    m_physicalCamCounter++;

    GraphicsCameraItem::updateVideos();
}

bool sortByXAxis(const QGraphicsItem *v1, const QGraphicsItem *v2)
{
    return v1->pos().x() < v2->pos().x();
}

bool sortByYAxis(const QGraphicsItem *v1, const QGraphicsItem *v2)
{
    return v1->pos().y() < v2->pos().y();
}

void CameraStitchingPage::slotUpdateBoard()
{
    groups.clear();
    // Sort by X
    qSort(m_physicalCams.begin(), m_physicalCams.end(), sortByXAxis);
    // Sort by Y
    qSort(m_physicalCams.begin(), m_physicalCams.end(), sortByYAxis);

    // Determine which camera belongs to which video
    QList<QGraphicsItem *> skipList;
    for (int i = 0; i < m_physicalCams.size(); ++i) {
        GraphicsCameraItem *item = static_cast<GraphicsCameraItem *>(m_physicalCams[i]);
        if(!skipList.contains(item)) {

            QList<QGraphicsItem *> list = extractCams(item, skipList);
            groups.append(list);
        }
    }

    QString result;
    // Go trough all video and sort then by only X axis
    for (int i = 0; i < groups.size(); ++i) {
        QList<QGraphicsItem *> group = groups[i];
        qSort(group.begin(), group.end(), sortByXAxis);

        qDebug() << "Video" << i+1;
        result.append(QString("Video%1 \n").arg(i+1));
        result.append("|");
        for (int j = 0; j < group.size(); ++j) {
            GraphicsCameraItem *item = static_cast<GraphicsCameraItem *>(group[j]);
            qDebug() << item->name();
            result.append("-- ");
            result.append(item->name()); result.append("\n");

            if(j!=group.size()-1) {
                result.append("|");
            }
        }
    }

    ui->labelResult->setText(result);
}

void CameraStitchingPage::on_pushButtonApply_clicked()
{
    std::vector< std::pair<string, string> > cam;

    cam.push_back( make_pair("softTrig", "<") );
    cam.push_back( make_pair("linehistory", "60000") );

    // Go trough all video and sort then by only X axis
    for (int i = 0; i < groups.size(); ++i) {
        QList<QGraphicsItem *> group = groups[i];
        cam.push_back( make_pair("virtualcam", "<") );
        for (int j = 0; j < group.size(); ++j) {
            //GraphicsCameraItem *item = static_cast<GraphicsCameraItem *>(group[j]);
            cam.push_back( make_pair("camera", "<") );
            cam.push_back( make_pair("start", "0") );
            cam.push_back( make_pair("end", "4079") );
            cam.push_back( make_pair("stitchdelay", "0") );
            cam.push_back( make_pair("camera", ">") );
        }
        cam.push_back( make_pair("virtualcam", ">") );
    }

    cam.push_back( make_pair("softTrig", ">") );

    generateSoftTriggerConf(cam);
}

void CameraStitchingPage::on_pushButtonDelete_clicked()
{
    QList<QGraphicsItem *> selectedItems = m_scene->selectedItems();

    for (int i = 0; i < selectedItems.size(); ++i) {
        GraphicsCameraItem *item = dynamic_cast<GraphicsCameraItem *>(selectedItems[i]);
        if(item) {
            qDebug() << item->name();
            m_physicalCams.removeOne(item);
            m_scene->removeItem(item);
        }
    }

    GraphicsCameraItem::updateVideos();
}

void CameraStitchingPage::on_pushButtonLoad_clicked()
{
    parseXML();
}
