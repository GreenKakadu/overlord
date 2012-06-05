/* 
 * File:   OrderDialog.h
 * Author: alex
 *
 * Created on January 25, 2012, 7:32 PM
 */

#ifndef ORDERDIALOG_H
#define	ORDERDIALOG_H

#include <string>
#include <vector>
#include "OrderLine.h"
#include "OrderWindow.h"

class ViewManager;
class OrderDialog {
public:
    OrderDialog(string keyword);
    virtual ~OrderDialog();
    static vector<OrderDialog *> orderDialogRegister;
    static OrderDialog * findOrderDialogByKeyword(string keyword);
    static OrderDialog * findOrderDialog(OrderLine *order);
//    static OrderLine * showOrderWindow(TokenEntity * token, OrderLine * order, ViewManager * view);
//    static OrderLine * showOrderWindow(TokenEntity * token, string order, ViewManager * view);
    void registerOrder(string keyword);
 //   virtual OrderWindow * edit(TokenEntity * token, OrderLine * order, ViewManager * view);
    virtual OrderWindow * show(TokenEntity * token, OrderLine * order, ViewManager * view, ExtendedCommand extendedCommandFlag);
     virtual OrderLine * getOrderLine();
    inline string getKeyword(){return keyword_;}
    void rewriteOrder(OrderLine * order);
protected:
    OrderWindow * prepareWindow(TokenEntity * token, OrderLine * order, ViewManager * view, string orderLabel);
    void showWindow(OrderWindow * win);
    OrderLine * updateOrderLine(string s);
    ParameterList params;
    string keyword_;
    TokenEntity * token_;
    OrderLine * order_;
    bool isNewOrder_;
};

#endif	/* ORDERDIALOGWINDOW_H */

