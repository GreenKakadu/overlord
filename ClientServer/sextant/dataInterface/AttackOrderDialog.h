#ifndef ATTACK_ORDER_DIALOG_H
#define ATTACK_ORDER_DIALOG_H
/*
 * File:   AttackOrderDialog.h
 * Author: alex
 *
 * Created on March 19, 2013
 */
#include "OrderDialog.h"
#include "OvlComboBox.h"
class StanceVariety;
class AttackOrderDialog : public OrderDialog{
public:
             AttackOrderDialog(string keyword) : OrderDialog(keyword){}
    virtual ~AttackOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
       void addTargetsByStance(TokenEntity * token, StanceVariety * stance);
private:
OvlComboBox * targetCB_;
};

#endif // ATTACK_ORDER_DIALOG_H
