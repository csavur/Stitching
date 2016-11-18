#include "camerastitchingpage.h"
#include "ui_camerastitchingpage.h"

#include <QGraphicsRectItem>
#include <QDebug>

CameraStitchingPage::CameraStitchingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraStitchingPage)
{
    ui->setupUi(this);

    m_scene = new Scene(this);

    ui->graphicsView->setScene(m_scene);

    m_scene->setSceneRect(QRectF(0, 0, 600, 500));

    ui->graphicsView->setCacheMode(QGraphicsView::CacheNone);
    ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    //QGraphicsTextItem *textItem = m_scene->addText("Group Board");

    qDebug() << m_scene->sceneRect();
    qDebug() << ui->graphicsView->sceneRect();

    //textItem->setPos(m_scene->sceneRect().topRight());
}

CameraStitchingPage::~CameraStitchingPage()
{
    delete ui;
}

void CameraStitchingPage::on_pushButtonAddCamera_clicked()
{
    static int counter = 0;
    GraphicsCameraItem *camera = new GraphicsCameraItem(QString("video") + QString::number(counter));
    m_scene->addItem(camera);

    m_cams.append(camera);

    counter++;
}

void CameraStitchingPage::on_pushButtonGroup_clicked()
{
    QList<QGraphicsItem *> skipList;

    for (int i = 0; i < m_cams.size(); ++i) {

        if(skipList.contains(m_cams[i]))
            continue;

        GraphicsCameraItem *item = static_cast<GraphicsCameraItem *>(m_cams[i]);
        QList<QGraphicsItem *> colList = item->stitched();
        qDebug() << item->name() << " is group with ";
        for (int j = 0; j < colList.size(); ++j) {
            GraphicsCameraItem *item0 = static_cast<GraphicsCameraItem *>(colList[j]);
            qDebug() << item0->name() << ", ";
            skipList.append(colList[j]);
        }

    }
}
