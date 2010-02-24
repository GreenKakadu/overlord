/***************************************************************************
                             CombatOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "CombatOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
#include "OrderLine.h"
extern bool ciStringCompare(const std::string& s1,const std::string& s2);
extern EntitiesCollection <UnitEntity>      units;
extern ReportPattern *	invalidParameterReporter;
extern ReportPattern *	missingParameterReporter;
extern ReportPattern *	AtReporter;
extern ReportPattern *	combatSettingsReporter;
ReportPattern * noCombatUseReporter = new ReportPattern("noCombatUseReporter");
ReportPattern * noCombatUse2Reporter = new ReportPattern("noCombatUse2Reporter");

CombatOrder  * instantiateCombatOrder = new CombatOrder();

CombatOrder::CombatOrder(){
  keyword_ = "Combat";
  registerOrder_();
  description = string("COMBAT actions \n") +
  "Immediate, one-shot.  Executes immediately, and sets the list of\n" +
	"combat actions during the battle to the list given.  \n" +
  "You may specify as combat actions:\n" +
  "- MELEE, to attack in hand-to-hand combat;\n" +
  "- PARRY, to defend against melee;\n" +
  "- skill-tag, to use the requisite skill or magic spell in combat;\n" +
  "- item-tag, to use the requisite item in combat.\n" +
  "\n" +
  "Your combat list ends up with an implicit \"MELEE PARRY\".\n";

  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS CombatOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;


 UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
	Parser * tempParser;
//	GameData * rule;
//	ItemRule * item;
//	SkillRule * skill;
	 for (int i = 0; i <4; ++i)
	 {
   	string combatOrderText = parser->getWord();// this includes all
	                                            // non-space-separated
	                                            // and may include prefixes.
	 	tempParser = new Parser(combatOrderText);
   	OrderLine::stripModifiers(tempParser);
	 	string keyword = tempParser->getWord(); // now without prefixes
   	if (keyword.size() == 0)
        {
         break;
        }
// if(unit->isTraced())
// {
// cout << "...";
// }
		parameters.push_back( new StringData (combatOrderText));
		checkCombatAction(keyword,unit);

		delete tempParser;
	}
  return OK;

}



ORDER_STATUS CombatOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

// if(unit->isTraced())
// {
//   cout << "COMBAT " <<entity->print()<< ": ";
  // 
// 	for (vector <AbstractData *>::iterator iter = parameters.begin();
// 			iter != parameters.end(); ++iter)
// 			{
// 				cout << (*iter)->print() << "; ";
// 			}
// 	cout << endl;
// }

  Parser * tempParser;
  string combatOrderText;
  unit->clearCombatSettings();
  for (vector <AbstractData *>::iterator iter = parameters.begin();
       iter != parameters.end(); ++iter)
  {
				// extract keyword
    combatOrderText =(*iter)->print();
    tempParser = new Parser( combatOrderText);
    OrderLine::stripModifiers(tempParser);
    string keyword = tempParser->getWord(); // now without prefixes
    if (keyword.size() == 0)
    {
      break;
    }
    unit->addCombatSetting(combatOrderText);	// create order
//				checkCombatAction(keyword,unit);// check validity
    delete tempParser;
  }
 // cout<<"Combat settings "<< unit->reportCombatSettings()<<endl;
    unit->reportCombatSettings();
    unit->addReport(new UnaryMessage(combatSettingsReporter,new StringData(unit->reportCombatSettings())));

  return SUCCESS;
}



bool CombatOrder::checkCombatAction(string & keyword, TokenEntity * entity, bool noReport)
{
	GameData * rule;
	ItemRule * item;
	SkillRule * skill;
// if(entity->isTraced())
// {
// //cout << "checkCombatAction ==== Keyword: "<<keyword<<endl;
// }
  if (!ciStringCompare(keyword,"melee"))
    {
      //cout << " ====     identified as Melee: "<<endl;
      return true;
    }
  if (!ciStringCompare(keyword,"parry"))
    {
    //cout << " ====     identified as Parry: "<<endl;
      return true;
    }
  if (!ciStringCompare(keyword,"reload"))
    {
    //cout << " ====     identified as Reload: "<<endl;
            return true;
    }
   if (!ciStringCompare(keyword,"ranged"))
    {
    //cout << " ====     identified as Ranged: "<<endl;
      return true;
    }

    rule = items.findByTag(keyword,false);

	if(rule) // <-------------- Item 
	{
		 item = dynamic_cast<ItemRule*>(rule);
// You may issue order without having item yet!
//				if(!entity->hasItem(item))// check that unit has this item
//				{
//				entity->addReport(new BinaryMessage(noCombatUseReporter,entity,
//                      new StringData(keyword)));
//							return false;
//				}

//cout << " ====     identified as valid Rule: "<<endl;
		if(!item->getCombatAction())// check that this item
								// may be used in combat
		{
                  if(!noReport) entity->addReport(new BinaryMessage(noCombatUseReporter,entity, new StringData(keyword)));
			return false;
		}
	
	// At the present moment only units are allowed to use items
	
	
	
	// insert CombatUse into the beginning of the keyword string
		keyword.insert(0, "CombatUse ");
		return true;

	}
	else// <-------------- Skill
	{
		rule= skills.findByTag(keyword,false);
		if(rule)
			{
			skill = dynamic_cast<SkillRule*>(rule);
			int level = entity->getSkillLevel(skill);
			if(level == 0)// check that unit has this skill
				{
                                  if(!noReport) entity->addReport(new BinaryMessage(noCombatUseReporter,entity,
						new StringData(keyword)));
				return false;
				}
			if(!skill->getCombatAction(level))// check that this skill may be
								// used in combat
			{
                          if(!noReport) entity->addReport(new BinaryMessage(noCombatUseReporter,entity,
				new StringData(keyword)));
				return false;
				}
// May be entity has this skill but not allowed to use it in combat
// (For instance building may be allowed use skills bit not in combat)
			if(!skill->mayBeUsedBy(entity))
			{
				entity->addReport(new BinaryMessage(noCombatUse2Reporter,entity,
				new StringData(keyword)));
				return false;
				}

//cout << " ====     identified as valid skill: "<<endl;
// insert Apply  into the beginning of the keyword string
			keyword.insert(0, "Apply ");
			return true;
			}
		else // not-existing
			{
                          if(!noReport) entity->addReport(new BinaryMessage(noCombatUseReporter,entity,
				new StringData(keyword)));
			return false;
			}
	}
	return false;
}
