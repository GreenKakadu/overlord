/***************************************************************************
                          SkillRule.cpp  -  description
                             -------------------
    begin                : Tue Dec 10 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "SkillRule.h"
#include "SkillElement.h"
#include "SkillLevelElement.h"
#include "TokenEntity.h"
#include "BasicLearningStrategy.h"
#include "PrototypeManager.h"
#include "ReportPattern.h"
#include "UnaryMessage.h"
#include "CombatActionStrategy.h"
#include "SkillBonusComboAttribute.h"
extern string longtostr(long u);
extern ReportPattern * unusableSkillReporter;
const int SkillRule::maxSkillLevel = 7;
SkillRule     sampleSkill     ("SKILL",    &sampleGameData);
RulesCollection <SkillRule>      skills(new DataStorageHandler("skills.rules"));

SkillRule::SkillRule ( const SkillRule * prototype ) : Rule(prototype)
{
   currentLevel_ = 0;
   max_ = 0;
   learningParadigm_.resize(maxSkillLevel+1);
   usingParadigm_.resize(maxSkillLevel+1);
   learningParadigm_[0] = new BasicLearningStrategy(sampleLearning);
 //  requirement_[0] = 0;
   usingParadigm_[0] = new BasicUsingStrategy(sampleUsing);
//   resources_.resize(maxSkillLevel+1);
   studyCost_.resize(maxSkillLevel+1);
   stats_.resize(maxSkillLevel+1);
   description_.resize(maxSkillLevel+1);
   capacity_.resize(maxSkillLevel+1);
	 expPoints_.resize(maxSkillLevel+1);
   requirement_.resize(maxSkillLevel+1);
   derivatives_.resize(maxSkillLevel+1);
	 combatAction_.resize(maxSkillLevel+1);
	 combatAction_[0] = 0;
     isCombat_ = false;
     isMagic_ = false;
     targetType_ = 0;
		 skillBonuses_ = new SkillBonusComboAttribute();
}



void SkillRule::initLevel_ (int level)
{
  if (level < 1)
      return;
//   resources_[level] = resources_[level - 1];
//  if(learningParadigm_[level] == 0)
   learningParadigm_[level] = learningParadigm_ [level - 1];
//  if(usingParadigm_[level] == 0)
   usingParadigm_[level] = usingParadigm_ [level - 1];
//  if(studyCost_[level] == 0)
   studyCost_[level] = studyCost_[level - 1];
//  if(stats_[level] == 0)
   stats_[level] = stats_[level - 1];
//  if(description_[level].empty())
   description_[level] = description_[level - 1];
//  if(capacity_[level].size() == 0)
   capacity_[level] = capacity_[level - 1];
	 combatAction_[level] = combatAction_[level - 1];

}



STATUS SkillRule::dataConsistencyCheck()
{
  postInit();
  return OK;
}



/** Fills non-initialized level values with those from the previous level */
void SkillRule::postInit()
{
int level;
  for(level = 1; level<= maxSkillLevel; ++level)
  {
     initLevel_(level);
  }
}



GameData * SkillRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<SkillRule> (this);
}


