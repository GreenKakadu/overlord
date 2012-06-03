/***************************************************************************
                          FactionEntity.cpp  -  description
                             -------------------
    begin                : Tue Nov 19 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <stdio.h>
#include <algorithm>
#include <cctype>

#include "GameConfig.h"
#include "Symbols.h"

#include "FileParser.h"
#include "ReportPrinter.h"
#include "SimpleMessage.h"

#include "FactionEntity.h"
#include "UnitEntity.h"
#include "TokenEntity.h"
#include "LocationEntity.h"
#include "ConstructionEntity.h"
#include "TokenEntity.h"
#include "EffectEntity.h"

#include "EntitiesCollection.h"
#include "RulesCollection.h"
#include "StanceVariety.h"
#include "ItemRule.h"
#include "RaceRule.h"
#include "RuleIndex.h"
#include "SkillRule.h"
#include "RulesCollection.h"

#include "CombatReport.h"
#include "ReportElement.h"
#include "ReportMessage.h"
#include "DataManipulator.h"
#include "TurnReport.h"

extern ReportPattern *	 resignReporter;
//extern DataManipulator * dataManipulatorPtr;
extern bool ciStringCompare(const string& s1,const string& s2);
//extern StanceVariety * findStanceByTag(const string &tag);
FactionEntity  sampleFaction ("FACTION", &sampleEntity);
//EntitiesCollection <FactionEntity>   factions(new DataStorageHandler(gameConfig.getFactionsFile() ),&sampleFaction);

extern int Roll_1Dx(int n);

FactionEntity::FactionEntity ( const FactionEntity * prototype ) : Entity(prototype)
{
  defaultStance_ =0;
  maxControlPoints_ = 200;
  controlPoints_ = 0;
  terseBattleReport_ = false;
  allSkillsToReshow_ = false;
  isResigned_ = false;
  isDisbanded_ = false;
  temporaryFlags_ = 0x0;
  x0_ = 0;
  y0_ = 0;
}

void    FactionEntity::preprocessData()
{
  if(defaultStance_==0)
  {
	defaultStance_  = neutralStance;
  }
  newKnowledge = knowledge_.size();
  newSkillKnowledge = skillKnowledge_.size();
  maxControlPoints_ = 200;
  controlPoints_ = 0;
  terseBattleReport_ = false;
  isResigned_= false;;
  isDisbanded_ = false;
  turnReport_ = new TurnReport();
  turnReport_->init(this);
}



GameData * FactionEntity::createInstanceOfSelf()
{
   return CREATE_INSTANCE<FactionEntity> (this);
}



// Create faction object as it is seen by referent
FactionEntity *     FactionEntity::createFactionImage(FactionEntity * referent)
{
    if(referent == this)
    {
        // Should not create image of your own faction
        return 0;
    }
  FactionEntity * faction = new FactionEntity(sampleFaction);
  faction->isImage_ = true;
  // Public. Faction as it seen from outside
  faction->setName(this->getName());
  faction->setDescription(this->getDescription());
  faction->setTag(this->getTag());
  return faction;

}

// Updates current image with data
void  FactionEntity::updateImage(FactionEntity * imagetoAdd)
{
  Entity::updateImage(imagetoAdd);
  if(this->getTimeStamp() < imagetoAdd->getTimeStamp())      // Image is newer
    {
        if(imagetoAdd->getImageLevel() == PRIVATE_IMAGE)
          {
            if(!imagetoAdd->getEMail().empty())
              {
                setEMail(imagetoAdd->getEMail());
              }
            funds_ = imagetoAdd->funds_;
            stances_ = imagetoAdd->stances_;
            defaultStance_ = imagetoAdd->defaultStance_;
            knowledge_ = imagetoAdd->knowledge_;
            skillKnowledge_ = imagetoAdd->skillKnowledge_;
          }


    }
  else if(this->getTimeStamp() == imagetoAdd->getTimeStamp())// Image is the same age
    {
      if(!imagetoAdd->getEMail().empty())
        {
          setEMail(imagetoAdd->getEMail());
        }
      if(this->getImageLevel() < imagetoAdd->getImageLevel())// new image has more reliable data
        {
//

          if(imagetoAdd->getImageLevel() == PRIVATE_IMAGE)
            {
//
            }

        }
//
    }
  else                                                       // Image is older
    {
//
    }

}







STATUS FactionEntity::initialize        ( Parser *parser )
{

       if (parser->matchKeyword("EMAIL"))
	{
	  email_ = parser->getParameter();
	  return OK;
	}
       if (parser->matchKeyword("PASSWORD"))
	{
	  password_ = parser->getParameter();
	  return OK;
	}
       if (parser->matchKeyword("DEFAULIT_STANCE"))
	{
		StanceVariety * defaultStance = gameFacade->stances[parser->getWord()];
			if(defaultStance)
	   		defaultStance_ =  defaultStance;
//cout << "DEFAULIT_STANCE of "<< printTag()<< " = "<<defaultStance_->print()<<endl;
	  return OK;
	}
       if (parser->matchKeyword("STANCE"))
	{
		Entity * entity = gameFacade->factions[parser->getWord()]; // Extend to all Entities
		if(entity == 0)
			return OK;
		StanceVariety * stance = gameFacade->stances[parser->getWord()];
		if(stance == 0)
				 stance =  defaultStance_;
		stances_.push_back(StanceElement(stance,entity));
	  return OK;
	}
       if (parser->matchKeyword("FUNDS"))
	{
        ItemElement newItem = ItemElement::readItemElement(parser);
        if(newItem.isValidElement())
          funds_.push_back(newItem);
	  return OK;
	}

 if (parser->matchKeyword("TERSE"))
	{
    terseBattleReport_ = true;
		return OK;
	}
      if (parser->matchKeyword("XY"))
      {
	x0_ = (parser->getInteger());
	y0_ = (parser->getInteger());
	return OK;
      }
//       if (parser->matchKeyword("REWARD"))
//	{
//	  set(parser->getText());
//	  return OK;
//	}
//       if (parser->matchKeyword("FATE"))
//	{
//	  set(parser->getText());
//	  return OK;
//	}
//       if (parser->matchKeyword("CONTROL"))
//	{
//	  set(parser->getText());
//	  return OK;
//	}

    loadKnowledge(parser);

  return Entity::initialize(parser);

}


/*
 * Save data after processing
 */
void FactionEntity::save(ostream &out)
{
	if (!isImage_ && (isResigned_ || isDisbanded_))// Image of disbanded faction still to be saved.
	{
		return;
	}

	out << endl << getCollectionKeyword() << " " << tag_ << " " << getKeyword()
			<< endl;
	if (!name_.empty())
		out << "NAME " << name_ << endl;
	if (!description_.empty())
		out << "DESCRIPTION " << description_ << endl;
	if (!email_.empty())
		out << "EMAIL " << email_ << endl;
	if (!password_.empty())
		out << "PASSWORD " << password_ << endl;
	if (terseBattleReport_)
		out << "TERSE" << endl;
	if((x0_ != 0) || (y0_ != 0))
	{
	out << "XY " << x0_ << " " << y0_ << " " << endl;
	}
	for (ItemElementIterator fundsIter = funds_.begin(); fundsIter
			!= funds_.end(); ++fundsIter)
	{
		out << "FUNDS ";
		(*fundsIter).save(out);
	}

	if (defaultStance_)
	{

		out << "DEFAULIT_STANCE " << defaultStance_->getTag() << endl;
	}
	for (StanceIterator iter = stances_.begin(); iter != stances_.end(); ++iter)
	{
		out << "STANCE " << (*iter).getParameter1() << (*iter).getRule()
				<< endl;
	}

	saveKnowledge(out);
	saveEvents(out);
	for (vector<OrderLine *>::iterator iter = orders_.begin(); iter
			!= orders_.end(); ++iter)
	{
		(*iter)->save(out);
	}
	out << endl;

}
/*
 * Tre-turn data hadling.
 * Once-per game data initialization is also done here
 *
 */
