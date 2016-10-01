#include "camerastitchingpage.h"
#include "ui_camerastitchingpage.h"

#include <QGraphicsRectItem>

CameraStitchingPage::CameraStitchingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraStitchingPage)
{
    ui->setupUi(this);

    m_scene = new QGraphicsScene();

    ui->graphicsView->setScene(m_scene);

    m_scene->setSceneRect(ui->widget->rect());

    m_cameras << "video0" << "video1" << "video2" << "video3";


    for (int i = 0; i < m_cameras.size(); ++i) {

        GraphicsCameraItem *camera = new GraphicsCameraItem(m_cameras.at(i));
        m_scene->addItem(camera);
        m_rect.append(camera);
    }

    ui->graphicsView->setCacheMode(QGraphicsView::CacheNone);
    ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
}

CameraStitchingPage::~CameraStitchingPage()
{
    delete ui;
}
