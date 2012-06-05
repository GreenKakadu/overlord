#include "hex.h"
#include <QPainter>
#include <QPolygon>

#include <QtGui>

 static const QPointF points[6] = 
{
     QPointF(0, 87),
     QPointF(50, 0),
     QPointF(150, 0),
     QPointF(200, 87),
     QPointF(150, 174),
     QPointF(50, 174),
 };

Hex::Hex(const QColor &color, int x, int y, LocationEntity * location)
{
    this->x = x;
    this->y = y;
    this->color = color;
    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable /*| ItemIsMovable*/);
    setAcceptsHoverEvents(true);
	location_ = location;
    name = location->getTag().c_str();
    setToolTip(name);
}

QRectF Hex::boundingRect() const
{
    return QRectF(0, 0, 200, 174);
}

QPainterPath Hex::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void Hex::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor
         = (option->state & QStyle::State_Selected) ? color.dark(150) : color;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(125);

    if (option->levelOfDetail < 0.2) {


        painter->setPen(QPen(Qt::black, 0));
        painter->setBrush(fillColor);
        painter->drawConvexPolygon(points, 6);//4
        return;
    }

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
        width += 2;

    pen.setWidth(width);
    painter->setBrush(QBrush(
            fillColor.dark(option->state & QStyle::State_Sunken ? 120 : 100)));

    painter->drawConvexPolygon(points, 6);//5
    painter->drawText(90, 92, name);
}
//QPointF Hex::selectPosition(int n)
//{
//  switch(n)
//  {
//
//  }
//  switch (n) {
//    case 1:
//      return QPointF(125,130);
//      break;
//    case 2:
//      return QPointF(75,130);
//
//       break;
//    case 3:
//      return QPointF(125,43);
//
//        break;
//    case 4:
//      return QPointF(75,43);
//
//       break;
//    default:
//      break;
//  }
//  return QPointF(87,100);
//}
void Hex::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    emit sendLocData(location_);
    update();
}

void Hex::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
    emit sendMousePos(x,y);
    update();
}

void Hex::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    if (event->modifiers() & Qt::ShiftModifier) {
//        stuff << event->pos();
//        update();
//        return;
//    }
    QGraphicsItem::mouseMoveEvent(event);
}

void Hex::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