STATUS FactionEntity::prepareData()
{
    if (IO_ERROR == Entity::prepareData()) // Check consistency of data
        return IO_ERROR;
    if (gameFacade->getGameConfig()->runMode == STARTING_TURN)
    {
        // Set factional origin of coordinates
        if (!loyalUnits_.empty())
        {
            UnitEntity * someUnit = loyalUnits_[0];
            if (someUnit)
            {
                LocationEntity * loc = someUnit->getLocation();
                if (loc)
                {
                    this->setX0(loc->getX());
                    this->setY0(loc->getY());
                }
            }
        }
    }
    // C
    this->calculateControlPoints();
    return OK;
}

/*
 * Load all orders from the input
 */
void FactionEntity::loadOrders()
{
  string orderFlename = gameFacade->getGameConfig()->getOrdersFileName(this);
  FileParser  * parser = new FileParser(orderFlename.c_str());
	if(parser->status != OK)
		return;
	else
	cout << "Orders file " << orderFlename << " opened" <<endl;
   TokenEntity * currentEntity;
    parser -> getLine() ;

   if(IO_ERROR == loadFactionOrders(parser,&currentEntity))
		{
    		 			return;
		}

	if(parser->eof()  )
		{
   		 				delete parser;
    		 			return;
		}
// ============== UNIT orders	  ====================
	do
	  {
		if (parser->matchKeyword ("END") )
		{
			delete parser;
			return;
		}

 	     	if(parser->matchChar(COMMENT_SYMBOL))  // skip  lines starting from comment
		{
			parser -> getLine();
			continue;
		}
		else
		{
			if(parser->matchChar(';'))  // overlord comment compartibility
			{
				parser -> getLine();
				continue;
			}
		}

		if(parser->matchKeyword ("UNIT"))
			{
		
				currentEntity = currentEntityOrders(gameFacade->units, parser);
				continue;
			}
		
		if(parser->matchKeyword ("CONSTRUCTION"))
			{
				currentEntity =  currentEntityOrders(gameFacade->buildingsAndShips, parser);
				continue;
			}
                if(parser->matchKeyword ("BUILDING"))
                {
                  currentEntity =  currentEntityOrders(gameFacade->buildingsAndShips, parser);
                  continue;
                }

		  if(currentEntity == 0)
			  {
  			  	parser -> getLine();
  				continue;
			  }
		// Then it is order line
		string order =  parser->getText();
		if( !order.empty() )
			{
//    				cout <<currentEntity->getName() << ": Entity order " << order<<endl;
				currentEntity  -> addOrder(order);
			}

  			   parser -> getLine();
 } while   (!  parser -> eof() );



		delete parser;

}

/*
 * Load faction orders from the input
 */
STATUS  FactionEntity::loadFactionOrders(Parser * parser, TokenEntity ** entity)
{
 string currentGame ;
 string order;
 bool gridSymbol= false;
 while(true) // check section before first UNIT or CONSTRUCTION keyword
  {
		gridSymbol = parser->matchChar('#'); // for compartibility with old Overlord
		if (parser->matchKeyword ("GAME"))
    		{
   		 		if ( ciStringCompare( tag_, parser->getWord()) )
   		 			{
    		 			cerr << "Invalid orders (wrong faction id) for faction " << tag_<< endl;
    		 			delete parser;
    		 			return IO_ERROR;
    		 		}
    		 	if ( password_ != parser->getParameter() )   // case sensitive!
    		 		{
    		 			cerr << "Invalid orders (wrong faction password) for faction " << tag_<< endl;
    		 			delete parser;
    		 			return IO_ERROR;
    		 		}

   		 		currentGame = parser->getWord();
   		 		if( !currentGame.empty() )
   		 			{
   		 				if ( ciStringCompare( gameFacade->getGameConfig()->getGameId(),  currentGame )  ) // take care about case
    		 				{
    		 					cerr << "Invalid orders (wrong game id) for faction " << tag_ << endl;
     		 					delete parser;
    		 					return IO_ERROR;
    		 			}
    		 		}
       	 parser -> getLine();
    		clearOrders();  // clear current faction orders
      		continue;
    		}


 	 if (parser->matchKeyword ("END") )
    		{
    		 			delete parser;
    		 			return IO_ERROR;
     		}
  if(gridSymbol)
	{
		gridSymbol = false;
		parser->rewind(-1);
	}
  if(parser->matchKeyword ("UNIT"))
    {
      *entity = currentEntityOrders(gameFacade->units, parser);
      return OK;
    }

  if(parser->matchKeyword ("CONSTRUCTION"))
    {
      *entity = currentEntityOrders(gameFacade->buildingsAndShips, parser);
      return OK;
    }
  //-------------------------------------------------------
    		// Read Faction orders:

/*	if(parser->matchChar(COMMENT_SYMBOL))  // skip  lines starting from comment
	  {
		  continue;
	  }
	if(parser->matchChar(';'))  // overlord comment compartibility
	  {
			continue;
	  }*/
//	else
//	  {
		 order =  parser->getText();
		  if( !order.empty() )
			  {
//          				cout << "Faction order " << order<<endl;
				  orders_.push_back(new OrderLine(order, this));
			  }
//	  }

  parser -> getLine();

 }
}



/*
 * Load  from the input orders for one entity
 */
TokenEntity * FactionEntity::currentEntityOrders(BasicEntitiesCollection & collection,
                                  Parser * parser)
{
  string entityName =  parser->getWord();
 TokenEntity * currentEntity = 0;
 FactionEntity * faction = 0;
	// First check it for being new entity placaholder
  if(gameFacade->getGameConfig()->isNewEntityName(entityName,faction))
    {
      NewEntityPlaceholder * placeholder = gameFacade->getDataManipulator()->findOrAddPlaceholder(entityName);
      if(placeholder != 0)  // this is  placeholder. Now we'll create new unit but will
			// not register it untill it is really created
        {
//          GameData* realEntity = placeholder->getRealEntity();
//         if(realEntity) //
//					{
//						currentEntity = realEntity
//					}
//          else   // placeholder is still empty. use temporary entity to store orders
//					{
	currentEntity = placeholder->getNewEntity();
	if(faction ==0) // Faction was omited. Default is this
	faction = this;
	currentEntity->setFaction(faction);
//					}
        }
		}
  else
	{
  	GameData * current = collection.findByTag(entityName);
  	if(current == 0)
			{
		  	cerr << "Wrong entity name " << entityName <<endl;
  			parser -> getLine();
  			return 0;
			}

  	currentEntity = dynamic_cast<TokenEntity *>(current);
  	if(currentEntity == 0)
			{
		  	cerr << "Wrong entity name " << entityName <<endl;
  			parser -> getLine();
  			return 0;
			}
	}

  if(  currentEntity -> getFaction() != this )
		{
		  cerr << "Attempt to give orders to foreign entity " <<  currentEntity -> getTag() << endl;
		  currentEntity =0;
   	  	parser -> getLine();
  		return 0;
		}
  else
		{
		  currentEntity -> clearOrders();
   	  	parser -> getLine();
  		return currentEntity;
		}

}
// This function extracts faction image from image
void FactionEntity::addFactionImage(FactionEntity * factionToAdd)
{
    if (factionToAdd == 0)
    {
        return;
    }
    if(factionToAdd == this)
    {
    	return;
    }
    //cerr<< "Adding image of "<<image->print()<<" to "<<print()<<" ";

    for(vector<FactionEntity *>::iterator iter = knownFactions_.begin();
            iter != knownFactions_.end(); ++iter)
    {
        if ((*iter)->getTag() == factionToAdd->getTag())
        {
            (*iter)->updateImage(factionToAdd);
            //cerr<<imageToAdd->print()<<" already recorded."<< endl;
           return;
        }

    }
    //cerr<<imageToAdd->print()<<" added"<< endl;
    FactionEntity * imageToAdd = factionToAdd->createFactionImage(this);
        knownFactions_.push_back(imageToAdd);
}



