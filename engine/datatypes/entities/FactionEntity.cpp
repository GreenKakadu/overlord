/***************************************************************************
                          FactionEntity.cpp  -  description
                             -------------------
    begin                : Tue Nov 19 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include <stdio.h>
#include <algorithm>

#include "GameInfo.h"
#include "Symbols.h"
#include "FileParser.h"
#include "FactionEntity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "EntitiesCollection.h"
#include "RulesCollection.h"
#include "ReportElement.h"
#include "ItemElement.h"
#include "ReportRecord.h"
#include "ReportPattern.h"
#include "StanceVariety.h"
#include "ItemRule.h"
#include "RuleIndex.h"
#include "SkillRule.h"
#include "RulesCollection.h"

extern bool ciStringCompare(const string& s1,const string& s2);
extern RulesCollection <SkillRule>     skills;
extern EntitiesCollection <FactionEntity>   factions;
extern EntitiesCollection <UnitEntity>      units;
extern VarietiesCollection    <StanceVariety>      stances;
extern GameInfo game;
extern RuleIndex ruleIndex;

FactionEntity::FactionEntity ( const FactionEntity * prototype ) : Entity(prototype)
{
	defaultStance_  = stances["neutral"];
}

void    FactionEntity::preprocessData()
{
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
		stances_.push_back(new StanceElement(stance,entity));
	  return OK;
	}
       if (parser->matchKeyword("FUNDS"))
	{
			if(parser->matchElement())
			  funds_.push_back(new ItemElement(parser));
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
void
FactionEntity::save(ostream &out)
{
	saveReport();

  out << keyword_ << " " << tag_ << endl;
  if(!name_.empty())          out << "NAME "               << name_            << endl;
  if(!description_.empty()) out << "DESCRIPTION " << description_  << endl;
 if(!email_.empty())             out << "EMAIL "               << email_            << endl;
 if(!password_.empty())      out << "PASSWORD "    << password_      << endl;

  vector<ItemElement *>::iterator fundsIter ;
  for (fundsIter  = funds_.begin(); fundsIter != funds_.end(); fundsIter++)
    {
           out << "FUNDS ";
           (*fundsIter)->save(out);
    }
	

 StanceIterator iter;
  out << "DEFAULIT_STANCE " << defaultStance_ << endl;
	for (iter = stances_.begin(); iter != stances_.end(); iter++)
		{
  out << "STANCE " <<(*iter)->getParameter1() << (*iter)->getRule() << endl;
		}

   saveKnowledge(out);
   
  vector<Order *>::iterator iter2;
  for ( iter2 = orders_.begin(); iter2 != orders_.end(); iter2++)
    {
           (*iter2)->save(out);
    }
  out << endl;

 
}


void FactionEntity::print() // For debugging
{
    cout  << getName();
    cout << " [" << getTag()  << "] "<< endl;

  vector<Order *>::iterator iter;
  for ( iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           (*iter)->save(cout);
    }

}

/** Loads orders */
void FactionEntity::loadOrders()
{
  string orderFlename = game.getOrdersFileName(this);
  FileParser  * parser = new FileParser(orderFlename.c_str());
	if(parser->status != OK)
		return;
	else		
	cout << "Orders file " << orderFlename << " opened" <<endl;
   string currentGame ;	
   UnitEntity * currentUnit;
   	string unitName;
    parser -> getLine() ;
 do // check section before first UNIT keyword
    {
		parser->matchChar('#'); // for compartibility with old Overlord
		 if (parser->matchKeyword ("GAME") )
    		{

   		 		if ( ciStringCompare( tag_, parser->getWord()) )
   		 			{
    		 			cout << "Invalid orders (wrong faction id) for faction " << tag_<< endl;
    		 			delete parser;
    		 			return;
    		 		}
    		 	if ( password_ != parser->getParameter() )   // case sensitive!
    		 		{
    		 			cout << "Invalid orders (wrong faction password) for faction " << tag_<< endl;
    		 			delete parser;
    		 			return;
    		 		}

   		 		currentGame = parser->getWord();
   		 		if( !currentGame.empty() )
   		 			{
   		 				if ( ciStringCompare( game.getGameId(),  currentGame )  ) // take care about case
    		 				{
    		 					cout << "Invalid orders (wrong game id) for faction " << tag_ << endl;
     		 					delete parser;
    		 					return;
    		 			}
    		 		}
       	 parser -> getLine();
    		clearOrders();  // clear current faction orders
      		continue;
    		}
    		
 //   		clearOrders();  // clear current faction orders
   		
    		
 	 if (parser->matchKeyword ("END") )
    		{
    		 			delete parser;
    		 			return;
     		}
   		//-------------------------------------------------------
    		// Read Faction orders:

	if(parser->matchChar(COMMENT_SYMBOL))  // skip  lines starting from comment
	{
		continue;
	}
	else
	{
		string order =  parser->getText();
		if( !order.empty() )
			{
//				cout << "Faction order " << order<<endl;
				orders_.push_back(new Order(order, this));
			}
	}

//  	 if (parser->matchKeyword ("PASSWORD") )
//  	 {
//  	 	password_ = parser->getParameter();
//  	 }		
      parser -> getLine();

    } while  (! ( parser->matchKeyword ("UNIT"))  || parser->eof() );

// ============== UNIT orders	  ====================

	if(parser->eof()  )
		{
   		 				delete parser;
    		 			return;
		}	
   	currentUnit = units[parser->getWord()];   // check
	if(currentUnit != 0)
		{
 			if(  currentUnit -> getFaction() != this )
				{
					cout << "Attempt to give orders to foreign unit " <<  currentUnit -> getTag() << endl;
					currentUnit =0;
   					parser -> getLine();   		
				}
			else
				{
						currentUnit -> clearOrders();
   			   			parser -> getLine();   		
				}
         }
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
	 		if (parser->matchKeyword ("UNIT") )
    			{
					unitName =  parser->getWord();
   					currentUnit = units[unitName];   // check
					if(currentUnit == 0)
							{
						 		cout << "Wrong unit name " << unitName <<endl;
  			   					parser -> getLine();   		
  								continue;
							}
 					if(  currentUnit -> getFaction() != this )
						{
							cout << "Attempt to give orders to foreign unit " <<  currentUnit -> getTag() << endl;
							currentUnit =0;
   			   				parser -> getLine();   		
  							continue;
						}
					else
						{
							currentUnit -> clearOrders();
   			   				parser -> getLine();   		
  							continue;
						}
    			}
		if(currentUnit == 0)
			{
  			   	parser -> getLine();   		
  				continue;
			}
		// Then it is order line
		string order =  parser->getText();
		if( !order.empty() )
			{
//				cout << "Unit order " << order<<endl;
				currentUnit  -> addOrder(new Order(order,currentUnit));
			}
		
  			   parser -> getLine();   		
   } while   (!  parser -> eof() );
	
	
	
		delete parser;

}



