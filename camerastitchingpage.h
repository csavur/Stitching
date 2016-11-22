#ifndef CAMERASTITCHINGPAGE_H
#define CAMERASTITCHINGPAGE_H

#include <QWidget>
#include "scene.h"
#include "graphicscameraitem.h"

namespace Ui {
class CameraStitchingPage;
}

class CameraStitchingPage : public QWidget
{
    Q_OBJECT

public:
    struct Camera {
        QString name;
        int start;
        int end;
        int stitchdelay;
        GraphicsCameraItem *item;
    };

    explicit CameraStitchingPage(QWidget *parent = 0);
    ~CameraStitchingPage();

    QList<QGraphicsItem *> extractCams(GraphicsCameraItem *parent, QList<QGraphicsItem *> &skipList);

    static void updateBoard(void *obj);

    void show();

    void resizeView();

    void parseXML();

    void drawCamerasIntoView();

signals:
    void signalUpdateBoard();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void slotUpdateBoard();
    void on_pushButtonAddCamera_clicked();

    void on_pushButtonApply_clicked();

    void on_pushButtonDelete_clicked();

    void on_pushButtonLoad_clicked();

private:
    Ui::CameraStitchingPage *ui;

    Scene *m_scene;
    int m_physicalCamCounter;

    QList<GraphicsCameraItem *> m_physicalCams;

    // GroupList
    QList<QList<QGraphicsItem *> > groups;

    QList<QList<Camera> > m_allCams;
};

#endif // CAMERASTITCHINGPAGE_H