//// This function extracts faction image from image
//void FactionEntity::addFactionImage(TokenEntity * image)
//{
//    if (image == 0)
//    {
//        return;
//    }
//
//    FactionEntity * factionToAdd = image->getFaction();
//    if (factionToAdd == 0)
//    {
//        return;
//    }
//    if(factionToAdd == this)
//    {
//    	return;
//    }
//    //cerr<< "Adding image of "<<image->print()<<" to "<<print()<<" ";
//    FactionEntity * imageToAdd = factionToAdd->createFactionImage(this);
//
//    for(vector<FactionEntity *>::iterator iter = knownFactions_.begin();
//            iter != knownFactions_.end(); ++iter)
//    {
//        if ((*iter)->getTag() == imageToAdd->getTag())
//        {
//            (*iter)->updateImage(imageToAdd);
//            // delete one of two images?
//            //cerr<<imageToAdd->print()<<" already recorded."<< endl;
//           return;
//        }
//
//    }
//    //cerr<<imageToAdd->print()<<" added"<< endl;
//        knownFactions_.push_back(imageToAdd);
//}


// This function extracts faction image from real Token
void FactionEntity::extractFactionImage(TokenEntity * tokenToAdd)
{
    if(tokenToAdd == 0)
    {
        return;
    }
    // Check faction of unit. If it is visible search for this faction in knownFactions_
    // if not present - create and add image of faction othervise - update.

    if(tokenToAdd->mayBeIdentified(this))
    {
        FactionEntity * factionToAdd =tokenToAdd->getFaction();
        FactionEntity * imageToAdd = factionToAdd->createFactionImage(this);
    for(vector<FactionEntity *>::iterator iter = knownFactions_.begin();
            iter != knownFactions_.end(); ++iter)
    {
        if ((*iter)->getTag() == imageToAdd->getTag())
        {
            (*iter)->updateImage(imageToAdd);
            // delete one of two images?
            return;
        }

    }
        knownFactions_.push_back(imageToAdd);
    }
}
//
//void FactionEntity::addUnitImage(UnitEntity * imageToAdd)
//{
//    if(imageToAdd)
//    {
//      addFactionImage(imageToAdd);
//      for(vector<UnitEntity *>::iterator iter =observedUnits_.begin();
//                iter != observedUnits_.end(); ++iter )
//        {
//            if((*iter)->getTag() == imageToAdd->getTag())
//            {
//                delete *iter;
//                *iter = imageToAdd;
//                return;
//            }
//        }
//            observedUnits_.push_back(imageToAdd );
//    }
//}
//
//void FactionEntity::createAndAddUnitImage(UnitEntity * unitToAdd, TokenEntity * referent)
//{
//    addFactionImage(unitToAdd);
//
//    // if image is not collected - create. otherwise - update
//    UnitEntity * imageToAdd = unitToAdd->createUnitImage(referent);
//    if(imageToAdd)
//    {
//        for(vector<UnitEntity *>::iterator iter =observedUnits_.begin();
//                iter != observedUnits_.end(); ++iter )
//        {
//            if((*iter)->getTag() == imageToAdd->getTag())
//            {
//                delete *iter;
//                *iter = imageToAdd;
//                return;
//            }
//        }
//            observedUnits_.push_back(imageToAdd );
//    }
//}
//
//
//
//
//void FactionEntity::addConstructionImage(ConstructionEntity * imageToAdd)
//{
//    if(imageToAdd)
//    {
//        addFactionImage(imageToAdd);
//        for(vector<ConstructionEntity *>::iterator iter =observedConstructions_.begin();
//                iter != observedConstructions_.end(); ++iter )
//        {
//            if((*iter)->getTag() == imageToAdd->getTag())
//            {
//                delete *iter;
//                *iter = imageToAdd;
//                return;
//            }
//        }
//            observedConstructions_.push_back(imageToAdd );
//    }
//
//
//}
//void FactionEntity::addEffectImage(EffectEntity * imageToAdd)
//{
//   if(imageToAdd)
//    {
//        addFactionImage(imageToAdd);
//
//        for(vector<EffectEntity *>::iterator iter =observedEffects_.begin();
//                iter != observedEffects_.end(); ++iter )
//        {
//            if((*iter)->getTag() == imageToAdd->getTag())
//            {
//                delete *iter;
//                *iter = imageToAdd;
//                return;
//            }
//        }
//            observedEffects_.push_back(imageToAdd );
//    }
//}


/*
 * Add location to the list of discovered locations
 * if it is not yet in this list
 * These are locations, connected to visited locations
 */
void FactionEntity::addDiscoveredLocation(LocationEntity * location)
{
    if(visitedLocations_.end() != find(visitedLocations_.begin(),
            visitedLocations_.end(), location )  )
    {
        return; //Location is already visited
    }

    if(discoveredLocations_.end() == find(discoveredLocations_.begin(),
        discoveredLocations_.end(), location )  )
    {
        discoveredLocations_.push_back(location->createBasicImage(this));
    }


}

/*
 * Add location to the list of locations visited
 * if it is not yet in this list
 * If it is, then try to update info.
 */
bool FactionEntity::addVisitedLocation(LocationEntity * location)
{
	vector<LocationEntity *>::iterator iter = find(visitedLocations_.begin(),
            visitedLocations_.end(), location );

    if(iter == visitedLocations_.end())// not found
    {
        location->addNeighbours(this);
        visitedLocations_.push_back(location);
        return true;
    }

    return false;
}

//// add LocationImage to faction. replace if older exists.
// void FactionEntity::addLocationImage(LocationEntity * image)
//{
//       for(vector<LocationEntity *>::iterator iter = visitedLocations_.begin();
//                iter != visitedLocations_.end(); ++iter )
//        {
//            if((*iter)->getTag() == image->getTag())
//            {
//                delete *iter;
//                *iter = image;
//                return;
//            }
//        }
//        visitedLocations_.push_back(image);
// }

/*
 * Delete all entries from the list of visited locations
 */
void FactionEntity::clearVisitedLocations(){
        visitedLocations_.clear();
}



