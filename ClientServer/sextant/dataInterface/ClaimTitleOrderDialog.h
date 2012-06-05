/* 
 * File:   ClaimTitleOrderDialog.h
 * Author: alex
 *
 * Created on April 23, 2012, 11:10 AM
 */

#ifndef CLAIM_TITLE_ORDERDIALOG_H
#define	CLAIM_TITLE_ORDERDIALOG_H
#include "OrderDialog.h"
#include "OvlComboBox.h"
#include <QCheckBox>
class ClaimTitleOrderDialog : public OrderDialog{
public:
    ClaimTitleOrderDialog(string keyword): OrderDialog(keyword){}
    virtual ~ClaimTitleOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, 
                     ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
private:
OvlComboBox * CB_;
QCheckBox * freeButton_;
};

#endif	/* CLAIM_TITLE_ORDERDIALOG_H */

