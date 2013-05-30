/* 
 * File:   ReleaseTitleOrderDialog.h
 * Author: alex
 *
 * Created on May 1, 2012, 11:48 AM
 */
#ifndef RELEASE_TITLE_ORDERDIALOG_H
#define	RELEASE_TITLE_ORDERDIALOG_H
#include "OrderDialog.h"
#include "OvlElementComboBox.h"
class ReleaseTitleOrderDialog : public OrderDialog{
public:
    ReleaseTitleOrderDialog(string keyword): OrderDialog(keyword){}
    virtual ~ReleaseTitleOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, 
                     ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
private:
OvlElementComboBox * CB_;
};

#endif	/* RELEASE_TITLE_ORDERDIALOG_H */


