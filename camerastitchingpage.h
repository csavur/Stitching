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

signals:
    void signalUpdateBoard();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void slotUpdateBoard();
    void on_pushButtonAddCamera_clicked();

    void on_pushButtonApply_clicked();

private:
    Ui::CameraStitchingPage *ui;

    Scene *m_scene;

    QList<GraphicsCameraItem *> m_cams;

    // GroupList
    QList<QList<QGraphicsItem *> > groups;
};

#endif // CAMERASTITCHINGPAGE_H
