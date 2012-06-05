/* 
 * File:   OvlElementComboBox.h
 * Author: alex
 *
 * Created on May 29, 2012, 11:30 AM
 */

#ifndef OVL_ELEMENT_COMBOBOX_H
#define	OVL_ELEMENT_COMBOBOX_H

#include <vector>
#include <QComboBox>
class AbstractData;

using namespace std;
class OvlElementComboBox :public QComboBox{
public:
    OvlElementComboBox(AbstractData * data =0);
    virtual ~OvlElementComboBox();
    void addGameItem (AbstractData * item, bool showTag = false);
    AbstractData * getCurrentItem();

private:
    vector<AbstractData *> items_;
    int maxLen_;
};

#endif	/* OVL_ELEMENT_COMBOBOX_H */

