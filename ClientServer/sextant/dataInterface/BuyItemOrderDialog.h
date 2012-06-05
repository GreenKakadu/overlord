/* 
 * File:   BuyItemOrderDialog.h
 * Author: alex
 *
 * Created on March 15, 2012
 */

#ifndef BUY_ITEM_ORDERDIALOG_H
#define	BUY_ITEM_ORDERDIALOG_H
#include "OrderDialog.h"
#include "OvlComboBox.h"

class BuyItemOrderDialog : public OrderDialog{
public:
    BuyItemOrderDialog(string keyword) : OrderDialog(keyword){}
    virtual ~BuyItemOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
private:
QSpinBox * amountSB_;
QSpinBox * priceSB_;
OvlComboBox * itemCB_;

};

#endif	/* BUY_ITEM_ORDERDIALOG_H */

