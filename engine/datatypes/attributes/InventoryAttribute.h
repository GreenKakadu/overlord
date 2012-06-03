/***************************************************************************
                          InventoryAttribute.h
                             -------------------
    begin                : Sun Apr 03 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the BSD License.                          *
 *                                                                         *
 ***************************************************************************/

#ifndef INVENTORY_ATTRIBUTE_H
#define INVENTORY_ATTRIBUTE_H

#include <map>
#include "InventoryElement.h"
class EntityStatistics;
class SkillRule;
class TokenEntity;
class FactionEntity;
class BasicCondition;
/**
  *@author Alex Dribin
  */

class InventoryAttribute {
public: 
    InventoryAttribute(){}
    InventoryAttribute(TokenEntity * entity);
    InventoryAttribute(InventoryAttribute & p);
    virtual ~InventoryAttribute();
    //	InventoryAttribute(vector < InventoryElement> &inventory);
    STATUS     initialize      ( Parser *);
    void save(ostream &out);
    void save(ostream &out, string prefix);
    ostream& report(ostream &out);

    InventoryElement * findItem(ItemRule * item);
    vector < InventoryElement > getSlotContent(EquipmentSlotVariety * slot);
    void deleteElement(InventoryElement * element);
    void    add(ItemRule * item, RationalNumber& num);
    void    add(ItemRule * item, int num);
    void    add(InventoryElement item);
    void   init(TokenEntity * entity);
    int     take(ItemRule * item, int num);
    int     hasItem(ItemRule * item) ;
    bool isEquiped(InventoryElement * item);
    RationalNumber     getItemAmount(ItemRule * item) ;
    virtual int     equipItem(ItemRule * item, int num);
    int     hasEquiped(ItemRule * item) ;
    vector < InventoryElement > & getAll();
    void giveAll(TokenEntity * unit);
    virtual int getProductionBonus(SkillRule * skill);
    virtual int getMovementBonus(MovementVariety * mode);
    virtual int getLearningBonus(SkillRule * skill);
    virtual  int getStudyBonus(SkillRule * skill);
    //  ==============================================
    void reportInventory(FactionEntity * faction, ReportPrinter &out);
    void reportPublicInventory(ReportPrinter &out, bool isMultiple);
     virtual void extractAndAddKnowledge(Entity * recipient, int parameter = 0);
    vector <AbstractArray>   aPrint();
    inline bool empty()  {return inventory_.empty();}
    int getWeight();
    virtual int getCapacity(int modeIndex);
    virtual int getCapacity(MovementVariety * mode);
    void  addStats(EntityStatistics * stats, int figuresNumber );
    int equipedInSlot(EquipmentSlotVariety * slot);
    void updateSlotEquipement(EquipmentSlotVariety * slot,
                              vector < InventoryElement > & unequiped, int equipMax);
    InventoryAttribute getObservableImage();
    void combineInventories(InventoryAttribute * inventory);
    void addEquipCondition(ItemRule * item);
    void removeEquipCondition(ItemRule * item);
    void checkEquipmentConditions(TokenEntity * entity);
    BasicCondition *  registerEquipCondition(ItemRule * item, int currentlyEquipedItems, int num);
		protected:
    vector < InventoryElement> inventory_;
    TokenEntity * entity_;
    map <ItemRule *, BasicCondition *> equipConditions_;
};
typedef   vector <InventoryElement>::iterator  InventoryIterator;
#endif
