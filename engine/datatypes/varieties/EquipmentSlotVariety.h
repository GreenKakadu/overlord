/***************************************************************************
                          EquipmentSlotVariety.h  -  description
                             -------------------
    begin                : Mon Jan 6 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef EQUIPMENT_SLOT_H
#define EQUIPMENT_SLOT_H
#include "Variety.h"

class EquipmentSlotVariety : public Variety  {
public: 
  EquipmentSlotVariety ( const string & keyword, GameData * parent) : Variety(keyword, parent){}
  EquipmentSlotVariety ( const EquipmentSlotVariety * prototype ) : Variety(prototype){}
//	EquipmentSlotVariety();
	~EquipmentSlotVariety(){}
  virtual STATUS     initialize      ( Parser *parser);
  GameData * createInstanceOfSelf();
		protected:
    private:
};
extern EquipmentSlotVariety      sampleEquipmentSlot;
extern VarietiesCollection <EquipmentSlotVariety>      equipments;

#endif
