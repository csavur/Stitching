#ifndef GRAPHICSCAMERAITEM_H
#define GRAPHICSCAMERAITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneDragDropEvent>

class GraphicsCameraItem : public QGraphicsItem
{
public:
    GraphicsCameraItem(QString name);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QColor color() const;
    void setColor(const QColor &color);

    QRectF rect() const;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    QString m_name;
    QColor m_color;

    //QRectF m_rect
};

#endif // GRAPHICSCAMERAITEM_H
