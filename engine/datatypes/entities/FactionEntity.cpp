/***************************************************************************
                          FactionEntity.cpp  -  description
                             -------------------
    begin                : Tue Nov 19 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include <stdio.h>
#include <algorithm>
#include <cctype>

#include "GameInfo.h"
#include "Symbols.h"

#include "FileParser.h"
#include "ReportPrinter.h"

#include "FactionEntity.h"
#include "UnitEntity.h"
#include "PhysicalEntity.h"
#include "LocationEntity.h"
#include "ConstructionEntity.h"

#include "EntitiesCollection.h"
#include "RulesCollection.h"
#include "StanceVariety.h"
#include "ItemRule.h"
#include "RaceRule.h"
#include "RuleIndex.h"
#include "SkillRule.h"
#include "RulesCollection.h"

#include "ReportElement.h"
#include "ItemElement.h"
#include "ReportRecord.h"
#include "ReportPattern.h"

extern GameInfo game;
extern bool ciStringCompare(const string& s1,const string& s2);
//extern StanceVariety * findStanceByTag(const string &tag);
extern VarietiesCollection <StanceVariety>    stances;

FactionEntity::FactionEntity ( const FactionEntity * prototype ) : Entity(prototype)
{
  totalControlPoints_ = 200;
  controlPoints_ = 0;
  terseBattleReport_ = false;
  allSkillsToReshow_ = false;
  isResigned_ = false;
}

void    FactionEntity::preprocessData()
{
	defaultStance_  = neutralStance;
  newKnowledge = knowledge_.size();
  newSkillKnowledge = skillKnowledge_.size();
//  cout <<"Begin: (" <<knowledge_.size()<<") -> " <<newKnowledge <<" ->end: " << (knowledge_.end() - knowledge_.begin())<<endl;
}



GameData * FactionEntity::createInstanceOfSelf()
{
   return CREATE_INSTANCE<FactionEntity> (this);
}


STATUS
FactionEntity::initialize        ( Parser *parser )
{


      if (parser->matchKeyword ("NAME") )
        {
	  setName(parser->getText());
	  return OK;
	}
      if (parser->matchKeyword("DESCRIPTION"))
	{
	  setDescription(parser->getText());
	  return OK;
	}
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
		StanceVariety * defaultStance = stances[parser->getWord()];
			if(defaultStance)
	   		defaultStance_ =  defaultStance;
	  return OK;
	}
       if (parser->matchKeyword("STANCE"))
	{
		Entity * entity = factions[parser->getWord()]; // Extend to all Entities
		if(entity == 0)
			return OK;
		StanceVariety * stance = stances[parser->getWord()];
		if(stance == 0)
				 stance =  defaultStance_;
		stances_.push_back(StanceElement(stance,entity));
	  return OK;
	}
       if (parser->matchKeyword("FUNDS"))
	{
			if(parser->matchElement())
			  funds_.push_back(new ItemElement(parser));
	  return OK;
	}

 if (parser->matchKeyword("TERSE"))
	{
    terseBattleReport_ = true;
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
      if (parser->matchKeyword("ORDER"))
 	{
	  //          cout << "Reading order..." << endl;
	  orders_.push_back(new Order(parser->getText(),this));
 	}

    loadKnowledge(parser);

    return OK;

}


/*
 * Save data after processing
 */
void FactionEntity::save(ostream &out)
{
	saveReport();
  if(isResigned_)
    return; 
    
  out << keyword_ << " " << tag_ << endl;
  if(!name_.empty())          out << "NAME "               << name_            << endl;
  if(!description_.empty()) out << "DESCRIPTION " << description_  << endl;
 if(!email_.empty())             out << "EMAIL "               << email_            << endl;
 if(!password_.empty())      out << "PASSWORD "    << password_      << endl;
  if(terseBattleReport_) out << "TERSE" << endl;
  
  vector<ItemElement *>::iterator fundsIter ;
  for (fundsIter  = funds_.begin(); fundsIter != funds_.end(); ++fundsIter)
    {
           out << "FUNDS ";
           (*fundsIter)->save(out);
    }
	

  out << "DEFAULIT_STANCE " << defaultStance_ << endl;
	for (StanceIterator iter = stances_.begin(); iter != stances_.end(); ++iter)
		{
  out << "STANCE " <<(*iter).getParameter1() << (*iter).getRule() << endl;
		}

   saveKnowledge(out);
   
  for (vector<Order *>::iterator  iter = orders_.begin(); iter != orders_.end(); ++iter)
    {
           (*iter)->save(out);
    }
  out << endl;

 
}



