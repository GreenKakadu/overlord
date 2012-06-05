/* 
 * File:   MoveToLocationOrderDialog.h
 * Author: alex
 *
 * Created on March 15, 2012
 */

#ifndef MOVE_TO_LOCATION_ORDERDIALOG_H
#define	MOVE_TO_LOCATION_ORDERDIALOG_H
#include "OrderDialog.h"
#include "OvlComboBox.h"

class MoveToLocationOrderDialog : public OrderDialog{
public:
             MoveToLocationOrderDialog(string keyword) : OrderDialog(keyword){}
    virtual ~MoveToLocationOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
private:
OvlComboBox * destinationCB_;
};

#endif	/* MOVE_TO_LOCATION_ORDERDIALOG_H */

