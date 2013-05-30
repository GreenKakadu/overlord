#ifndef REVOKE_TITLE_ORDER_DIALOG_H
#define REVOKE_TITLE_ORDER_DIALOG_H
#include "OrderDialog.h"
#include "OvlElementComboBox.h"
class RevokeTitleOrderDialog : public OrderDialog
{
public:
    RevokeTitleOrderDialog(string keyword): OrderDialog(keyword){}
    virtual ~RevokeTitleOrderDialog(){}
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order,
                     ViewManager * view, ExtendedCommand extendedCommandFlag);
    virtual OrderLine * getOrderLine();
private:
OvlElementComboBox * CB_;
};

#endif // REVOKE_TITLE_ORDER_DIALOG_H
