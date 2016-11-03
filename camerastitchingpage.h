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
    explicit CameraStitchingPage(QWidget *parent = 0);
    ~CameraStitchingPage();

private:
    Ui::CameraStitchingPage *ui;


    Scene *m_scene;

    QList<GraphicsCameraItem *> m_rect;
};

#endif // CAMERASTITCHINGPAGE_H
