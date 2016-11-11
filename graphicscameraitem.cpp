#include "graphicscameraitem.h"

#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QApplication>
#include <qmath.h>
#include <math.h>
#include "scene.h"

#include <QDebug>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

GraphicsCameraItem::GraphicsCameraItem(QString name) : m_name(name)
{
    setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);

    setToolTip(m_name);
}

QRectF GraphicsCameraItem::boundingRect() const
{
    return QRectF(0, 0, 150, 150);
}

void GraphicsCameraItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    //[1] draw rectangle
    QBrush brush(scene()->collidingItems(this).isEmpty() ? Qt::lightGray : Qt::green);
    QPen pen(Qt::black);
    pen.setWidth(2);

    QRectF tRect = rect();
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(tRect);

    // [2] draw name of the camera
    pen.setColor(Qt::black);
    painter->setPen(pen);

    QFontMetrics fm = painter->fontMetrics();
    int sWidth = fm.width(m_name);

    qreal bZone = 0;
    painter->drawText((option->rect.width()-bZone)/2 - sWidth/2, rect().bottom() - 5, m_name);


    //[3] adding camera icon
    QPointF p = rect().center();
    p -= QPointF(18, 18);

    painter->drawPixmap(p.x(), 0, QPixmap(":images/camera.png").scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    painter->drawPixmap(p, QPixmap(":images/move.png").scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));

}

QRectF GraphicsCameraItem::rect() const
{
    qreal bZone = 10.0;
    QRectF rect = boundingRect();
    rect -= QMarginsF(0, 0, bZone, bZone);
    return rect;
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

    //return QGraphicsItem::itemChange(change, value);
}

void GraphicsCameraItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setOpacity(0.7);
    QGraphicsItem::mousePressEvent(event);
}

void GraphicsCameraItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> items = scene()->items(QRectF(mapToScene(0, 0), mapToScene(150, 150) ));

    foreach (QGraphicsItem *item, items) {

        if (item == this)
            continue;

        QLineF lineToMouse(QPointF(0 + event->pos().x(), 0 + event->pos().y()), mapFromItem(item, 0, 0));
        qreal angleToMouse = atan2(lineToMouse.dy(), lineToMouse.dx())*180 / Pi;

        qDebug() << "distance to Rect :" << lineToMouse.length();
        qDebug() << "angle to Rect    :" << angleToMouse;
        qDebug() << "mouse pos        :" << event->pos();

        if(lineToMouse.length() <= 140) {
            event->accept();
            return;
        }
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void GraphicsCameraItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setOpacity(1);
    QGraphicsItem::mouseReleaseEvent(event);
}
