/* 
 * File:   EquipItemOrderDialog.h
 * Author: alex
 *
 * Created on March 15, 2012
 */

#ifndef EQUIP_ORDERDIALOG_H
#define	EQUIP_ORDERDIALOG_H
#include "OrderDialog.h"
#include "OvlComboBox.h"

class EquipItemOrderDialog : public OrderDialog{
public:
             EquipItemOrderDialog(string keyword) : OrderDialog(keyword){}
    virtual ~EquipItemOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
private:
QSpinBox * amountSB_;
OvlComboBox * itemCB_;
};

#endif	/* EQUIP_ORDERDIALOG_H */

