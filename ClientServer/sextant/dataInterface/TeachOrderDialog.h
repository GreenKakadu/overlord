/* 
 * File:   TeachOrderDialog.h
 * Author: alex
 *
 * Created on May 27, 2013, 8:39 AM
 */

#ifndef TEACH_ORDERDIALOG_H
#define	TEACH_ORDERDIALOG_H
#include "OrderDialog.h"
#include "OvlComboBox.h"

class TeachOrderDialog  : public OrderDialog {
public:
    TeachOrderDialog(string keyword) : OrderDialog(keyword){}
    virtual ~TeachOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
private:
OvlComboBox * skillCB_;
QSpinBox * levelSB_;
};

#endif	/* TEACH_ORDERDIALOG_H */

