#ifndef FACTIONVIEW_H_
#define FACTIONVIEW_H_
#include <QtGui>
#include "DataView.h"

class FactionEntity;


class FactionView : public DataView
{
    Q_OBJECT
public:
    FactionView(FactionEntity * faction, QWidget *parent = 0);
    virtual ~FactionView();
    void customisePalette();
public slots:
    void updateFactionData(FactionEntity * faction);
private:
    //QListWidget *listWidget;
    //QListWidget *listWidget2;

    //QTreeWidget *lTree;
    //FactionEntity * faction_;
private slots:
//    void listWidget_itemDouble_Clicked(QListWidgetItem * item);
 //   void on_listWidget2_itemDouble_Clicked(QListWidgetItem * item);

};

#endif /*FACTIONVIEW_H_*/
