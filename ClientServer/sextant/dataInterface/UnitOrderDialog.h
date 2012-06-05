/* 
 * File:   UnitOrderDialog.h
 * Author: alex
 *
 * Created on March 29, 2012, 2:08 PM
 */

#ifndef UNIT_ORDERDIALOG_H
#define	UNIT_ORDERDIALOG_H
#include "OrderDialog.h"
#include "OvlComboBox.h"
class UnitOrderDialog : public OrderDialog{
public:
    UnitOrderDialog(string keyword) : OrderDialog(keyword){}
    virtual ~UnitOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();    
private:
OvlComboBox * unitCB_;
};

#endif	/* UNIT_ORDERDIALOG_H */


