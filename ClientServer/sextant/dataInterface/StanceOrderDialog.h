/* 
 * File:   StanceOrderDialog.h
 * Author: alex
 *
 * Created on May 21, 2013, 8:53 PM
 */

#ifndef STANCE_ORDERDIALOG_H
#define	STANCE_ORDERDIALOG_H
#include "OrderDialog.h"
#include "OvlComboBox.h"

class StanceOrderDialog : public OrderDialog{
public:
             StanceOrderDialog(string keyword) : OrderDialog(keyword){}
    virtual ~StanceOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
private:
OvlComboBox * stanceTargetCB_;
OvlComboBox * stanceCB_;


};

#endif	/* STANCE_ORDERDIALOG_H */

