#ifndef HEX_H
#define HEX_H

#include <QtGui/QColor>
#include <QtGui/QGraphicsItem>
#include "LocationEntity.h"
class Hex : public QObject, public QGraphicsItem
{
public:
    Hex(const QColor &color, int x, int y, LocationEntity * location);
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item,
               QWidget *widget);
public:
    LocationEntity * getLocation(){return location_;}
public slots:

signals:
    void sendLocData(LocationEntity *);
    void sendMousePos(int,int);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    QPointF selectPosition(int n);
private:
    int x, y;
    QColor color;
//    QList<QPointF> stuff;
    QString name;
    LocationEntity * location_;
};
#endif
