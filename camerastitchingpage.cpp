#include "camerastitchingpage.h"
#include "ui_camerastitchingpage.h"

#include <QGraphicsRectItem>

CameraStitchingPage::CameraStitchingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraStitchingPage)
{
    ui->setupUi(this);

    m_scene = new Scene(this);

    ui->graphicsView->setScene(m_scene);

    m_scene->setSceneRect(ui->widget->rect());

    // Temprorary
    QStringList m_cameras0;
    m_cameras0 << "video0" << "video1";

    for (int i = 0; i < m_cameras0.size(); ++i) {
        GraphicsCameraItem *camera = new GraphicsCameraItem(m_cameras0.at(i));
        m_scene->addItem(camera);

        camera->moveBy(150*i, 0);

        m_rect.append(camera);
    }

    QStringList m_cameras1;
    m_cameras1 << "video2" << "video3";

    for (int i = 0; i < m_cameras1.size(); ++i) {
        GraphicsCameraItem *camera = new GraphicsCameraItem(m_cameras1.at(i));
        m_scene->addItem(camera);

        camera->moveBy(150*i, 100*i + 200);
        m_rect.append(camera);
    }

    ui->graphicsView->setCacheMode(QGraphicsView::CacheNone);
    ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
}

CameraStitchingPage::~CameraStitchingPage()
{
    delete ui;
}
