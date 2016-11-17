#ifndef GRAPHICSCAMERAITEM_H
#define GRAPHICSCAMERAITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneDragDropEvent>

class GraphicsCameraItem : public QGraphicsItem
{
public:
    enum BOX_SIDE { LEFT, RIGHT, UPPER, LOWER };

    GraphicsCameraItem(QString name);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QRectF rect() const;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    GraphicsCameraItem::BOX_SIDE closestSide(const QPointF &p, const QRectF &rect);
    qreal distance(const QPointF &p, const QLineF &l);

private:
    QString m_name;
};

#endif // GRAPHICSCAMERAITEM_H
