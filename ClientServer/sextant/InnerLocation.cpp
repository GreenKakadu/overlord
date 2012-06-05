#include "InnerLocation.h"
    InnerLocation::InnerLocation(LocationEntity * location,  const QPixmap & pixmap, QGraphicsItem * parent):
          QGraphicsPixmapItem (pixmap, parent)
{
      setZValue(5);// Place it above regular map with z= 0 or 1

      setFlags(ItemIsSelectable /*| ItemIsMovable*/);
      setAcceptsHoverEvents(true);
          location_ = location;
      name = location->getTag().c_str();
      setToolTip(name);
}



    void InnerLocation::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsItem::mousePressEvent(event);
  emit  sendLocData( location_);
  update();
}

void InnerLocation::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  QGraphicsItem::hoverEnterEvent(event);
//  emit  sendMousePos(x, y);
  update();
}

void InnerLocation::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsItem::mouseMoveEvent(event);
}

void InnerLocation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsItem::mouseReleaseEvent(event);
  update();
    }
