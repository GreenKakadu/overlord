#ifndef LOCATIONVIEW_H_
#define LOCATIONVIEW_H_
//#include <QtGui>
#include "LocationEntity.h"
#include "DataView.h"
class QVBoxLayout;
class UnitEntity;
class ListViewer;

        class LocationView : public DataView
{
    Q_OBJECT

public:
        LocationView(LocationEntity * location, QWidget *parent = 0);

private:

    public slots:
    void updateLocation(LocationEntity * location);
    void leftClickOnLocation(LocationEntity * location);

};

#endif /*LOCATIONVIEW_H_*/

