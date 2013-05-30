/* 
 * File:   TargetOrderDialog.h
 * Author: alex
 *
 * Created on May 22, 2013, 10:20 AM
 */

#ifndef TARGET_ORDERDIALOG_H
#define	TARGET_ORDERDIALOG_H
#include "OrderDialog.h"
#include "OvlComboBox.h"


class TargetOrderDialog: public OrderDialog {
public:
             TargetOrderDialog(string keyword) : OrderDialog(keyword){}
    virtual ~TargetOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
private:
OvlComboBox * targetCB_;

};

#endif	/* TARGET_ORDERDIALOG_H */