bool FactionEntity::isVisitedLocation(LocationEntity * location)
{
if(visitedLocations_.end() != find(visitedLocations_.begin(),  visitedLocations_.end(), location )  )
{
    return true;
}
else
{
    return false;
}

}



/*
 * Process all events reported by entities and select those,
 * that will be used for report
 */
void FactionEntity::dailyReport()
{
// cout <<"Daily report for faction ["<< tag_ <<"]"<<endl;
  vector< UnitEntity *>::iterator unitIterator;
  vector< ReportElement *>::iterator reportIterator;
 for ( unitIterator = loyalUnits_.begin(); unitIterator != loyalUnits_.end(); unitIterator++)
	{
 		vector <ReportElement *> unitReporting;
   if( (*unitIterator)->getReportDestination() == 0)
    continue;
    else
       (*unitIterator)->getReportDestination()->extractReport((*unitIterator), unitReporting);
		for ( reportIterator = unitReporting.begin(); reportIterator != unitReporting.end(); reportIterator++)
			{
//				 cout << "[Report extracted] "; (*reportIterator)->getRecord()->print(cout);
				updateReports( (*reportIterator));
			}
		 unitReporting.clear();
	}
}

/*
 * Collect all events observed by entities
 */
void FactionEntity::dailyEventProcessing()
{
// cout <<"Daily event collection for faction ["<< tag_ <<"]"<<endl;
  vector< UnitEntity *>::iterator unitIterator;
  EventIterator eventIterator;
 for ( unitIterator = loyalUnits_.begin(); unitIterator != loyalUnits_.end(); unitIterator++)
	{
 		vector <Event * > unitReporting;
    // Collect events from location?
                // from all present units?
// 		if((*unitIterator)->getReportDestination() == gameFacade->locations["L62"])
// 		{
// 			cout << (*unitIterator)->getTag() << " tries to extract events from "<< (*unitIterator)->getReportDestination()->getTag()<<endl;
// 		}

   if( (*unitIterator)->getReportDestination() == 0)
    continue;
    else
       (*unitIterator)->getReportDestination()->extractEventsImages((*unitIterator), unitReporting);
		for ( eventIterator = unitReporting.begin(); eventIterator != unitReporting.end(); eventIterator++)
			{
				cout << "[Event "<<(*eventIterator)->getTag() <<" extracted] "<<endl;
				updateEvents( (*eventIterator));
			}
		 unitReporting.clear();
	}
}

bool locOrder(LocationEntity *l1,LocationEntity *l2)
{
	if(gameFacade->locations.getIndex(l1->getTag()) < gameFacade->locations.getIndex(l2->getTag()))
		return true;
	else
		return false;
}
/*
 * Adds report to the list if it is different from those,
 * that are already there
 */
void FactionEntity::updateReports(ReportElement * report)
{
    for (ReportIterator reportIterator = collectedReports_.begin(); reportIterator != collectedReports_.end(); reportIterator++)
    {
        if (report->getRecord() == (*reportIterator)->getRecord())
        {
            delete report;
            return;
        }
    }
    collectedReports_.push_back(report);
}
/*
 * Adds report to the list if it is different from those,
 * that are already there
 */
void FactionEntity::updateEvents(Event *  event)
{
    for (EventIterator iter =collectedEvents_.begin(); iter != collectedEvents_.end(); iter++)//
    {
        if (event->getTag() == (*iter)->getTag())
        {
            return;
        }
    }
    // <- insert into the final list of events collectedReports_?
   // collectedEvents_.push_back(event);
}

ReportPrinter outfile;

/*
 * Creates turn report file for faction
 */
void FactionEntity::saveReport()
{
    outfile.open(gameFacade->getGameConfig()->getReportFileName(this));
	cout << gameFacade->getGameConfig()->getReportFileName(this)<<endl;
	if(!isNPCFaction())
	{
    		reportlist<< gameFacade->getGameConfig()->getReportFileName(this)<<endl;
	}

// Mail header
	outfile << "To: " << email_<<"\n";
  	outfile << "Subject:"<< gameFacade->getGameConfig()->getGameId()<<" Report for Turn "
	        << gameFacade->getGameTurn() << endl;
	outfile << "From: "<< gameFacade->getGameConfig()->getServer()
	        <<" ("<<gameFacade->getGameConfig()->getServerName()<< " )"<< endl;
	outfile << "\n";

// Report
   outfile << gameFacade->getGameConfig()->getGameId() <<" Report for Turn "<< gameFacade->getGameTurn()
	         <<" for "<< print() << ", "<<email_<<"\n";
   if(isResigned_)
      {
       outfile<< "---- Resigned. ----"<<endl;
       outfile <<"END"<<endl;
       outfile.close();
       return;
      }
     outfile << "Next turn: "<<gameFacade->getGameConfig()->getDeadline()<<endl;
	 outfile <<  endl << "// Faction stats " <<  endl;
   reportFunds(outfile);
   outfile << "Control Points: " << getControlPoints() <<" of "
	         << getMaxControlPoints()<< endl;
   outfile <<  endl;

//   outfile << "Combat units: " <<endl;
//   long attackRating;
//   long totalAttackRating=0;
//   long defenceRating;
//   long totalDefenceRating=0;
//   long totalCombatRating;
// for ( vector< UnitEntity *>::iterator unitIterator = loyalUnits_.begin(); unitIterator != loyalUnits_.end(); unitIterator++)
//	{
//            attackRating = (*unitIterator)->getAttackRating()/100;
//            defenceRating =(*unitIterator)->getDefenceRating()/100;
//            if(attackRating != 0 || defenceRating != 0 )
//            {
// 		outfile << (*unitIterator)->print()<<" AR: "<<attackRating
//                        <<" DR: "<<defenceRating<<" ("
//                        <<(*unitIterator)->getFiguresNumber()<<") "<<endl;
//                totalAttackRating += attackRating;
//                totalDefenceRating += defenceRating;
//            }
//	}
//  totalCombatRating = (totalAttackRating * totalAttackRating);
//  outfile << "Total Combat rating: " <<totalCombatRating<<" ("<<totalAttackRating <<"*"<< totalDefenceRating<<")" <<endl;

   outfile << "// Diplomacy  (Stances)" <<  endl <<  endl;
   outfile <<  "Default attitude is " <<defaultStance_->getName()<<  endl;
     if(!stances_.empty())
   {
//   outfile <<  "Attitude to " <<" is "<<defaultStance_->getName()<<  endl;

    for(StanceIterator iter = stances_.begin(); iter != stances_.end();++iter)
      {
   outfile <<  "Attitude to " <<(*iter).getParameter1()->print()<<" is "<<(*iter).getRule()->getName() <<  endl;

      }
   }
//--------------

   outfile <<  endl << "// Global Events " <<  endl <<  endl;
   for (ReportIterator reportIterator = collectedReports_.begin(); reportIterator != collectedReports_.end(); /*reportIterator++*/)
  {
      if (  this == (*reportIterator)->getDestination())
	{
               (*reportIterator)->printReport(outfile);
               extractedReports_.push_back(*reportIterator); // may cause beeng copied twice
              reportIterator = collectedReports_.erase(reportIterator);
	}
            else
              reportIterator++;
  }

   outfile <<  endl << "// Units " <<  endl <<  endl;

 for ( vector< UnitEntity *>::iterator unitIterator = loyalUnits_.begin(); unitIterator != loyalUnits_.end(); unitIterator++)
	{
 		(*unitIterator) -> produceFactionReport(this, outfile);
	}

   outfile <<  endl << "// Constructions " <<  endl <<  endl;
 for ( vector< ConstructionEntity *>::iterator constructionIterator = loyalConstructions_.begin(); constructionIterator != loyalConstructions_.end(); constructionIterator++)
	{
 		(*constructionIterator) -> produceFactionReport(this, outfile);
	}

   outfile <<  endl << "// Locations " <<  endl <<  endl;

  vector< LocationEntity *>::iterator locIterator;
//  ReportIterator reportIterator;
    bool eventsReported = false;
    //locOrder(*(visitedLocations_.begin()),*(visitedLocations_.end()));
    std::sort(visitedLocations_.begin(), visitedLocations_.end(), locOrder);

    for (locIterator = visitedLocations_.begin(); locIterator != visitedLocations_.end(); locIterator++)
    {
        (*locIterator) -> produceFactionReport(this, outfile);
        outfile << "\nEvents during turn: \n";
        eventsReported = false;
        for (ReportIterator reportIterator = collectedReports_.begin(); reportIterator != collectedReports_.end();)
        {
            if ((*locIterator) == (*reportIterator)->getDestination())
            {
                (*reportIterator)->printReport(outfile);
                reportIterator = collectedReports_.erase(reportIterator);
                eventsReported = true;
            } else
                reportIterator++;
        }
        if (!eventsReported)
            outfile << "   - Nothing notable\n";
        outfile << endl;
    }
    outfile << endl << "// Combat " << endl << endl;

    for (vector< CombatReport *>::iterator iter = combatReports_.begin();
            iter != combatReports_.end(); ++iter)
    {
        outfile << *(*iter);
    }


    outfile << endl << "// Knowledge " << endl << endl;

    reportNewKnowledge(outfile);
    outfile.max_width(60); // Make string shorter for reports

    outfile << endl << endl << "#============== Orders Template ==========" << endl << endl;

    outfile << "GAME " << getTag() << " " << password_ << " " << gameFacade->getGameConfig()->getGameId() << endl << endl;

    for (vector< UnitEntity *>::iterator unitIterator = loyalUnits_.begin(); unitIterator != loyalUnits_.end(); unitIterator++)
    {
        (*unitIterator) -> printOrderTemplate(outfile);
    }

    outfile << endl << "#    --- Constructions ---" << endl << endl;
    for (vector< ConstructionEntity *>::iterator constructionIterator = loyalConstructions_.begin(); constructionIterator != loyalConstructions_.end(); constructionIterator++)
    {
        (*constructionIterator) -> printOrderTemplate(outfile);
    }
    outfile << "END" << endl;
    outfile.close();
}
/*
 * Creates serialized turn report file for faction
 */
