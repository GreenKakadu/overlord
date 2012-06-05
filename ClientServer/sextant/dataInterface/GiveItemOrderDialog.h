/* 
 * File:   GiveItemOrderDialog.h
 * Author: alex
 *
 * Created on January 31, 2012, 2:30 PM
 */

#ifndef GIVE_ITEM_ORDERDIALOG_H
#define	GIVE_ITEM_ORDERDIALOG_H
#include "OrderDialog.h"
#include "OvlComboBox.h"

class GiveItemOrderDialog : public OrderDialog{
public:
    GiveItemOrderDialog(string keyword) : OrderDialog(keyword){}
    virtual ~GiveItemOrderDialog(){}
//    virtual OrderWindow * edit(TokenEntity * token, OrderLine * order, ViewManager * view);
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
//   virtual QFrame * showOrderBar(TokenEntity * token, OrderLine * order, ViewManager * view);
    virtual OrderLine * getOrderLine();
private:
QSpinBox * amountSB_;
OvlComboBox * itemCB_;
OvlComboBox * targetCB_;

};

#endif	/* GIVEORDERWINDOW_H */

