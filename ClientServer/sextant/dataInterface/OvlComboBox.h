/* 
 * File:   OvlComboBox.h
 * Author: alex
 *
 * Created on February 14, 2012, 10:30 PM
 */

#ifndef OVLCOMBOBOX_H
#define	OVLCOMBOBOX_H
#include <vector>
#include <QComboBox>
class GameData;

using namespace std;
class OvlComboBox :public QComboBox{
public:
    OvlComboBox(GameData * data =0);
    virtual ~OvlComboBox();
    void addGameItem (GameData * item, bool showTag = false);
    GameData * getCurrentItem();

private:
    vector<GameData *> items_;

    int maxLen_;
};

#endif	/* OVLCOMBOBOX_H */