STATUS
SkillRule::initialize        ( Parser *parser )
{

  if (parser->matchKeyword ("NAME") )
    {
      setName(parser->getText());
      return OK;
    }
  if (parser->matchKeyword ("POINTS") )
    {
      expPoints_[1] = parser->getInteger();
     initLevel_(1);
      return OK;
    }
   if (parser->matchKeyword ("COMBAT_SKILL") )
    {
      isCombat_ = true;
      return OK;
    }
   if (parser->matchKeyword ("MAGIC_SKILL") )
    {
      isMagic_ = true;
      return OK;
    }
 if (parser->matchKeyword ("LEVEL") )
    {
			currentLevel_ ++;
      initLevel_(currentLevel_);
			if  (currentLevel_ > maxSkillLevel)
				{
					cout << "Error : for "<<print() <<" Skill Level is higher than maximum\n";
          currentLevel_ = maxSkillLevel;
					return OK;
				}
      expPoints_[currentLevel_] = parser->getInteger();
//    if( this == skills["mane"] )
//    cout <<printTag()<< " Mane[" << currentLevel_ <<"] = "<<expPoints_[currentLevel_]<<endl;
      return OK;
    }
  if (parser->matchKeyword("DESCRIPTION"))
    {
      description_[currentLevel_] = parser->getText();
      return OK;
    }
  if (parser->matchKeyword ("REQUIRES") )
    {
//        int level;
//        SkillRule * skill = skills[parser->getWord()];
//       if( skill == 0  )
//          return OK;
//       if( !parser->matchInteger())
//          level = 1; // default
//       else
//           level = parser->getInteger();
//        SkillElement * parentSkill = new SkillElement(skill, )
//
//        return new SkillElement(skill, expPoints);

        SkillLevelElement * parentSkill  = SkillLevelElement::readElement(parser);
      requirement_[currentLevel_] = parentSkill;
      if(parentSkill )
          parentSkill->getSkill()->addDerivative(new SkillLevelElement(this,currentLevel_+1),parentSkill->getLevel());
      return OK;
    }
  if (parser->matchKeyword("CAPACITY"))
    {
      if(parser->matchInteger())
				{
					int index =  parser->getInteger();
					(capacity_[currentLevel_])[index]  = parser->getInteger();
				}
			else
					{
						string modeTag = parser->getWord();
						if(movementModes.isValidTag(modeTag))
							{
								(capacity_[currentLevel_])[modeTag]  = parser->getInteger();
							}
					}
      return OK;
    }
  if (parser->matchKeyword ("STUDY_COST") )
    {
      studyCost_[currentLevel_] = parser->getInteger();
      return OK;
    }

	if (parser->matchKeyword ("LEARNING_PARADIGM") )
    {
        string keyword = parser->getWord();
       	GameData * temp =  prototypeManager->findInRegistry(keyword);
			if(temp == 0)
				{
					cout << "Unknown learning paradigm " << keyword  << " for skill " << print()<< endl;
				}
			else
				{
  				learningParadigm_[currentLevel_] =
                  dynamic_cast<BasicLearningStrategy *>(temp ->createInstanceOfSelf ());
        }
      return OK;
    }

  if (parser->matchKeyword ("USING_PARADIGM") )
    {
        string keyword = parser->getWord();
       	GameData * temp =  prototypeManager->findInRegistry(keyword);
			if(temp == 0)
				{
					cout << "Unknown using paradigm " << keyword  << " for skill " << print()<< endl;
				}
			else
				{
  				usingParadigm_[currentLevel_] =
                  dynamic_cast<BasicUsingStrategy *>(temp ->createInstanceOfSelf ());
//          cout << print()<<" at level "<<currentLevel_ << " using keyword "<<usingParadigm_[currentLevel_]->getKeyword()<<endl;
        }
      return OK;
    }

  if (parser->matchKeyword ("TARGET") )
    {
       targetType_ = createByKeyword(parser->getWord());
      return OK;
    }

  if (parser->matchKeyword ("COMBAT_ACTION") )
    {
        string keyword = parser->getWord();
       	GameData * temp =  prototypeManager->findInRegistry(keyword);
			if(temp == 0)
				{
					cout << "Unknown combat action " << keyword  << " for skill "
						<< print()<< endl;
				}
			else
				{
  				combatAction_[currentLevel_] =
          	dynamic_cast<CombatActionStrategy *>(temp ->createInstanceOfSelf ());

        }
      return OK;
    }
	if (parser->matchKeyword("COMBAT"))
    {
			if(combatAction_[currentLevel_] == 0)
			{
					cout << "combat parameter COMBAT "<< parser->getText()<< " defined before combat action  for skill " << print()<< endl;

      return OK;
			}

			combatAction_[currentLevel_]->initialize(parser);

      return OK;
    }
//  if (parser->matchKeyword ("") )
//    {
//       = parser->getInteger();
//      return OK;
//    }
     if(stats_[currentLevel_].initialize(parser)!= OK)
        cout << "Error on initialization of stats modifiers for "<<print()
						 <<" ("<< currentLevel_<<")"<<endl;
     if(learningParadigm_[currentLevel_]->initialize(parser) != OK)
        cout << "Error on initialization of learning Paradigm for "<<print()
						 <<" ("<< currentLevel_<<")"<<endl;
     if(usingParadigm_[currentLevel_]->initialize(parser)!= OK)
        cout << "Error on initialization of using Paradigm for "<<print()
						 <<" ("<< currentLevel_<<")"<<endl;
		 if(combatAction_[currentLevel_])
		 {
		 	if(combatAction_[currentLevel_]->initialize(parser)!= OK)
        cout << "Error on initialization of combat action for "<<print()
						 <<" ("<< currentLevel_<<")"<<endl;
		 }

		skillBonuses_->initialize(parser);
		movementBonuses_.initialize(parser);

			return OK;

}



