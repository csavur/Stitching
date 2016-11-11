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

    ui->graphicsView->setCacheMode(QGraphicsView::CacheNone);
    ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

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

    counter++;
}