void FactionEntity::saveCReport()
{

    outfile.open(gameFacade->getGameConfig()->getCReportFileName(this));
    cout << gameFacade->getGameConfig()->getCReportFileName(this) << endl;
    if (!isNPCFaction())
    {
        reportlist << gameFacade->getGameConfig()->getCReportFileName(this) << endl;
    }

    outfile <<"GAME "<< gameFacade->getGameConfig()->getGameId()<<endl;
    outfile <<"TURN "<< gameFacade->getGameTurn()<<endl;
    outfile <<"REPORT "<< getTag()<<endl;

    // Save entities data as they seen by faction
    outfile <<"LOCATIONS "<< getTag()<<endl;
   for(vector<LocationEntity *>::iterator iter = visitedLocations_.begin();
           iter != visitedLocations_.end(); ++iter)
  {
      (*iter)->save(outfile);
  }
  for( vector<LocationEntity *>::iterator iter =  discoveredLocations_.begin();
          iter != discoveredLocations_.end(); ++iter)
  {
      (*iter)->save(outfile);
  }

   outfile <<"UNITS "<< getTag()<<endl;
  for(vector<UnitEntity *>::iterator iter =  observedUnits_.begin();
          iter != observedUnits_.end(); ++iter)
  {
      (*iter)->save(outfile);
  }

  outfile <<"CONSTRUCTIONS "<< getTag()<<endl;
  for(vector<ConstructionEntity *>::iterator iter = observedConstructions_.begin();
          iter != observedConstructions_.end(); ++iter)
  {
     (*iter)->save(outfile);
  }

  outfile <<"EFFECTS "<< getTag()<<endl;
  for(vector<EffectEntity *>::iterator iter =  observedEffects_.begin();
          iter != observedEffects_.end(); ++iter)
  {
      (*iter)->save(outfile);
  }

  outfile <<"FACTIONS "<< getTag()<<endl;
  this->save(outfile);
  for(vector<FactionEntity *>::iterator iter =  knownFactions_.begin();
          iter != knownFactions_.end(); ++iter)
  {
     (*iter)->save(outfile);
  }


    // Save New Faction Knowledge
  outfile <<endl<<"KNOWLEDGE "<< getTag()<<endl;
   for(KnowledgeIterator iter = knowledge_.begin() + newKnowledge;
            iter != knowledge_.end(); ++iter)
    {
         (*iter)->save(outfile);
    }
    for(SkillLevelIterator iter = skillKnowledge_.begin() + newSkillKnowledge;
            iter != skillKnowledge_.end(); ++iter)
    {
       //(*iter)->getSkill()->saveLevel(outfile, (*iter)->getLevel());
       (*iter)->saveLevelElement(outfile);
    }

    // Save Events

    outfile << "END" << endl;
    outfile.close();
}


vector <Rule *> FactionEntity::getAllKnowledge()
{
vector <Rule *> knowledge;
for(KnowledgeIterator iter = knowledge_.begin() + newKnowledge;
         iter != knowledge_.end(); ++iter)
 {
      knowledge.push_back(*iter);
 }
    return knowledge;
}


vector <SkillLevelElement *> FactionEntity::getAllSkillKnowledge()
{
vector <SkillLevelElement *> knowledge;
for(SkillLevelIterator iter = skillKnowledge_.begin() + newSkillKnowledge;
        iter != skillKnowledge_.end(); ++iter)

 {
      knowledge.push_back(*iter);
 }
    return knowledge;
}



vector <ReportElement *> FactionEntity::getEvents()
{
  if (extractedReports_.size() != 0)
  {
    return extractedReports_;
  }
  else
  {
   for (ReportIterator reportIterator = collectedReports_.begin(); reportIterator != collectedReports_.end(); /*reportIterator++*/)
    {
      if (  this == (*reportIterator)->getDestination())
      {
       extractedReports_.push_back(*reportIterator);
      }
      else
      {
       cout <<(*reportIterator)->getDestination()->print()<<endl;
        reportIterator++;
        }
    }
    return extractedReports_;
  }
}

/*
 * Daily post-processing
 */
void FactionEntity::dailyUpdate()
{
   dailyReport();
   dailyEventProcessing();
   dailyImageProcessing();
}


/*
 * Daily image creation and updating
 */
