/***************************************************************************
                          MovementVariety.h  -  description
                             -------------------
    begin                : Wed Jan 15 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef MOVEMENT_VARIETY_H
#define MOVEMENT_VARIETY_H
#include <vector>
#include "Variety.h"
class SkillRule;
class MovementVariety : public Variety  {
public:
      MovementVariety ( const string & keyword, GameData * parent) : Variety(keyword, parent){}
      MovementVariety ( const MovementVariety * prototype ): Variety(prototype){}
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
      ~MovementVariety (){}
    protected:
    private:
};
extern MovementVariety   sampleMode;
extern MovementVariety * walkingMode;
extern MovementVariety * swimingMode;
extern MovementVariety * flyingMode;
extern VarietiesCollection <MovementVariety> movementModes;
extern MovementVariety  sampleMode;

#endif