SkillElement * SkillRule::getMax()
{
  if (max_ == 0)
   max_ = new SkillElement (this, expPoints_[currentLevel_]);
  return max_;
}



LEARNING_RESULT SkillRule::mayBeStudied(TokenEntity * tokenEntity)
{
  int nextLevel =  tokenEntity->getSkillLevel(this);
//  if(nextLevel >6)
//    cout << tokenEntity << " has " << tokenEntity->getSkillPoints(this)<< " of " <<print()<<endl;
  return learningParadigm_[nextLevel]->mayStudy(tokenEntity, this);
}



bool SkillRule::teacherRequired(TokenEntity * tokenEntity)
{
  int nextLevel =  tokenEntity->getSkillLevel(this);
  return learningParadigm_[nextLevel]->teacherRequired(tokenEntity, this);
}



int  SkillRule::calculateLearningExperience(TokenEntity * tokenEntity, TeachingOffer * teacher = 0)
{
  int nextLevel =  tokenEntity->getSkillLevel(this);
  return learningParadigm_[nextLevel]->calculateLearningExperience(tokenEntity, this, teacher);
}



int  SkillRule::calculateUsingExperience(TokenEntity * tokenEntity)
{
  int nextLevel =  tokenEntity->getSkillLevel(this);
  return usingParadigm_[nextLevel]->calculateUsingExperience(tokenEntity, this);
}


void SkillRule::addLearningExperience(TokenEntity * tokenEntity, int exp)
{
  int nextLevel = 0;
  SkillElement experience(this,exp);
  learningParadigm_[nextLevel]->addLearningExperience(tokenEntity,experience);
}



void SkillRule::addUsingExperience(TokenEntity * tokenEntity, int exp)
{
  int nextLevel = 0;
  SkillElement experience(this,exp);
  usingParadigm_[nextLevel]->addUsingExperience(tokenEntity,experience);
}

//void SkillRule::addRecursiveExperience(UnitEntity * tokenEntity, SkillElement & skill)
//{
//  learningParadigm_[nextLevel]->addRecursiveLearningExperience(tokenEntity,skill);
//}



int SkillRule::getLevel(int expPoints)
{
  vector<int>::iterator iter;
  int level = 0;
  for(iter = expPoints_.begin(); iter != expPoints_.end(); iter++)
    {
//    if( this == skills["rela"] )
//    cout <<printTag()<<" comparing current points " <<expPoints<<" with Exp level " << (*iter) <<endl;

      if (expPoints < (*iter) )
          break;
      if( (*iter) == 0 && (level >1))
          break;
      else
         level++;

    }
    if(level < 1)
      return 0;
    return level - 1; // in current implementation expPoints[0] is 0 for all skills
}



/*
 * Determines if current skill is in the tree growing from the given skill
 */
