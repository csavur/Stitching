#include "camerastitchingpage.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CameraStitchingPage w;
    w.show();

    return a.exec();
}