/** Add location to the list of locations visited if it is not yet in this list */
void FactionEntity::addVisitedLocation(LocationEntity * location)
{
	if(visitedLocations_.end() == find(visitedLocations_.begin(),  visitedLocations_.end(), location )  )
 	visitedLocations_.push_back(location);
}



/** No descriptions */
void FactionEntity::clearVisitedLocations(){
        visitedLocations_.clear();
}



/** No descriptions */
void FactionEntity::dailyReport()
{
// cout <<"Daily report for faction ["<< tag_ <<"]"<<endl;
  vector< UnitEntity *>::const_iterator unitIterator;
  vector< ReportElement *>::const_iterator reportIterator;
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



/** Adds report to the list if it is different from those, that are already there */
 void FactionEntity::updateReports(ReportElement * report){
 vector< ReportElement *>::iterator reportIterator;
 for ( reportIterator = collectedReports_.begin(); reportIterator != collectedReports_.end(); reportIterator++)
		{
         	if (report->getRecord() == (*reportIterator)->getRecord())
				{
					delete report;
					return;
				}
		}
	 collectedReports_.push_back(report);
}



/** Creates turn report file for faction*/
void FactionEntity::saveReport()
{
   ofstream outfile (game.getReportFileName(this).c_str() );
// Mail header
  outfile << "Subject:"<< game.getGameId()<<" Report for Turn "<< game.turn << endl;
	outfile << "From: "<< game.getServer()<< " ("<<game.getServerName()<< " Game server )"<< endl;
	outfile << "To: " << email_<<"\n";
	outfile << "\n";
// Report
   outfile << game.getGameId() <<" Report for Turn "<< game.turn <<" for "<< printName() << ", "<<email_<<"\n";

   outfile <<  endl << "// Faction stats " <<  endl <<  endl;

   outfile << "// Diplomacy  (Stances)" <<  endl <<  endl;

   outfile <<  endl << "// Global Events " <<  endl <<  endl;

   outfile <<  endl << "// Units " <<  endl <<  endl;

  vector< UnitEntity *>::const_iterator unitIterator;
 for ( unitIterator = loyalUnits_.begin(); unitIterator != loyalUnits_.end(); unitIterator++)
	{
		(*unitIterator) -> report(this, outfile);
	}
   outfile <<  endl << "// Locations " <<  endl <<  endl;

  vector< LocationEntity *>::iterator locIterator;
  vector< ReportElement *>::iterator reportIterator;

	for ( locIterator = visitedLocations_.begin(); locIterator != visitedLocations_.end(); locIterator++)
		{
       (*locIterator) -> report(this, outfile);
        outfile << "Events: \n";
        for ( reportIterator = collectedReports_.begin(); reportIterator != collectedReports_.end(); )
		      {
         	  if (  (*locIterator) == (*reportIterator)->getDestination())
				    {
              (*reportIterator)->print(outfile);
              collectedReports_.erase(reportIterator);
				    }
            else
              reportIterator++;
		      }

    outfile<<endl;
		}
   outfile <<  endl << "// Combat " <<  endl <<  endl;

   outfile <<  endl << "// Knowledge " <<  endl <<  endl;
   reportNewKnowledge(outfile);
   outfile <<  endl << "// Orders Template " <<  endl <<  endl;


 outfile.close();
}



/** No descriptions */
void FactionEntity::dailyUpdate()
{
   dailyReport();
}



/** register unit */
void FactionEntity::addUnit(UnitEntity * unit)
{
	if (unit)
		{
			loyalUnits_.push_back(unit);
//			cout << unit->getName() << " " << unit->printTag() << " registered as loyal unit of faction " << printTag()<<endl;
       unit->setFaction(this);
		}
	else
	{
		cout << "Cant register" << unit->printName() <<" to faction " << printTag()<<endl;
	}
}



/** No descriptions */
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

			loyalUnits_.erase(iter);
}



