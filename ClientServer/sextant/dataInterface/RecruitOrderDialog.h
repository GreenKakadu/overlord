#ifndef RECRUIT_ORDER_DIALOG_H
#define RECRUIT_ORDER_DIALOG_H
/*
 * File:   RecruitOrderDialog.h
 * Author: Alex
 *
 * Created on April 09, 2013
 */
#include "OrderDialog.h"
#include "OvlComboBox.h"
//     RECRUIT unit-id number race price-per-figure
class RecruitOrderDialog : public OrderDialog{
public:
             RecruitOrderDialog(string keyword);
    virtual ~RecruitOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
             static int newUnitPlaceholderCounter;
private:
//OvlComboBox * newUnitCB_;
QSpinBox * newUnitCB_;
string newUnitPlaceholder_;
OvlComboBox * raceCB_;
QSpinBox * amountSB_;
QSpinBox * priceSB_;
OvlComboBox * newUnit_;
int newUnitPlaceholderCounter_;
};

#endif // RECRUIT_ORDER_DIALOG_H
