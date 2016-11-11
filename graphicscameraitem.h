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

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QString m_name;
};

#endif // GRAPHICSCAMERAITEM_H