void FactionEntity::dailyImageProcessing()
{
     // Generate list of visited locations
    LocationEntity * location = 0;
    vector< LocationEntity *>::iterator locIterator;
    for (vector< UnitEntity *>::iterator iter = loyalUnits_.begin(); iter != loyalUnits_.end(); ++iter)
    {
        location = (*iter)->getLocation();
        if (location)
        {
            vector<LocationEntity *>::iterator locIterator = find(visitedTodayLocations_.begin(),
                                                                  visitedTodayLocations_.end(), location);
            if (locIterator == visitedTodayLocations_.end())
            {
                visitedTodayLocations_.push_back(location);
            }
        }
    }


    for (locIterator = visitedTodayLocations_.begin(); locIterator != visitedTodayLocations_.end(); locIterator++)
    {
        this->getTurnReport()->addLocationImage(*locIterator);
    }
    visitedTodayLocations_.clear();
}



/*
 * Monthly post-processing
 */
void FactionEntity::postProcessData()
{
	if(isUnknownEntity())
	{
		return;
	}
}



/*
 * Register unit as belonging to faction
 * Called when old unit loaded from saved data,
 * new unit created or unit transfered to faction.
 */
void FactionEntity::addUnit(UnitEntity * unit)
{
	if (unit)
		{
			loyalUnits_.push_back(unit);
//			cout << unit->getName() << " " << unit->printTag() << " registered as loyal unit of faction " << printTag()<<endl;
       unit->setFaction(this);
////       controlPoints_ += unit->getControlPoints();
       // if has title add title CP
       // if has landTitle - change land ownership
       // controlled monsters?
		}
	else
	{
		cerr << "Cant register" << unit->print() <<" to faction " << printTag()<<endl;
	}
}



/*
 * Remove unit from faction
 * Called when unit disbanded or  transfered to other faction.
 */
void FactionEntity::removeUnit(UnitEntity * unit)
{
	if (!unit)
		{
		  cerr << "Cant remove" << unit->print() <<" from faction " << printTag()<<endl;
       return;
    }

 vector< UnitEntity *>::iterator iter = find (loyalUnits_.begin(), loyalUnits_.end(), unit);
	if (iter == loyalUnits_.end())
		{
		  cerr << "Cant find" << unit->print() <<" in faction " << printTag()<<endl;
       return;
    }
////       controlPoints_ -= unit->getControlPoints();
       // if has title add title CP
       // if has landTitle - change land ownership
       // controlled monsters?
			loyalUnits_.erase(iter);
}



/*
 * Register construction as belonging to faction
 * Called when old construction loaded from saved data,
 * new construction builded, bought or conquered .
 */
void FactionEntity::addConstruction(ConstructionEntity * construction)
{
	if (construction)
		{
			loyalConstructions_.push_back(construction);
//			cout << unit->getName() << " " << unit->printTag() << " registered as loyal unit of faction " << printTag()<<endl;
       construction->setFaction(this);
		}
	else
	{
		cerr << "Cant register" << construction->print() <<" to faction " << printTag()<<endl;
	}
}



/*
 * Remove construction from faction
 * Called when construction destroyed, sold or conquered by other faction.
 */
void FactionEntity::removeConstruction(ConstructionEntity * construction)
{
  	if (!construction)
		{
		  cerr << "Cant remove" << construction->print() <<" from faction " << printTag()<<endl;
       return;
    }

 vector< ConstructionEntity *>::iterator iter = find (loyalConstructions_.begin(), loyalConstructions_.end(), construction);
	if (iter == loyalConstructions_.end())
		{
		  cerr << "Cant find" << construction->print() <<" in faction " << printTag()<<endl;
       return;
    }

			loyalConstructions_.erase(iter);

}



/*
 * Returns reference to Entity, to whom entity's events should be reported
 */
Entity * FactionEntity::getReportDestination(){
return this;
}


/*
 * Determines faction's stance toward some unit.
 * Faction may have special stance to some unit.
 * Otherwise, if faction may determine to which faction belongs this unit
 * stance to it's faction will be returned.
 * Filally default stance will be used.
 */
long wcounter8 = 0;
StanceVariety * FactionEntity::getStance(TokenEntity * token)
{
    FactionEntity * faction = token->getFaction();
    if (faction == 0) // Unowned constructions
    {
        return defaultStance_;
    }

    if (faction == this)
    {
        return privateStance;
    }

    for (StanceIterator iter = stances_.begin(); iter != stances_.end(); iter++)
    {
        if ((*iter).getParameter1() == token)
            return (*iter).getRule();
    }
    // Check that we can determine unit's owner
    if (token->getAdvertising())
        return getStance(faction);
    LocationEntity * location = token->getLocation();
    if(location == 0)
    {
        cerr << "Trying to determine stances to "<<token->print()<<" that has no location" <<endl;
        return getStance(faction);
    }
//    	  wcounter8 ++;
//	  if(wcounter8%1000 ==0)
//	    cerr <<"Day "<<gameFacade->getCurrentDay() <<" getStance  counter = "<< wcounter8/1000<<",000" <<endl;
    if (token->getStealth() < location->getFactionalObservation(this))
        return getStance(faction);
    else
    { // we allways know identity of allied factions
        if (faction->stanceAtLeast(this, alliedStance))
            return getStance(faction);
        else // otherwise default
            return defaultStance_;
    }

}



/*
 * Determines faction's stance toward faction.
 * Faction may have special stance set toward it.
 * Otherwise  default stance will be used.
 */
StanceVariety * FactionEntity::getStance(FactionEntity * faction)
{
 if  (faction == this)
 {
    return privateStance;
 }
	for (StanceIterator iter = stances_.begin(); iter != stances_.end(); ++iter)
		{
			if ( (*iter).getParameter1() == faction)
				return (*iter).getRule();
		}
		return  defaultStance_;
}




/*
 * Set stance toward faction or unit
 */
void FactionEntity::setStance(Entity * entity, StanceVariety * stance)
{
  if( entity == this)
  {
      defaultStance_ = stance;
      return;
  }

  for (StanceIterator iter = stances_.begin(); iter != stances_.end(); ++iter)
		{
			if ( (*iter).getParameter1() == entity)
				 (*iter).setRule(stance);
		}
	stances_.push_back(StanceElement(stance,entity));
}



/*
 * Withdraw some coins or items from faction funds
 */
int FactionEntity::withdraw(ItemRule * item, int number)
{
  int num;
  if(item == 0)
      return 0;

  for (ItemElementIterator fundsIter  = funds_.begin();
				fundsIter != funds_.end(); fundsIter++)
    {
           if((*fundsIter).getItemType() == item)
              {
                num = (*fundsIter).getItemNumber();
                if(num >= number)
                  {
                   (*fundsIter).setItemNumber(num - number);
                   return number;
                  }
                 else
                 {
                   funds_.erase(fundsIter);
                   return num;
                 }
              }
    }
  return 0;
}



/*
 * Determines either faction may withdraw specified amount of item
 * (or coins)
 */
bool FactionEntity::mayWithdraw(ItemRule * item, int number)
{
  if(item == 0)
      return false;

  for (ItemElementIterator fundsIter  = funds_.begin();
				fundsIter != funds_.end(); fundsIter++)
    {
      if((*fundsIter).getItemType() == item)
        {
          if(number <= (*fundsIter).getItemNumber())
            return true;
          else
            return false;
        }
    }
    return false;
}


//===============================  Knowledge ============
/** Faction keeps record of rules that are known to it for reporting purpose */
/* Here also we can add it to TurnReporter*/
bool FactionEntity::addKnowledge(Rule * info)
{
  if(info == 0)
      return false;

  
  if(!hasKnowledge(info))
  {
    knowledge_.push_back(info);
    info->extractKnowledge(this);
    turnReport_->addRule(info);
    return true;
    }
    else
      return false;
}



