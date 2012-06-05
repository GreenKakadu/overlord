/* 
 * File:   OathOrderDialog.h
 * Author: alex
 *
 * Created on March 15, 2012
 */

#ifndef OATH_ORDERDIALOG_H
#define	OATH_ORDERDIALOG_H
#include "OrderDialog.h"
#include "OvlComboBox.h"

class OathOrderDialog : public OrderDialog{
public:
             OathOrderDialog(string keyword) : OrderDialog(keyword){}
    virtual ~OathOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
private:
OvlComboBox * oathTargetCB_;
};

#endif	/* OATH_ORDERDIALOG_H */

