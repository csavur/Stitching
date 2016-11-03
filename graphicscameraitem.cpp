#include "graphicscameraitem.h"

#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QApplication>
#include <qmath.h>
#include "scene.h"

#include <QDebug>

GraphicsCameraItem::GraphicsCameraItem(QString name) : m_name(name)
{
    setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);

    setToolTip(m_name);

    setColor(Qt::lightGray);
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

    pen.setColor(Qt::black);
    painter->setPen(pen);

    QFontMetrics fm = painter->fontMetrics();
    int sWidth = fm.width(m_name);

    qreal bZone = 10.0;
    painter->drawText((option->rect.width()-bZone)/2 - sWidth/2, rect().bottom() - 5, m_name);

    QPointF p = rect().center();
    p -= QPointF(18, 18);

    painter->drawPixmap(p.x(), 0, QPixmap(":images/camera.png").scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    painter->drawPixmap(p, QPixmap(":images/move.png").scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));

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
//    QList<QGraphicsItem *> list = collidingItems();

//    for (int i = 0; i < list.size(); ++i) {
//        QRectF rect = list[i]->boundingRect();
//        this->setPos(rect.topRight());
//        this->update();
//        return;
//    }

    QGraphicsItem::mouseMoveEvent(event);
}

QVariant GraphicsCameraItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
           QPointF newPos = value.toPointF();
           if(QApplication::mouseButtons() == Qt::LeftButton && qobject_cast<Scene*> (scene())){
               Scene* customScene = qobject_cast<Scene*> (scene());
               int gridSize = customScene->getGridSize();
               qreal xV = round(newPos.x()/gridSize)*gridSize;
               qreal yV = round(newPos.y()/gridSize)*gridSize;
               return QPointF(xV, yV);
           }
           else
               return newPos;
       }
       else
           return QGraphicsItem::itemChange(change, value);
}
