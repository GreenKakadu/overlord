/***************************************************************************
                          BasicApplyStrategy.cpp
                             -------------------
    begin                : Tue Feb 28 2006
    copyright            : (C) 2006 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "BasicApplyStrategy.h"
#include "TokenEntity.h"
#include "UnitEntity.h"

BasicApplyStrategy        sampleApply   ("APPLY",&sampleGameData);
BasicApplyStrategy::BasicApplyStrategy ( const BasicApplyStrategy * prototype ) : Strategy(prototype)
{

}


GameData * BasicApplyStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<BasicApplyStrategy> (this);
}









