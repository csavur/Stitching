#include "graphicscameraitem.h"

#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QApplication>
#include <qmath.h>
#include <math.h>
#include "scene.h"
#include <limits>

#include <QDebug>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

#define CAM_SIZE (150)

GraphicsCameraItem::GraphicsCameraItem(QString name) : m_name(name)
{
    setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);

    setToolTip(m_name);
}

QRectF GraphicsCameraItem::boundingRect() const
{
    return QRectF(0, 0, CAM_SIZE, CAM_SIZE);
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
    qreal bZone = 20.0;
    QRectF rect = boundingRect();
    rect -= QMarginsF(bZone, bZone, bZone, bZone);
    return rect;
}

QVariant GraphicsCameraItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
//        if (change == ItemPositionChange && scene()) {

//            QPointF newPos = value.toPointF();

//            if(QApplication::mouseButtons() == Qt::LeftButton && qobject_cast<Scene*> (scene())){

//                Scene* customScene = qobject_cast<Scene*> (scene());
//                int gridSize = customScene->getGridSize();

//                qreal xV = round(newPos.x()/gridSize)*gridSize;
//                qreal yV = round(newPos.y()/gridSize)*gridSize;

//                return QPointF(xV, yV);
//            }
//            else {
//                return newPos;
//            }
//        }
//        else
//            return QGraphicsItem::itemChange(change, value);


    return QGraphicsItem::itemChange(change, value);
}

void GraphicsCameraItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setOpacity(0.7);
    QGraphicsItem::mousePressEvent(event);
}

void GraphicsCameraItem::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
    const int minDistanceX = 40;
    const int minDistanceY = 60;

    //QPointF p = pos();
    QGraphicsItem::mouseMoveEvent(e);
    QPointF p_new = pos();

    QList<QGraphicsItem*> col_it = collidingItems(Qt::IntersectsItemBoundingRect);

    if(col_it.size() > 0) {

        qreal x_min = col_it[0]->pos().x() - boundingRect().width();
        qreal x_max = col_it[0]->pos().x() + col_it[0]->boundingRect().width();
        qreal y_min = col_it[0]->pos().y() - boundingRect().height();
        qreal y_max = col_it[0]->pos().y() + col_it[0]->boundingRect().height();

        for (int i = 1; i < col_it.size(); ++i) {
            if(col_it[i]->pos().x() - boundingRect().width() < x_min)
                x_min = col_it[i]->pos().x() - boundingRect().width();
            if(col_it[i]->pos().x() + col_it[i]->boundingRect().width() > x_max)
                x_max = col_it[i]->pos().x() + col_it[i]->boundingRect().width();
            if(col_it[i]->pos().y() - boundingRect().height() < y_min)
                y_min = col_it[i]->pos().y() - boundingRect().height();
            if(col_it[i]->pos().y() + col_it[i]->boundingRect().height() > y_max)
                y_max = col_it[i]->pos().y() + col_it[i]->boundingRect().height();
        }

        QRectF rect(QPointF(x_min, y_min), QPointF(x_max, y_max));

        switch(closestSide(p_new, rect)) {
        case LEFT:
            p_new.setX(x_min + minDistanceX);
            break;
        case RIGHT:
            p_new.setX(x_max - minDistanceX);
            break;
        case UPPER:
            p_new.setY(y_min - minDistanceY);
            break;
        case LOWER:
            p_new.setY(y_max + minDistanceY);
            break;
        }
        setPos(p_new);
    }
}

//void GraphicsCameraItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//    QList<QGraphicsItem *> items = scene()->items(QRectF(mapToScene(0, 0), mapToScene(CAM_SIZE, CAM_SIZE) ));
//
//    foreach (QGraphicsItem *item, items) {
//
//        if (item == this)
//            continue;
//
//        QLineF lineToMouse(QPointF(0 + event->pos().x(), 0 + event->pos().y()), mapFromItem(item, 0, 0));
//        qreal angleToMouse = atan2(lineToMouse.dy(), lineToMouse.dx())*180 / Pi;
//
//        qDebug() << "distance to Rect :" << lineToMouse.length();
//        qDebug() << "angle to Rect    :" << angleToMouse;
//        qDebug() << "mouse pos        :" << event->pos();
//
//        if(lineToMouse.length() <= 140) {
//            event->accept();
//            return;
//        }
//    }
//    QGraphicsItem::mouseMoveEvent(event);
//}

void GraphicsCameraItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setOpacity(1);
    QGraphicsItem::mouseReleaseEvent(event);
}

GraphicsCameraItem::BOX_SIDE GraphicsCameraItem::closestSide(const QPointF &p, const QRectF &rect)
{
    qreal x_min = rect.x();
    qreal x_max = rect.x() + rect.width();
    qreal y_min = rect.y();
    qreal y_max = rect.y() + rect.height();
    qreal temp_dist = 0;

    // left
    QLineF l(QPointF(x_min, y_min), QPointF(x_min, y_max));
    qreal min_dist = distance(p,l);
    BOX_SIDE side = LEFT;

    // right
    l.setPoints(QPointF(x_max,y_min), QPointF(x_max, y_max));
    temp_dist = distance(p,l);
    if(temp_dist < min_dist) {
        min_dist = temp_dist;
        side = RIGHT;
    }

    // upper
    l.setPoints(QPointF(x_min, y_min), QPointF(x_max, y_min));
    temp_dist = distance(p,l);
    if(temp_dist < min_dist) {
        min_dist = temp_dist;
        side = UPPER;
    }

    // lower
    l.setPoints(QPointF(x_min, y_max), QPointF(x_max, y_max));
    temp_dist = distance(p,l);
    if(temp_dist < min_dist) {
        min_dist = temp_dist;
        side = LOWER;
    }

    return side;
}

qreal GraphicsCameraItem::distance(const QPointF &p, const QLineF &l)
{
    QPointF p1 = l.p1();
    QPointF p2 = l.p2();
    qreal x = p.x() - p1.x();
    qreal y = p.y() - p1.y();
    qreal x2 = p2.x() - p1.x();
    qreal y2 = p2.y() - p1.y();

    // if line is a point, return distance between point and one line node
    qreal norm = sqrt(x2*x2 + y2*y2);
    if (norm <= std::numeric_limits<int>::epsilon())
      return sqrt(x*x + y*y);

    // distance
    return fabs(x*y2 - y*x2) / norm;
}