/*
 * Load all orders from the input
 */
void FactionEntity::loadOrders()
{
  string orderFlename = game.getOrdersFileName(this);
  FileParser  * parser = new FileParser(orderFlename.c_str());
	if(parser->status != OK)
		return;
	else		
	cout << "Orders file " << orderFlename << " opened" <<endl;
   PhysicalEntity * currentEntity;
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

          currentEntity = currentEntityOrders(units, parser);
          continue;
        }

      if(parser->matchKeyword ("CONSTRUCTION"))
        {
          currentEntity =  currentEntityOrders(buildingsAndShips, parser);
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
//    				cout << "Entity order " << order<<endl;
				currentEntity  -> addOrder(new Order(order,currentEntity));
			}
		
  			   parser -> getLine();   		
 } while   (!  parser -> eof() );
	
	
	
		delete parser;

}

/*
 * Load faction orders from the input
 */
STATUS  FactionEntity::loadFactionOrders(Parser * parser, PhysicalEntity ** entity)
{
 string currentGame ;
 string order;
 while(true) // check section before first UNIT or CONSTRUCTION keyword
  {
		  parser->matchChar('#'); // for compartibility with old Overlord
		  if (parser->matchKeyword ("GAME") )
    		{

   		 		if ( ciStringCompare( tag_, parser->getWord()) )
   		 			{
    		 			cout << "Invalid orders (wrong faction id) for faction " << tag_<< endl;
    		 			delete parser;
    		 			return IO_ERROR;
    		 		}
    		 	if ( password_ != parser->getParameter() )   // case sensitive!
    		 		{
    		 			cout << "Invalid orders (wrong faction password) for faction " << tag_<< endl;
    		 			delete parser;
    		 			return IO_ERROR;
    		 		}

   		 		currentGame = parser->getWord();
   		 		if( !currentGame.empty() )
   		 			{
   		 				if ( ciStringCompare( game.getGameId(),  currentGame )  ) // take care about case
    		 				{
    		 					cout << "Invalid orders (wrong game id) for faction " << tag_ << endl;
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

  if(parser->matchKeyword ("UNIT"))
    {
      *entity = currentEntityOrders(units, parser);
      return OK;
    }

  if(parser->matchKeyword ("CONSTRUCTION"))
    {
      *entity = currentEntityOrders(buildingsAndShips, parser);
      return OK;
    }
  //-------------------------------------------------------
    		// Read Faction orders:

	if(parser->matchChar(COMMENT_SYMBOL))  // skip  lines starting from comment
	  {
		  continue;
	  }
	if(parser->matchChar(';'))  // overlord comment compartibility
	  {
			continue;
	  }
	else
	  {
		 order =  parser->getText();
		  if( !order.empty() )
			  {
//          				cout << "Faction order " << order<<endl;
				  orders_.push_back(new Order(order, this));
			  }
	  }

  parser -> getLine();

 } 
}



/*
 * Load  from the input orders for one entity
 */
PhysicalEntity * FactionEntity::currentEntityOrders(BasicEntitiesCollection & collection,
                                  Parser * parser)
{    
  string entityName =  parser->getWord();
  GameData * current = collection.findByTag(entityName);
  if(current == 0)
		{
		  cout << "Wrong entity name " << entityName <<endl;
  		parser -> getLine();
  		return 0;
		}
  
  PhysicalEntity * currentEntity = dynamic_cast<PhysicalEntity *>(current);
  if(currentEntity == 0)
		{
		  cout << "Wrong entity name " << entityName <<endl;
  		parser -> getLine();
  		return 0;
		}

  if(  currentEntity -> getFaction() != this )
		{
		  cout << "Attempt to give orders to foreign entity " <<  currentEntity -> getTag() << endl;
		  currentEntity =0;
   	  parser -> getLine();
  		return 0;
		}
  else
		{
//      cout << "Entity "<< currentEntity->printName()<<endl;
		  currentEntity -> clearOrders();
   	  parser -> getLine();
  		return currentEntity;
		}

}



/*
 * Add location to the list of locations visited
 * if it is not yet in this list
 */
void FactionEntity::addVisitedLocation(LocationEntity * location)
{
	if(visitedLocations_.end() == find(visitedLocations_.begin(),  visitedLocations_.end(), location )  )
 	visitedLocations_.push_back(location);
}



/*
 * Delete all entries from the list of visited locations
 */
void FactionEntity::clearVisitedLocations(){
        visitedLocations_.clear();
}



/*
 * Process all events reported by entities and slect those,
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

ReportPrinter outfile;

/*
 * Creates turn report file for faction
 */
void FactionEntity::saveReport()
{
//   ReportPrinter outfile (game.getReportFileName(this));
    outfile.open(game.getReportFileName(this));
// Mail header
  outfile << "Subject:"<< game.getGameId()<<" Report for Turn "<< game.turn << endl;
	outfile << "From: "<< game.getServer()<< " ("<<game.getServerName()<< " Game server )"<< endl;
	outfile << "To: " << email_<<"\n";
	outfile << "\n";
// Report
   outfile << game.getGameId() <<" Report for Turn "<< game.turn <<" for "<< printName() << ", "<<email_<<"\n";
   if(isResigned_)
      {
       outfile<< "---- Resigned. ----"<<endl;
       outfile <<"END"<<endl;
       outfile.close();
       return;
      }
   outfile <<  endl << "// Faction stats " <<  endl <<  endl;
   outfile << "// Diplomacy  (Stances)" <<  endl <<  endl;
   outfile <<  "Default attitude is " <<defaultStance_->getName()<<  endl;
     if(!stances_.empty())
   {
//   outfile <<  "Attitude to " <<" is "<<defaultStance_->getName()<<  endl;

    for(StanceIterator iter = stances_.begin(); iter != stances_.end();++iter)
      {
   outfile <<  "Attitude to " <<(*iter).getParameter1()->printName()<<" is "<<(*iter).getRule()->getName() <<  endl;

      }
   }
//--------------

   outfile <<  endl << "// Global Events " <<  endl <<  endl;
   for (ReportIterator reportIterator = collectedReports_.begin(); reportIterator != collectedReports_.end(); /*reportIterator++*/)
		 {
      if (  this == (*reportIterator)->getDestination())
				    {
               (*reportIterator)->print(outfile);
              collectedReports_.erase(reportIterator);
				    }
            else
              reportIterator++;
		 }
 
   outfile <<  endl << "// Units " <<  endl <<  endl;

 for ( vector< UnitEntity *>::iterator unitIterator = loyalUnits_.begin(); unitIterator != loyalUnits_.end(); unitIterator++)
	{
 		(*unitIterator) -> report(this, outfile);
	}

   outfile <<  endl << "// Constructions " <<  endl <<  endl;
 for ( vector< ConstructionEntity *>::iterator constructionIterator = loyalConstructions_.begin(); constructionIterator != loyalConstructions_.end(); constructionIterator++)
	{
 		(*constructionIterator) -> report(this, outfile);
	}

   outfile <<  endl << "// Locations " <<  endl <<  endl;

  vector< LocationEntity *>::iterator locIterator;
//  ReportIterator reportIterator;
  bool eventsReported = false;
	for ( locIterator = visitedLocations_.begin(); locIterator != visitedLocations_.end(); locIterator++)
		{
        (*locIterator) -> report(this, outfile);
        outfile << "\nEvents during turn: \n";
        eventsReported = false;
        for (ReportIterator reportIterator = collectedReports_.begin(); reportIterator != collectedReports_.end(); )
		      {
         	  if (  (*locIterator) == (*reportIterator)->getDestination())
				    {
               (*reportIterator)->print(outfile);
              collectedReports_.erase(reportIterator);
              eventsReported = true;
				    }
            else
              reportIterator++;
		      }
        if(!eventsReported)
          outfile << "   - Nothing notable\n";
    outfile<<endl;
		}
   outfile <<  endl << "// Combat " <<  endl <<  endl;

   outfile <<  endl << "// Knowledge " <<  endl <<  endl;
   
   reportNewKnowledge(outfile);
   
   outfile <<  endl <<  endl << "#============== Orders Template ==========" <<  endl <<  endl;

  outfile <<"GAME "<< getTag()<< " "<<password_ << " " << game.getGameId() <<  endl <<  endl;

 for ( vector< UnitEntity *>::iterator unitIterator = loyalUnits_.begin(); unitIterator != loyalUnits_.end(); unitIterator++)
	{
 		(*unitIterator) -> printOrderTemplate(outfile);
	}

   outfile <<  endl << "// Constructions " <<  endl <<  endl;
 for ( vector< ConstructionEntity *>::iterator constructionIterator = loyalConstructions_.begin(); constructionIterator != loyalConstructions_.end(); constructionIterator++)
	{
 		(*constructionIterator) -> printOrderTemplate(outfile);
	}
  outfile <<"END"<<endl;
  outfile.close();
}



/*
 * Daily post-processing
 */
void FactionEntity::dailyUpdate()
{
   dailyReport();
}



/*
 * Monthly post-processing
 */
void FactionEntity::postProcessData()
{
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
       controlPoints_ += unit->getControlPoints();
       // if has title add title CP
       // if has landTitle - change land ownership
       // controlled monsters?
		}
	else
	{
		cout << "Cant register" << unit->printName() <<" to faction " << printTag()<<endl;
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
		  cout << "Cant remove" << unit->printName() <<" from faction " << printTag()<<endl;
       return;
    }
    
 vector< UnitEntity *>::iterator iter = find (loyalUnits_.begin(), loyalUnits_.end(), unit);
	if (iter == loyalUnits_.end())
		{
		  cout << "Cant find" << unit->printName() <<" in faction " << printTag()<<endl;
       return;
    }
       controlPoints_ -= unit->getControlPoints();
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
		cout << "Cant register" << construction->printName() <<" to faction " << printTag()<<endl;
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
		  cout << "Cant remove" << construction->printName() <<" from faction " << printTag()<<endl;
       return;
    }

 vector< ConstructionEntity *>::iterator iter = find (loyalConstructions_.begin(), loyalConstructions_.end(), construction);
	if (iter == loyalConstructions_.end())
		{
		  cout << "Cant find" << construction->printName() <<" in faction " << printTag()<<endl;
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
StanceVariety * FactionEntity::getStance(UnitEntity * unit) 
{
	for (StanceIterator iter = stances_.begin(); iter != stances_.end(); iter++)
		{
			if ( (*iter).getParameter1() == unit)
				return (*iter).getRule();
		}
    // Check that we can determine unit's owner
    return  getStance(unit->getFaction());
    // otherwise default
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
    return alliedStance;
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
   vector<ItemElement *>::iterator fundsIter ;
  for (fundsIter  = funds_.begin(); fundsIter != funds_.end(); fundsIter++)
    {
           if((*fundsIter)->getItemType() == item)
              {
                num = (*fundsIter)->getItemNumber();
                if(num >= number)
                  {
                   (*fundsIter)->setItemNumber(num - number);
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
   vector<ItemElement *>::iterator fundsIter ;
  for (fundsIter  = funds_.begin(); fundsIter != funds_.end(); fundsIter++)
    {
      if((*fundsIter)->getItemType() == item)
        {
          if(number <= (*fundsIter)->getItemNumber())
            return true;
          else
            return false;
        }        
    }
    return false;
}


//===============================  Knowledge ============
/** No descriptions */
bool FactionEntity::addKnowledge(Rule * info)
{
  if(info == 0)
      return false;

  if(!hasKnowledge(info))
  {
    knowledge_.push_back(info);
    info->extractKnowledge(this);
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
    return true;
  }
  else
      return false;
}



/** No descriptions */
Rule * FactionEntity::hasKnowledge(Rule * info)
{
    
  KnowledgeIterator  iter = find(knowledge_.begin(), knowledge_.end(),info) ;
  
  if ( iter != knowledge_.end())
    return *iter;
  else
      return 0;
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



/** No descriptions */
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
  for(int i=0; i < skills.size(); ++i )
  {
    for (int level = 0; level < SkillRule::getMaxSkillLevel(); ++level)
    {
      reshowSkill( skills[i], level,out);
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
  if(collection->getCollectionKeyword()  == skills.getCollectionKeyword()) 
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
  for(vector < BasicRulesCollection  *>::iterator collIter = ruleIndex.getAllRules().begin();
          collIter != ruleIndex.getAllRules().end(); ++collIter)
    {
      if((*collIter) == &skills) // this is a skill
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
                 string collName = (*collIter)->printName();
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
          if((*iter)->isDescendantFrom((*((*collIter)->begin()))) )
            {
              if(isFirst)
              {
                isFirst = false;
                 string collName = (*collIter)->printName();
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



/** No descriptions */
void FactionEntity::loadKnowledge(Parser *parser)
{
   	 if (parser->matchKeyword ("SKILL_KNOWLEDGE") )
    		{
          string knowledgeTag =parser->getWord();
   	      SkillRule * skill = skills[knowledgeTag];
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
   	      Rule * rule = ruleIndex.findRule(knowledgeTag);
          if(rule)
            {
              knowledge_.push_back(rule);
    		 			return;
            }
            cout << "ERROR: unknown knowledge ["<<knowledgeTag<<"] for faction "<<printTag()<<endl;
     		}
}



void FactionEntity::resign(FactionEntity * faction)
{
 // transfer all posessions
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
 return game.isNPCFaction(this);
}
  