bool SkillRule::isDescendFrom(SkillRule * root, int level)
{
	if(root == 0)
    return false;

	SkillRule * current;
	SkillLevelElement * requirement = 0;
  int tryLevel;
  for (tryLevel = 0; tryLevel< level; tryLevel++)
  {
    for(current = this; current != 0;  )
      {
				requirement = current ->getRequirement(tryLevel);
				if(requirement == 0)
					break;
				current = requirement->getSkill();
				if (current == root )
          return true;
      }
  }
    return false;
}



SkillLevelElement * SkillRule::getRequirement(int level) const
{
  // If different requirement skills for different levels are supported
  // all of them should be tried
//  cout << "Requirement for " <<printTag()<<" on level" <<level<< " is ";
//  if(requirement_[level])
//      cout << (requirement_[level])->getSkill()->printTag()<<endl;
//  else
//    cout << "no requirement"<<endl;
  return  requirement_[level];

}



// Registers skillElement "skill" as a skill derived from current skill on level "level"
void SkillRule::addDerivative(SkillLevelElement * skill, int level)
{

  derivatives_[level].push_back(skill);
}


int SkillRule::getStudyCost(TokenEntity * const tokenEntity)
{
  return  studyCost_[tokenEntity->getSkillLevel(this)];
}



int SkillRule::getMaxLevel()
{
  return getLevel(getMax()->getExpPoints());
}



/** No descriptions */
int SkillRule::getLevelExperience(int level) const{
  return expPoints_[level];
}



void SkillRule::getDerivatives (vector <SkillLevelElement *> & derivatives, int level)
{
  vector <SkillLevelElement *>::iterator iter;
  vector <SkillLevelElement *>::iterator iter2;
  bool alreadyThere;
  for(iter = derivatives_[level].begin(); iter != derivatives_[level].end(); iter++)
  {
  alreadyThere = false;
    for(iter2 = derivatives.begin(); iter2 != derivatives.end(); iter2++)
    {
     if ((*iter) == (*iter2))
        alreadyThere = true;
//    if(find(derivatives.begin(),derivatives.end(),(*iter)) != derivatives.end())
    }
    if(!alreadyThere)
     derivatives.push_back(*iter);
  }

}



void SkillRule::printLevel(int level, ostream & out)
{
   switch (level)
   {
     case 0:
      out<< "novice ";
      break;
     case 1:
      out<< level<< "st ";
      break;
     case 2:
      out<< level<< "nd ";
      break;
     case 3:
      out<< level<< "rd ";
      break;
     default:
      out<< level<< "th ";
     }
     out << print();
}



string SkillRule::printLevel(int level)
{
   switch (level)
   {
     case 0:
      return string( "novice ") + print();
      break;
     case 1:
      return string( "1st ") + print();
      break;
     case 2:
      return string( "2nd ") + print();
      break;
     case 3:
      return string( "3rd ") + print();
      break;
     default:
     {
      return longtostr(level) + "th "+ print();
      }
     }
    return  string("");
}



USING_RESULT     SkillRule::mayBeUsedBy(TokenEntity * tokenEntity)
{
  int nextLevel =  tokenEntity->getSkillLevel(this);
  if(usingParadigm_[nextLevel])
    return usingParadigm_[nextLevel]->mayUse(tokenEntity, this);
  else
    return  UNUSABLE;
}



void SkillRule::reportUse(USING_RESULT result, TokenEntity * tokenEntity)
{
  int level =  tokenEntity->getSkillLevel(this);
   if(usingParadigm_[level])
        usingParadigm_[level]->reportUse(result, tokenEntity);
}



USING_RESULT SkillRule::use(TokenEntity * tokenEntity, int & useCounter)
{
  int level =  tokenEntity->getSkillLevel(this);
//  cout <<tokenEntity->print()<<" using skill "<< print()<<" at level "<<level << "  keyword "<<usingParadigm_[level]->getKeyword()<<endl;
   USING_RESULT result = usingParadigm_[level]->use(tokenEntity,this,useCounter);
   if( result == UNUSABLE)
   {
       tokenEntity->addReport( new UnaryMessage(unusableSkillReporter, this));
    }

  return result;
}



