/* 
 * File:   BestowTitleOrderDialog.h
 * Author: alex
 *
 * Created on May 1, 2012, 12:40 PM
 */

#ifndef BESTOW_TITLE_ORDERDIALOG_H
#define	BESTOW_TITLE_ORDERDIALOG_H
#include "OrderDialog.h"
#include "OvlComboBox.h"
#include "OvlElementComboBox.h"
class BestowTitleOrderDialog : public OrderDialog{
public:
    BestowTitleOrderDialog(string keyword): OrderDialog(keyword){}
    virtual ~BestowTitleOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, 
                     ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
private:
OvlElementComboBox * titleCB_;
OvlComboBox * unitCB_;
};

#endif	/* BESTOW_TITLE_ORDERDIALOG_H */
