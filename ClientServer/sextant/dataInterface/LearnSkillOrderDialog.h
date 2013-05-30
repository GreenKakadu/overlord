/* 
 * File:   LearnSkillOrderDialog.h
 * Author: alex
 *
 * Created on May 23, 2013, 12:51 PM
 */

#ifndef LEARN_SKILL_ORDERDIALOG_H
#define	LEARN_SKILL_ORDERDIALOG_H
#include "OrderDialog.h"
#include "OvlComboBox.h"
//#include "OvlElementComboBox.h"

class LearnSkillOrderDialog  : public OrderDialog{
public:
    LearnSkillOrderDialog(string keyword) : OrderDialog(keyword){}
    virtual ~LearnSkillOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
private:
OvlComboBox * skillCB_;
QSpinBox * levelSB_;


};

#endif	/* LEARN_SKILL_ORDERDIALOG_H */

