/***************************************************************************
                          ApplyCombatOrder.cpp
                             -------------------
    begin                : Wed Apr 20 2005
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "ApplyCombatOrder.h"
#include "CombatActionStrategy.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "reporting.h"
extern ReportPattern * combatApplyReporter;


ApplyCombatOrder * instantiateApplyCombatOrder = new ApplyCombatOrder();
//ReportPattern * combatParryReporter= new ReportPattern("combatParryReporter");

ApplyCombatOrder::ApplyCombatOrder(){
  keyword_ = "Apply";
  registerOrder_();
  description = string("APPLY: [skill tag] \n") +
	"The unit attempts to apply the indicated skill, if there is a valid\n" +
  "target within range, and the unit has the required components or\n" +
  "equipment for the effect.  If the skill cannot be used, it is skipped.\n" +
  "If the skill is magic (a spell), and if it was attempted, it is removed\n" +
  "from the combat tactical setting, thus avoiding the casting of the same\n" +
  "spell over and over again.  Once no spells can be attempted on the combat\n" +
  "list, theoriginal combat list is restored anew, and the list examined\n" +
  "again once.\n";
  orderType_   = COMBAT_ACTION_ORDER;
  initiative_ = 0;
 isSequentive_ = true;
  mayInterrupt_ = true;
}

STATUS ApplyCombatOrder::loadParameters(Parser * parser,
                         ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
    if(!parseGameDataParameter(entity,  parser, gameFacade->skills, "skill tag", parameters))
            return IO_ERROR;

	// Set initiative to initiative of CombatAction
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
	SkillRule * skill = dynamic_cast<SkillRule *>(parameters[0]);

	if(skill == 0)
	{
   return OK;
	}

	if(!unit->hasSkill(skill,1))
	{
   return OK;
	}

	int level = unit->getSkillLevel(skill);
	CombatActionStrategy * combatAction = 	skill->getCombatAction(level);
	if(combatAction == 0)
	{
   return OK;
	}

//	/*combatReportFile*/ cout << "Loading APPLY " << skill->printTag()<< " order for " <<unit->printTag();
	initiative_ = combatAction->getInitiative();
//	/*combatReportFile*/ cout << " Initiative " << initiative_<<endl;
	// initiative is relative. That's why nonCumulative initiative stat is not used

   return OK;

}



ORDER_STATUS ApplyCombatOrder::process (Entity * entity, ParameterList &parameters)
{

  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  SkillRule * skill = dynamic_cast<SkillRule *>(parameters[0]);

   if ( skill == 0)
    {
 		return FAILURE;
    }

	combatReportFile<<entity<<" applies "<<skill->print()<<endl;
  if(!unit->hasSkill(skill,1))
  {
      if(entity->isTraced())
      {
	combatReportFile<<entity<<" doesn't know "<<skill->print()<<endl;
      }
	    return FAILURE;
  }

	int level = unit->getSkillLevel(skill);
	CombatActionStrategy * combatAction = 	skill->getCombatAction(level);
        //cout<<entity<<" applies "<<skill->print()<<" "<<level<<" "<<combatAction<<endl;
        //combatAction->debugPrint();
	if(combatAction ==0)
        {
          if(entity->isTraced())
          {
            combatReportFile<<" There is no CombatActionStrategy for "<<skill->print()<<endl;
          }
	    return FAILURE;
        }


    BattleInstance * battleInstance = unit->getBattleInstantiation();
    CombatReport * report = battleInstance->getBattleField()->getCombatEngine()
						->getCombatReport();


        // Check material and mana requirements
       if(!combatAction->mayApplyAction(battleInstance))
        {
            return FAILURE;
        }
        // Consume mana and resources
        combatAction->consumeActionResources(battleInstance);

	battleInstance->setAffectingAction(combatAction);
        battleInstance->recalculateStats(); // After setting AffectingAction
    report->add(new BinaryMessage(combatApplyReporter, battleInstance->getOrigin(), skill));
 	vector <BattleTargetElement> potentialTargets =
	combatAction->getPotentialTargets(battleInstance, report);
	if(potentialTargets.empty()) // No targets.  But already eported
	{
          if(entity->isTraced())
          {
            combatReportFile<<entity<<" finds no targets for "<<skill->print()<<endl;
          }
		return FAILURE;
	}

	combatAction->performAction(battleInstance, potentialTargets, report);

	battleInstance->addActionExperience(skill);

	return SUCCESS;
}

