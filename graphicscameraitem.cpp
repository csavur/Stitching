#include "graphicscameraitem.h"

#include <QRectF>
#include <QStyleOptionGraphicsItem>

#include<QDebug>

GraphicsCameraItem::GraphicsCameraItem(QString name) : m_name(name)
{
    setFlag(QGraphicsItem::ItemIsMovable);

    setToolTip(m_name);

    setColor(Qt::blue);
}

QRectF GraphicsCameraItem::boundingRect() const
{
    return QRectF(0, 0, 150, 150);
}

void GraphicsCameraItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush brush(color());
    QPen pen(Qt::black);
    pen.setWidth(2);

    QRectF tRect = rect();


    painter->setPen(pen);
    painter->setBrush(brush);

    painter->drawRect(tRect);

    pen.setColor(Qt::cyan);
    painter->setPen(pen);

    QFontMetrics fm = painter->fontMetrics();
    int sWidth = fm.width(m_name);

    qreal bZone = 10.0;
    painter->drawText((option->rect.width()-bZone)/2 - sWidth/2, 15, m_name);
}

QColor GraphicsCameraItem::color() const
{
    return m_color;
}

void GraphicsCameraItem::setColor(const QColor &color)
{
    m_color = color;
}

QRectF GraphicsCameraItem::rect() const
{
    qreal bZone = 10.0;
    QRectF rect = boundingRect();
    rect -= QMarginsF(0, 0, bZone, bZone);
    return rect;
}

void GraphicsCameraItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> list = collidingItems();

    if(list.size() != 0) {
        foreach(QGraphicsItem * item , list)
        {
            GraphicsCameraItem * camera= static_cast<GraphicsCameraItem *>(item);
            if (camera) {
                camera->setColor(Qt::red);
                camera->update();
            }
        }

        setColor(Qt::red);

    } else {
        setColor(Qt::blue);
    }

    qDebug() << "dragging";

    QGraphicsItem::mouseMoveEvent(event);
}
