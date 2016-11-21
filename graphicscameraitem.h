#ifndef GRAPHICSCAMERAITEM_H
#define GRAPHICSCAMERAITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneDragDropEvent>

typedef void (*FunctionPtr) (void*);

class GraphicsCameraItem : public QGraphicsItem
{
public:
    enum BOX_SIDE { LEFT, RIGHT, UPPER, LOWER };

    GraphicsCameraItem(QString name);

    QString name() const;
    QRectF rect() const;

    GraphicsCameraItem::BOX_SIDE closestSide(const QPointF &p, const QRectF &rect);
    qreal distance(const QPointF &p, const QLineF &l);

    QList<QGraphicsItem *> stitched() const;

    void autoDetectStitching();
    static void updateVideos();

    static void registerMethod(FunctionPtr ptr, void *p);

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QString m_name;

    QList<QGraphicsItem *> m_stitched;

    // This allow me  to find reach all cams
    static QList<GraphicsCameraItem *> m_allVideos;

    static FunctionPtr m_callbackFuncPtr;
    static void * m_callbackObjPtr;
};

#endif // GRAPHICSCAMERAITEM_H
