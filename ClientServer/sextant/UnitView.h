#ifndef UNITVIEW_H_
#define UNITVIEW_H_
#include <QtGui>
#include "UnitEntity.h"
#include "ConstructionEntity.h"
#include "DataView.h"
class UnitView : public DataView
{
    Q_OBJECT
public:
        UnitView(TokenEntity * token);
	virtual ~UnitView();

private:
 void showUnitView(UnitEntity * unit);
 void showConstructionView(ConstructionEntity *  construction);
 void showOrders(TokenEntity * token);
public slots:

        void updateUnitView(TokenEntity * token);
private slots:

};

#endif /*UNITVIEW_H_*/
