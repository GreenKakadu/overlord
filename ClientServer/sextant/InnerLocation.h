#ifndef INNERLOCATION_H
#define INNERLOCATION_H

#include <QGraphicsItem>
#include "LocationEntity.h"

class InnerLocation : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    InnerLocation(LocationEntity * location, const QPixmap & pixmap, QGraphicsItem * parent = 0 );

signals:
    void sendLocData(LocationEntity *);
    void sendMousePos(int,int);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);



public slots:
private:
LocationEntity * location_;
QString name;
};

#endif // INNERLOCATION_H