Entity * FactionEntity::getReportDestination(){
return this;
}



/** No descriptions */
StanceVariety * FactionEntity::getStance(UnitEntity * unit) 
{
 StanceIterator iter;
	for (iter = stances_.begin(); iter != stances_.end(); iter++)
		{
			if ( (*iter)->getParameter1() == unit)
				return (*iter)->getRule();
		}
		return  getStance(unit->getFaction());
}



StanceVariety * FactionEntity::getStance(FactionEntity * faction) 
{
 if  (faction == this)
 {
    return allied;
//    return stances["ally"];
 }
 StanceIterator iter;
	for (iter = stances_.begin(); iter != stances_.end(); iter++)
		{
			if ( (*iter)->getParameter1() == faction)
				return (*iter)->getRule();
		}
		return  defaultStance_;
}



/** No descriptions */
/** No descriptions */
void FactionEntity::setStance(Entity * entity, StanceVariety * stance)
{
 StanceIterator iter;
	for (iter = stances_.begin(); iter != stances_.end(); iter++)
		{
			if ( (*iter)->getParameter1() == entity)
				 (*iter)->setRule(stance);
		}
	stances_.push_back(new StanceElement(stance,entity));
}



/** No descriptions */
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

Rule * FactionEntity::hasKnowledge(Rule * info)
{
    
  KnowledgeIterator  iter = find(knowledge_.begin(), knowledge_.end(),info) ;
  
  if ( iter != knowledge_.end())
    return *iter;
  else
      return 0;
}

SkillLevelElement * FactionEntity::hasSkillKnowledge(SkillRule * knowledge, int level)
{

  for(SkillLevelIterator iter = skillKnowledge_.begin(); iter != skillKnowledge_.end(); ++iter)
    {
      if( (*iter)->getSkill() == knowledge  && (*iter)->getLevel() == level )
        return (*iter);
    }
  return 0;

}

void FactionEntity::reshow(Rule * info, ostream &out)
{
  if(hasKnowledge(info))
    out << hasKnowledge(info)->getDescription();
}

void FactionEntity::reshow(BasicRulesCollection  * collection, ostream &out )
{
  for(RulesIterator iter = collection->begin();iter != collection->end(); ++iter )
    {
      reshow(*iter, out);
    }

}

void FactionEntity::reshowSkills(ostream &out )
{
//  for(RulesIterator iter = skills.begin();iter != skills.end(); ++iter )
  for(unsigned i=0; i < skills.size(); ++i )
  {
    for (int level = 0; level < 7; ++level)
    {
      reshowSkill( skills[i], level,out);
    }
  }
}

void FactionEntity::reshowSkill(SkillRule * knowledge, int level, ostream &out)
{
  if(hasSkillKnowledge(knowledge,level))
    hasSkillKnowledge(knowledge,level)->getSkill()->printDescription(level,out);
}

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

void FactionEntity::reportNewKnowledge(ostream &out)
{
  if(skillKnowledge_.size() > newSkillKnowledge)
  {
    out << "\nSkill knowledge:\n";
    for(SkillLevelIterator iter = skillKnowledge_.begin() + newSkillKnowledge; iter != skillKnowledge_.end(); ++iter)
      {
        out<<endl;
        reshowSkill((*iter)->getSkill(), (*iter)->getLevel(),out);
//        (*iter)->getSkill()->printDescription((*iter)->getLevel(),out);
      }
  }
//    out << "\nRace knowledge:\n";
//    out << "\nItem knowledge:\n";
//  for(KnowledgeIterator iter = knowledge_.begin() + newKnowledge; iter != knowledge_.end(); ++iter)
//    {
//        out<<endl;
//        out<< (*iter)->getDescription();
//    }
   
}

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

