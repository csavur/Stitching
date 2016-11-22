#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QPainter>
#include <QApplication>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    int getGridSize() const;

protected:
    void drawBackground (QPainter* painter, const QRectF &rect);
private:
    int gridSize;
};

#endif // SCENE_H
