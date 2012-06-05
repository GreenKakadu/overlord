/* 
 * File:   UseSkillOrderDialog.h
 * Author: alex
 *
 * Created on February 27, 2012, 2:12 PM
 */

#ifndef USE_SKILL_ORDERDIALOG_H
#define	USE_SKILL_ORDERDIALOG_H
#include "OrderDialog.h"
#include "OvlComboBox.h"

class UseSkillOrderDialog : public OrderDialog{
public:
    UseSkillOrderDialog(string keyword) : OrderDialog(keyword){}
    virtual ~UseSkillOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
private:
QSpinBox * amountSB_;
OvlComboBox * skillCB_;
OvlComboBox * targetCB_;
};

#endif	/* USEORDERWINDOW_H */