SkillRule * SkillRule::getBasicSkill()
{

  SkillRule * current= this;
    while(current)
    {
      if (current->getRequirement(0) == 0)
        return current;
      else
      current = current->getRequirement(0)->getSkill();
    }
    return 0;
}



void    SkillRule::extractKnowledge (Entity * recipient, int parameter)
{
  vector <SkillLevelElement *>::iterator iter;
  for(iter = derivatives_[parameter].begin(); iter != derivatives_[parameter].end(); iter++)
  {
    if(*iter)
      recipient->addSkillKnowledge((*iter)->getSkill(),(*iter)->getLevel());
    // No further extraction before skill learned
  }
  if(learningParadigm_[parameter])
    learningParadigm_[parameter]->extractKnowledge(recipient);
  if(usingParadigm_[parameter])
    usingParadigm_[parameter]->extractKnowledge(recipient);
}



void SkillRule::printSkillDescription(int level, ostream & out)
{
// if(level==0)
//  cerr<< "ERROR: no level for " <<print() <<endl;

 assert(level);
 printLevel(level,out);
 out << ": "<< description_[level]<<".";
 BasicLearningStrategy * learningParadigm =  learningParadigm_ [level - 1];
 if(learningParadigm)
  {
    if(studyCost_[level])
      out << " Requires " << expPoints_[level]/learningParadigm->getPointsPerDay() <<" days and $" <<studyCost_[level]<<"/day to reach the level. ";
    else
      out << " This skill cannot be studied.";

    Rule * raceClass = learningParadigm->getRacialClass();
    if(raceClass)
      {
        out << " Only "<<raceClass->getName()<< " may know this skill.";
      }

    if(learningParadigm->isSpecialist())
      {
        out << " This is specialist skill.";
      }
  }

 if(requirement_[level - 1])
  {
    out << " Requires "<<    *(requirement_[level-1])   << " to learn.";
  }
  else
    if(level == 1)
    out << " This is a basic skill.";

 if(learningParadigm)
  {
    if(learningParadigm->getItemRequired())
    {
    out<< " Study of the skill requires the ";
    if(learningParadigm->getItemRequired()->getEquipedNumber())
      out << "equiping";
    else
      out<<"possession";
    out<<" of "<< learningParadigm->getItemRequired()->getItemType()<<".";
    }
    if(!learningParadigm->getName().empty())
      out <<" This is a " << learningParadigm->getName()<<".";
  }
  if(stats_[level].getUpkeep())
    out << " Additional upkeep $" << stats_[level].getUpkeep()<<".";

  if(stats_[level].getControlPoints())
    out << " Skill mastery require " << stats_[level].getControlPoints()<<" control points.";

   // ". Use targets "
   if(targetType_)
   {
     out << " Use targets "<< targetType_->getKeyword();
   }

   BasicUsingStrategy *  usingParadigm = usingParadigm_[level];
   if(usingParadigm)
   {
     usingParadigm->printSkillDescription(out);
   }

   if(!stats_[level].empty())
   {
      out << " Added capacities: "<< stats_[level];
   }

   out<<endl<<endl;
}



int SkillRule::getUseDuration(TokenEntity * tokenEntity)
{
   int level =  tokenEntity->getSkillLevel(this);
   return usingParadigm_[level]->getUseDuration();
}



InventoryElement * SkillRule::getItemRequired(TokenEntity * tokenEntity)
{
   int level =  tokenEntity->getSkillLevel(this);
   return learningParadigm_[level]->getItemRequired();
}



int SkillRule::getProductionBonusValue(SkillRule * skill)
{
  return skillBonuses_->getProductionBonus(skill);
}



int SkillRule::getLearningBonus(SkillRule * skill)
{
  return skillBonuses_->getLearningBonus(skill);
}



int SkillRule::getStudyBonus(SkillRule * skill)
{
  return skillBonuses_->getStudyBonus(skill);
}