/** No descriptions */
bool FactionEntity::addSkillKnowledge(SkillRule * knowledge, int level)
{
  if(knowledge == 0)
      return false;
  
  if(!hasSkillKnowledge(knowledge,level))
  {
    skillKnowledge_.push_back(new SkillLevelElement(knowledge,level));
    knowledge->extractKnowledge(this,level);
    turnReport_->addSkillKnowledge(knowledge,level);
    return true;
  }
  else
      return false;
}



/** No descriptions */
Rule * FactionEntity::hasKnowledge(Rule * info)
{
    for(KnowledgeIterator iter = knowledge_.begin(); iter != knowledge_.end(); ++iter)
    {
      if((*iter)->getTag()== info->getTag())
      {
        return (*iter);
      }
    }
    return 0;


 // KnowledgeIterator  iter = find(knowledge_.begin(), knowledge_.end(),info) ;

//   if ( iter != knowledge_.end())
//     return *iter;
//   else
//       return 0;
}


/*
 * If faction already knows  this skill on given level (or higher)
 * returns SkillLevelElement at which faction actually knows skill.
 * Otherwise returns 0.
 */
SkillLevelElement * FactionEntity::hasSkillKnowledge(SkillRule * knowledge, int level)
{
  for(SkillLevelIterator iter = skillKnowledge_.begin(); iter != skillKnowledge_.end(); ++iter)
    {
      if( (*iter)->getSkill() == knowledge  && (*iter)->getLevel() >= level )
        return (*iter);
    }
  return 0;

}



/*
 * Prints rule knowledge if it is known to faction.
 */
void FactionEntity::reshow(Rule * info, ReportPrinter &out)
{
  if(hasKnowledge(info))
       hasKnowledge(info)->printDescription(out);
}



/** No descriptions */
void FactionEntity::reshow(BasicRulesCollection  * collection, ReportPrinter &out )
{
  for(RulesIterator iter = collection->begin();iter != collection->end(); ++iter )
    {
      reshow(*iter, out);
    }

}



/** No descriptions */
void FactionEntity::reshowSkills(ReportPrinter &out )
{
//  for(RulesIterator iter = skills.begin();iter != skills.end(); ++iter )
  for(int i=0; i < gameFacade->skills.size(); ++i )
  {
    for (int level = 0; level < SkillRule::getMaxSkillLevel(); ++level)
    {
      reshowSkill( gameFacade->skills[i], level,out);
    }
  }
}



/** No descriptions */
void FactionEntity::reshowSkill(SkillRule * knowledge, int level, ReportPrinter &out)
{

  if(level != 0)
    {
    if(hasSkillKnowledge(knowledge,level))
       knowledge->printSkillDescription(level,out);
     return;
    }

  else
    level = SkillRule::getMaxSkillLevel();

  for(int i = 1; i <= level; ++i)
  {
    if(hasSkillKnowledge(knowledge,i))
       knowledge->printSkillDescription(i,out);
    else
      return;
  }
}


/** No descriptions */
void FactionEntity::saveKnowledge(ostream &out)
{
  for(SkillLevelIterator iter = skillKnowledge_.begin(); iter != skillKnowledge_.end(); ++iter)
    {
        out<< "SKILL_KNOWLEDGE "<< (*iter)->getSkill()->getTag()<<" "<< (*iter)->getLevel()<<endl;
    }


  for(KnowledgeIterator iter = knowledge_.begin(); iter != knowledge_.end(); ++iter)
    {
        out<<"KNOWLEDGE "<< (*iter)->getTag()<<endl;
    }
}



void FactionEntity::markCollectionToReshow(BasicRulesCollection  * collection)
{
  if(collection->getCollectionKeyword()  == gameFacade->skills.getCollectionKeyword())
    {
      markAllSkillsToReshow();
    }
  collectionsToReshow_.push_back(collection);
}


void FactionEntity::markKnowledgeToReshow(Rule  * knowledge)
{
 knowledgeToReshow_.push_back(knowledge);
}



void FactionEntity::markSkillToReshow(SkillRule * skill, int level)
{
 skillsToReshow_.push_back(SkillLevelElement(skill,level));
}



/*
 * Print into report all new knowledge and knowledge marked for reshowing
 */
void FactionEntity::reportNewKnowledge(ReportPrinter &out)
{
  bool isFirst = true;
  SkillLevelIterator skillIter;
  if(allSkillsToReshow_)
    skillIter = skillKnowledge_.begin();
  else
    skillIter = skillKnowledge_.begin() + newSkillKnowledge;
  if((skillKnowledge_.size() > newSkillKnowledge) || allSkillsToReshow_)
  {
    out << "\nSkill knowledge:\n"<<endl<<endl;
    isFirst = false;
    for(; skillIter != skillKnowledge_.end(); ++skillIter)
      {
//        out<<endl<<endl;
        reshowSkill((*skillIter)->getSkill(), (*skillIter)->getLevel(),out);
//        (*iter)->getSkill()->printDescription((*iter)->getLevel(),out);
      }
   }
  if( (skillsToReshow_.size() != 0) && !allSkillsToReshow_)
    {
     if(isFirst)
         out << "\nSkill knowledge:\n"<<endl<<endl;
      for(vector<SkillLevelElement>::iterator iter = skillsToReshow_.begin(); iter != skillsToReshow_.end(); ++iter)
      {
        reshowSkill((*iter).getSkill(), (*iter).getLevel(),out);
      }

    }


  allSkillsToReshow_ = false;
  skillsToReshow_.clear();
  for(vector < BasicRulesCollection  *>::iterator collIter = gameFacade->ruleIndex.getAllRules().begin();
          collIter != gameFacade->ruleIndex.getAllRules().end(); ++collIter)
    {
      if((*collIter) == &(gameFacade)->skills) // this is a skill
         continue;
      if((*collIter)->size()==0) //collection is empty
         continue;
      isFirst = true;
      bool isReshowing = false;
      KnowledgeIterator iter;
      //if collection is marked for full reshow?
      if(collectionsToReshow_.end() != find(collectionsToReshow_.begin(),collectionsToReshow_.end(),(*collIter)))
            {
              iter = knowledge_.begin();
              isReshowing = true;
            }
      else
            iter = knowledge_.begin() + newKnowledge;
      for(; iter != knowledge_.end(); ++iter)
        {
          if((*iter)->isDescendantFrom((*((*collIter)->begin()))) )
            {
              if(isFirst)
              {
                isFirst = false;
                 string collName = (*collIter)->print();
                 transform (collName.begin() + 1, collName.end(),
                              collName.begin() + 1, (int(*)(int))tolower);

                out << endl<<endl << collName << " knowledge:\n";
              }
              out<<endl<<endl;
              reshow((*iter),out);
            }
        }
      // reshow additional rules marked for reshow;
      if(isReshowing)
            continue;
      for(KnowledgeIterator iter = knowledgeToReshow_.begin();
                        iter != knowledgeToReshow_.end(); ++iter)
      {
//          if((*iter)->isDescendantFrom((*((*collIter)->begin()))) )
        if((*collIter)->isValidTag((*iter)->getTag()))
            {
              if(isFirst)
              {
                isFirst = false;
                 string collName = (*collIter)->print();
                 transform (collName.begin() + 1, collName.end(),
                              collName.begin() + 1, (int(*)(int))tolower);

                out << endl<<endl << collName << " knowledge:\n";
              }
              out<<endl<<endl;
              reshow((*iter),out);
            }
      }
    }
 collectionsToReshow_.clear();
 knowledgeToReshow_.clear();
}



