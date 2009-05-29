/***************************************************************************
                          InventoryAttribute.h
                             -------------------
    begin                : Sun Apr 03 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef INVENTORY_ATTRIBUTE_H
#define INVENTORY_ATTRIBUTE_H


#include "InventoryElement.h"
class EntityStatistics;
class SkillRule;
class TokenEntity;
class FactionEntity;
/**
  *@author Alex Dribin
  */

class InventoryAttribute {
public: 
	InventoryAttribute(){}
	InventoryAttribute(TokenEntity * entity);
	virtual ~InventoryAttribute();
//	InventoryAttribute(vector < InventoryElement> &inventory);
  STATUS     initialize      ( Parser *);
  void save(ostream &out);
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
		inline bool empty()  {return inventory_.empty();}
		int getWeight();
   virtual int getCapacity(int modeIndex);
	 virtual int getCapacity(MovementVariety * mode);
          void  addStats(EntityStatistics * stats, int figuresNumber );
    int equipedInSlot(EquipmentSlotVariety * slot);
    void updateSlotEquipement(EquipmentSlotVariety * slot,
									vector < InventoryElement > & unequiped, int equipMax);

		protected:
  vector < InventoryElement> inventory_;
	TokenEntity * entity_;
};
typedef   vector <InventoryElement>::iterator  InventoryIterator;
#endif
