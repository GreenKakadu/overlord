/***************************************************************************
                          NewEntityPlaceholder.cpp 
                             -------------------
    begin                : Tue Jun 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "NewEntityPlaceholder.h"
#include "Entity.h"
#include "GameConfig.h"
#include "ConstructionEntity.h"
#include "UnitEntity.h"

NewEntityPlaceholder::NewEntityPlaceholder(const string& name)
{
//   cout << "newPlaceholder with tag " << name<< " created"<<endl;
  temporaryName_ = name;
  entity_ = 0;
	temporaryEntity_ = 0;
}

void NewEntityPlaceholder::saveAsParameter (ostream &out)
{
 if(entity_)
  entity_->saveAsParameter(out);
  else
    out << " " << temporaryName_;
}


// Determine type of Entity, based on temporaryName_ and create appropriate entity
// Default is TokenEntity
TokenEntity * NewEntityPlaceholder::getNewEntity()
{
 if(temporaryEntity_)
		return temporaryEntity_;


 char prefix = gameFacade->getGameConfig()->getEntityTypePrefix(temporaryName_);

 	if(prefix == gameFacade->units.getEntityTagPrefix())
	{
		temporaryEntity_ = new UnitEntity(sampleUnit);
	}
 	else if(prefix ==  gameFacade->buildingsAndShips.getEntityTagPrefix())
	{
		temporaryEntity_ = new ConstructionEntity(sampleConstructionEntity);
	}
  else
	{
		temporaryEntity_ = new TokenEntity(sampleTokenEntity);
	}

  temporaryEntity_->explicitInitialization();

		return temporaryEntity_;


}