/*
 * Loads all information about knoeledge already known.
 */
void FactionEntity::loadKnowledge(Parser *parser)
{
   	 if (parser->matchKeyword ("SKILL_KNOWLEDGE") )
    		{
          string knowledgeTag =parser->getWord();
   	      SkillRule * skill = gameFacade->skills[knowledgeTag];
          if(skill)
            {
              int level = parser->getInteger();
              skillKnowledge_.push_back(new SkillLevelElement(skill,level));
    		 			return;
            }
            cout << "ERROR: unknown knowledge ["<<knowledgeTag<<"] for faction "<<printTag()<<endl;
    		 			return;
     		}
   	 if (parser->matchKeyword ("KNOWLEDGE") )
    		{
          string knowledgeTag =parser->getWord();
   	      Rule * rule = gameFacade->ruleIndex.findRule(knowledgeTag);
          if(rule)
            {
              knowledge_.push_back(rule);
    		 			return;
            }
            cout << "ERROR: unknown knowledge ["<<knowledgeTag<<"] for faction "<<printTag()<<endl;
     		}
}



bool FactionEntity::checkAnyUnitsLeft()
{
 for (vector< UnitEntity *>::iterator iter = loyalUnits_.begin();
     iter != loyalUnits_.end(); ++iter)
  {
    if(!(*iter)->isDisbanded())
	return true;
  }

 for (vector< ConstructionEntity *>::iterator iter = loyalConstructions_.begin();
     iter != loyalConstructions_.end(); ++iter)
  {
      if((*iter)->oath(0) != SUCCESS) // Everything will be destroyed
         {
          (*iter)->destroy();
         }
  }
  isDisbanded_ = true;
  addReport(new SimpleMessage(resignReporter),0,0); 
  return false;
}


void FactionEntity::resign(FactionEntity * faction)
{
 // transfer all possessions
 for (vector< UnitEntity *>::iterator iter = loyalUnits_.begin();
     iter != loyalUnits_.end(); ++iter)
  {
    if((*iter)->oath(faction) != SUCCESS) // Everything will be destroyed
         {
          (*iter)->disband();
         }
  }

 for (vector< ConstructionEntity *>::iterator iter = loyalConstructions_.begin();
     iter != loyalConstructions_.end(); ++iter)
  {
      if((*iter)->oath(faction) != SUCCESS) // Everything will be destroyed
         {
          (*iter)->destroy();
         }
  }
  isResigned_ = true;
}



bool FactionEntity::isNPCFaction()
{
 return gameFacade->getGameConfig()->isNPCFaction(this);
}



bool FactionEntity::stanceAtLeast(FactionEntity * faction, StanceVariety * stance)
{ 
  return (*(getStance(faction)) >= *stance) ;
}



bool FactionEntity::stanceAtLeast(TokenEntity * token, StanceVariety * stance)
{
  return (*(getStance(token)) >= *stance) ;
}




bool FactionEntity::mayObserveTokenEntity( TokenEntity * tokenEntity, 	                                                      LocationEntity * location)
{
   if(tokenEntity->getFaction() == this ) // the same faction
        return true;
  if(tokenEntity->isExposed())
        return true;
   if (location->getFactionalObservation(this) >= tokenEntity->getStealth() )
        return true;
   else
        return false;

}



void FactionEntity::addToFunds(ItemRule * item, int number)
{
  for (ItemElementIterator iter  = funds_.begin(); iter != funds_.end(); ++iter)
    {
        if((*iter).getItemType() == item)
                {
                 (*iter).setItemNumber((*iter).getItemNumber()+ number);
                      return;
                }
    }
  funds_.push_back(ItemElement(item,number));
}



void FactionEntity::reportFunds(ReportPrinter &out)
{

	if(funds_.size() == 0)
	{
	 out <<  endl << "Faction funds are empty." <<  endl;
	 return;
	}
	out <<  endl << "Faction funds: " <<  endl;
	out.incr_indent();
  for (ItemElementIterator iter  = funds_.begin(); iter != funds_.end(); ++iter)
    {
			if(iter  != funds_.begin())
				out << ", ";
			out << (*iter).print();
    }
		out <<"."<< endl;
	out.decr_indent();
}



void FactionEntity::dailyPreProcess()
{
	if(isUnknownEntity())
	{
		return;
	}
	// Calculate CP
 calculateControlPoints();
 if(controlPoints_  > maxControlPoints_)
 {
 		for (vector< UnitEntity *>::iterator iter = loyalUnits_.begin();
     		iter != loyalUnits_.end(); ++iter)
  		{
				if(Roll_1Dx(maxControlPoints_) > controlPoints_ - maxControlPoints_)
				 (*iter)->setDisobeying(true);
				 // report
  		}

 }

}



void FactionEntity::calculateControlPoints()
{
 controlPoints_ = 0;
 for (vector< UnitEntity *>::iterator iter = loyalUnits_.begin();
     iter != loyalUnits_.end(); ++iter)
  {
    controlPoints_ += (*iter)->getControlPoints();
    //cout<<(*iter)->print()<<" "<<(*iter)->getFiguresNumber()<<" "<<(*iter)->getRace()->print()<<" " <<(*iter)->getControlPoints()<<" CP"<<endl;
  }
}



void FactionEntity::addCombatReport(CombatReport * combatReport)
{
//cout << " === Adding combat report "<<combatReport<< "to "<<print()<<" === "<<endl;
 for (vector< CombatReport *>::iterator iter = combatReports_.begin();
     iter != combatReports_.end(); ++iter)
  {
		if(*iter == combatReport)
			return;
  }
	combatReports_.push_back(combatReport);
}

bool FactionEntity::isLoyalUnit(UnitEntity * unit)
{
 for (vector< UnitEntity *>::iterator iter = loyalUnits_.begin();
     iter != loyalUnits_.end(); ++iter)
  {
     if((*iter)->getTag()==unit->getTag())
     {
         return true;
     }
 }
      return false;
}
bool FactionEntity::isLoyalConstruction(ConstructionEntity * construction)
{
 for (vector< ConstructionEntity *>::iterator iter = loyalConstructions_.begin();
     iter != loyalConstructions_.end(); ++iter)
  {
     if((*iter)->getTag()== construction->getTag())
     {
         return true;
     }
 }
     return false;
}
// This function looks for image of the object provided as a parameter.
// For strings and integers returns corresponding object
// For entities returns image of entity if faction has it. Otherwise returns special built-in unknown entity
// For rules?
// For elements?

AbstractData * FactionEntity::findImage(AbstractData * data)
{
    Entity * entity = dynamic_cast<Entity *> (data);
    if (entity == 0)
    {
        return data;
    }
    	return getTurnReport()->findEntityImage(entity);
}



void     FactionEntity::finalizeTurnEvents()
{
	   // Add global events to report at the end of turn.
	    vector< Event *> events = gameFacade->getGameEntity()->getAllCollectedEvents();
	    for(vector< Event *>::iterator iter = events.begin(); iter != events.end();++iter)
	    {
	       //this->getTurnReport()->addGlobalEvent(*iter);
	       this->getTurnReport()->addEventImage(*iter);
	    }

}
