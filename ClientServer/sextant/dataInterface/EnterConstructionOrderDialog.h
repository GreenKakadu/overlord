/* 
 * File:   EnterConstructionOrderDialog.h
 * Author: alex
 *
 * Created on May 20, 2013, 10:05 PM
 */

#ifndef ENTER_CONSTRUCTION_ORDER_DIALOG_H
#define	ENTER_CONSTRUCTION_ORDER_DIALOG_H
#include "OrderDialog.h"
#include "OvlComboBox.h"

class EnterConstructionOrderDialog  : public OrderDialog{
public:
            EnterConstructionOrderDialog(string keyword) : OrderDialog(keyword){cout<<"EnterConstructionOrderDialog:"<<keyword_ <<endl;}
    virtual ~EnterConstructionOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
private:
OvlComboBox * constructionCB_;
private:

};

#endif	/* ENTER_CONSTRUCTION_ORDER_DIALOG_H */

