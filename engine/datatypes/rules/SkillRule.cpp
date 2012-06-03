/***************************************************************************
                          SkillRule.cpp  -  description
                             -------------------
    begin                : Tue Dec 10 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
#include "CraftUsingStrategy.h"
#include "SkillBonusComboAttribute.h"
#include "GameConfig.h"
//#include "StringData.h"
#include "UnitEntity.h"
#include "BuildUsingStrategy.h"
extern ReportPattern * unusableBuildingSkillReporter;

extern string longtostr(long u);
extern ReportPattern * unusableSkillReporter;
const int SkillRule::maxSkillLevel = 7;
SkillRule     sampleSkill     ("SKILL",    &sampleGameData);
//RulesCollection <SkillRule>      skills(new DataStorageHandler(gameConfig.getSkillsFile()));
//RulesCollection <SkillRule>      skills(new DataStorageHandler("skills.rules"),&sampleSkill);

SkillRule::SkillRule ( const SkillRule * prototype ) : Rule(prototype)
{
   currentLevel_ = 0;
   max_ = 0;
   learningParadigm_.resize(maxSkillLevel+1);
   usingParadigm_.resize(maxSkillLevel+1);
   learningParadigm_[0] = new BasicLearningStrategy(sampleLearning);
   usingParadigm_[0] = new BasicUsingStrategy(sampleUsing);
//   resources_.resize(maxSkillLevel+1);
   studyCost_.resize(maxSkillLevel+1);
   stats_.resize(maxSkillLevel+1);
   description_.resize(maxSkillLevel+1);
   capacity_.resize(maxSkillLevel+1);
	 expPoints_.resize(maxSkillLevel+1);
   requirement_.resize(maxSkillLevel+1);
   requirement_[0] = 0 ;
//   requirement_[1] = 0 ;
   derivatives_.resize(maxSkillLevel+1);
	 combatAction_.resize(maxSkillLevel+1);
	 combatAction_[0] = 0;
     isCombat_ = false;
     isMagic_ = false;
     targetType_ = 0;
    skillBonuses_ = new SkillBonusComboAttribute();
     isElementalMagic_ = false;
     learningParadigmGenerator_ =0;
     usingParadigmGenerator_=0;
     combatActionGenerator_=0;
     isAnyoneCanUse_ = false;
     isNoLearn_= false;
     isNoExp_= false;
     isBasic_ =false;
}



void SkillRule::initLevel_ (int level)
{
  if (level < 1)
      return;
//  resources_[level] = resources_[level - 1];
  if(learningParadigm_[level - 1] != 0)
  {
    learningParadigm_[level] = (learningParadigm_ [level - 1])->cloneSelf();
  }
  else
  {
    learningParadigm_[level] = 0;
  }

  if(usingParadigm_[level-1] != 0)
  {

    usingParadigm_[level] = (usingParadigm_ [level - 1])->cloneSelf();

  }
  else
  {
    usingParadigm_[level] = 0;
  }

//   if(learningParadigmGenerator_ != 0)
// 	{
//           learningParadigm_[level] = dynamic_cast<BasicLearningStrategy *>(learningParadigmGenerator_->createInstanceOfSelf ());
//         }
//     else
// 	 {
//   learningParadigm_[level] = (learningParadigm_ [level - 1])->cloneSelf();
// 	 }
//   


//    if(usingParadigmGenerator_ != 0)
// 	{
//          usingParadigm_[level] = dynamic_cast<BasicUsingStrategy *>(usingParadigmGenerator_->createInstanceOfSelf ());
// 	}
// 	 else
// 	{
//   usingParadigm_[level] = (usingParadigm_ [level - 1])->cloneSelf();
// 	}

//  if(studyCost_[level] == 0)
   studyCost_[level] = studyCost_[level - 1];
//  if(stats_[level] == 0)
   stats_[level] = stats_[level - 1];
//  if(description_[level].empty())
   description_[level] = description_[level - 1];
//  if(capacity_[level].size() == 0)
   capacity_[level] = capacity_[level - 1];

   if(combatAction_[level-1] != 0)
   {

     combatAction_[level] = (combatAction_[level - 1])->cloneSelf();
//               if(this == skills["frai"])
//               {
//                 cout<<"  "<<this->print()<<" "<< level<<". ";
//                 combatAction_[level]->debugPrint();
//                 cout <<" Was: ";
//                 combatAction_[level-1]->debugPrint();
//                 cout<<" " <<combatAction_[level] <<endl;
//               }

   }
   else
   {
     combatAction_[level] = 0;
   }
   
   
//    if(combatActionGenerator_)
//            {
//            combatAction_[level] = dynamic_cast<CombatActionStrategy *>(combatActionGenerator_->createInstanceOfSelf ());
//           ////                combatAction_[level] = (combatAction_[level - 1]);
//           //                combatAction_[currentLevel_] = dynamic_cast<CombatActionStrategy *>(temp ->createInstanceOfSelf ());
//           //                (combatAction_[currentLevel_])->setExperienceGainingSkill(this);
//           //                cout <<"   after: "<<print()<<" "<< (combatAction_[level])->print()<<endl;
//           //                                 if(this == skills["frai"])
//           //                                 {
//           //                                   cout<<"  "<<this->print()<<" "<< level<<". ";
//           //                                   combatAction_[level]->debugPrint();
//           //                                   cout <<" Was: ";
//             //                                   combatAction_[level-1]->debugPrint();
//             //                                   cout<<" " <<combatAction_[level] <<endl;
//             //                                 }
//            }
//            else
//            {
//              combatAction_[level] = 0;
//              //cout <<"   No combat action: "<<print()<<endl;
//            }


    //requirement_[level] = requirement_[level-1];

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
//  for(level = 1; level<= maxSkillLevel; ++level)
//  {
//     initLevel_(level);
//  }
// cout<< "postInit "<<print()<<endl;
  max_ = new SkillElement (this, expPoints_[currentLevel_]);
  currentLevel_ =0;
}

void SkillRule::setExpForLevel(int level, int exp)
{
  expPoints_[level]  = exp;
}


GameData * SkillRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<SkillRule> (this);
}
STATUS
SkillRule::initialize        ( Parser *parser )
{
    if (parser->matchKeyword("LEVEL"))
    {
        currentLevel_++;
        initLevel_(currentLevel_);
        if (currentLevel_ > maxSkillLevel)
        {
            cerr << "Error : for " << print() << " Skill Level " << currentLevel_
                    << " is higher than maximum(" << maxSkillLevel << ")\n";
            currentLevel_ = maxSkillLevel;
            return OK;
        }
        expPoints_[currentLevel_] = parser->getInteger();
        //    if( this == skills["mane"] )
        //    cout <<printTag()<< " Mane[" << currentLevel_ <<"] = "<<expPoints_[currentLevel_]<<endl;
        return OK;
    }
    return initializeLevel(parser,currentLevel_);
}

STATUS
SkillRule::initializeLevel        ( Parser *parser, int currentLevel )
{

  if (parser->matchKeyword ("NAME") )
    {
      setName(parser->getText());
      return OK;
    }
   if (parser->matchKeyword ("COMBAT_SKILL") )
    {
      isCombat_ = true;
      return OK;
    }
   if (parser->matchKeyword ("BASIC_SKILL") )
    {
      isBasic_ = true;
      return OK;
    }
   if (parser->matchKeyword ("MAGIC_SKILL") )
    {
      isMagic_ = true;
      return OK;
    }
    if (parser->matchKeyword ("ELEMENTAL_MAGIC_SKILL") )
    {
      isElementalMagic_ = true;
      isMagic_ = true;
      return OK;
    }

  if (parser->matchKeyword("DESCRIPTION"))
    {
      description_[currentLevel] = parser->getText();
      return OK;
    }
  if (parser->matchKeyword ("REQUIRES") )
    {
        SkillLevelElement * parentSkill  = SkillLevelElement::readElement(parser);
      requirement_[currentLevel] = parentSkill;
      if(currentLevel==0) // REQUIRES should be defined for level 1, but in old base it is defined for 0
      {
        requirement_[1] = parentSkill;
      }
      

      if(parentSkill )
          parentSkill->getSkill()->addDerivative(new SkillLevelElement(this,currentLevel),parentSkill->getLevel());
      return OK;
    }
  if (parser->matchKeyword("CAPACITY"))
    {
      if(parser->matchInteger())
				{
					int index =  parser->getInteger();
					(capacity_[currentLevel])[index]  = parser->getInteger();
				}
			else
					{
						string modeTag = parser->getWord();
						if(gameFacade->movementModes.isValidTag(modeTag))
							{
								(capacity_[currentLevel])[modeTag]  = parser->getInteger();
							}
					}
      return OK;
    }
  if (parser->matchKeyword ("STUDY_COST") )
    {
      studyCost_[currentLevel] = parser->getInteger();
      return OK;
    }

	if (parser->matchKeyword ("LEARNING_PARADIGM") )
    {
        string keyword = parser->getWord();
        learningParadigmGenerator_ =  prototypeManager->findInRegistry(keyword);
        if(learningParadigmGenerator_ == 0)
				{
					cerr << "Unknown learning paradigm " << keyword  << " for skill " << print()<< endl;
				}
			else
				{
  				learningParadigm_[currentLevel] =
                                    dynamic_cast<BasicLearningStrategy *>(learningParadigmGenerator_ ->createInstanceOfSelf ());
        }
      return OK;
    }

    if (parser->matchKeyword("USING_PARADIGM"))
    {
        string keyword = parser->getWord();
//        if(keyword==string("USING_ACTION"))
//        {
//            cout << print()<< " is using action"<<endl;
//        }
        usingParadigmGenerator_ = prototypeManager->findInRegistry(keyword);
        if (usingParadigmGenerator_ == 0)
        {
            cerr << "Unknown using paradigm " << keyword << " for skill " << print() << endl;
        }
        else
        {
            usingParadigm_[currentLevel] =
                    dynamic_cast<BasicUsingStrategy *> (usingParadigmGenerator_ ->createInstanceOfSelf());
        }
        return OK;
    }

  if (parser->matchKeyword ("TARGET") )
    {
       targetType_ = createByKeyword(parser->getWord());
      return OK;
    }

  if (parser->matchKeyword ("USE_ANYONE") )
    {
      isAnyoneCanUse_ = true;
      return OK;
    }

  if (parser->matchKeyword ("NO_LEARN") )
    {
      isNoLearn_ = true;
      return OK;
    }

  if (parser->matchKeyword ("NO_EXP") )
    {
      isNoExp_ = true;
      return OK;
    }

  if (parser->matchKeyword ("COMBAT_ACTION") )
    {
//       if(this == skills["frai"])
//         cout<<"frai"<<endl;
      string keyword = parser->getWord();
      combatActionGenerator_ =  prototypeManager->findInRegistry(keyword);
      if(combatActionGenerator_ == 0)
                {
                        cerr << "Unknown combat action " << keyword  << " for skill "
                                << print()<< endl;
                }
	else
		{
                                  //cout <<"For "<<print()<<" "<< temp->print()<<endl;
                  combatAction_[currentLevel] = dynamic_cast<CombatActionStrategy *>(combatActionGenerator_ ->createInstanceOfSelf ());
                  (combatAction_[currentLevel])->setExperienceGainingSkill(this);
//                                 cout <<"   after: "<<print()<<" "<< (combatAction_[currentLevel_])->print()<<endl;
//                                 if(this == skills["frai"])
//                                 {
//                                   cout<<"  "<<this->print()<<" "<<currentLevel_<<" ";
//                                   combatAction_[currentLevel_]->debugPrint();
//                                   cout<<" " <<combatAction_[currentLevel_] <<endl;
//                                 }
 
                }
      return OK;
    }
	if (parser->matchKeyword("COMBAT"))
    {
        if (combatAction_[currentLevel] == 0)
        {
            cerr << "combat parameter COMBAT " << parser->getText() << " defined before combat action  for skill " << print() <<" at level "<<currentLevel << endl;
            return OK;
        }

        combatAction_[currentLevel]->initialize(parser);

        return OK;
    }
    //  if (parser->matchKeyword ("") )
    //    {
    //       = parser->getInteger();
    //      return OK;
    //    }
    if (stats_[currentLevel].initialize(parser) != OK)
        cerr << "Error on initialization of stats modifiers for " << print()
                << " (" << currentLevel << ")" << endl;
        if (learningParadigm_[currentLevel])
        {
            if (learningParadigm_[currentLevel]->initialize(parser) != OK)
                cerr << "Error on initialization of learning Paradigm for " << print()
                << " (" << currentLevel << ")" << endl;
        }
    if (usingParadigm_[currentLevel])
    {
        if (usingParadigm_[currentLevel]->initialize(parser) != OK)
            cerr << "Error on initialization of using Paradigm for " << print()
            << " (" << currentLevel << ")" << endl;
    }
    if (combatAction_[currentLevel])
    {
        if (combatAction_[currentLevel]->initialize(parser) != OK)
            cerr << "Error on initialization of combat action for " << print()
            << " (" << currentLevel << ")" << endl;
    }

    skillBonuses_->initialize(parser);
    movementBonuses_.initialize(parser);
    Rule::initialize(parser);

    return OK;

}

void SkillRule::save(ostream &out)
{
  for(int i = 0; i < maxSkillLevel; ++i)
  {
    saveLevel(out,i,false);
  }
}

void SkillRule::saveLevel(ostream &out, int level, bool isExtended)
{
    if (level == 0 || isExtended)
    {
        Rule::save(out);
        if (isCombat_) out << "COMBAT_SKILL" << endl;
        if (isBasic_) out << "BASIC_SKILL" << endl;
        if (isMagic_) out << "MAGIC_SKILL" << endl;
        if (isElementalMagic_) out << "ELEMENTAL_MAGIC_SKILL" << endl;

        if (isAnyoneCanUse_) out << "USE_ANYONE" << endl;
        if (isNoLearn_) out << "NO_LEARN" << endl;
        if (isNoExp_) out << "NO_EXP" << endl;
        if (targetType_) out << "TARGET" << " "
                << targetType_->getKeyword() << endl;
    }


    if (expPoints_[level] == 0)
    {
        return;
    }


    if (!description_[level].empty()) out << "DESCRIPTION"
            << " " << description_[level] << endl;
    if (requirement_[level])
    {
        out << "REQUIRES ";
        requirement_[level]->save(out);
    }
    for (int i = 0; i < gameFacade->movementModes.size(); ++i)
    {
        if (capacity_[level][i]) out << "CAPACITY " << (gameFacade->movementModes[i])->getTag()
            << " " << capacity_[level][i] << endl;
    }
    if (studyCost_[level]) out << "STUDY_COST" << " " << studyCost_[level] << endl;
    stats_[level].save(out, "", 0);
    if (learningParadigm_[level] && !isNoLearn_)
    {
        out << "LEARNING_PARADIGM" << " " << learningParadigm_[level]->getKeyword() << endl;
        (learningParadigm_[level])->save(out); //provide save(out)
    }
    if (usingParadigm_[level])
    {
        if ((usingParadigm_[level])->getKeyword() != "USING")
        {
            out << "USING_PARADIGM" << " " << usingParadigm_[level]->getKeyword() << endl;
            (usingParadigm_[level])->save(out); //provide save(out)
        }
    }

    if (combatAction_[level])
    {
        out << "COMBAT_ACTION" << " " << combatAction_[level]->getKeyword() << endl;
        (combatAction_[level])->save(out); //complete save(out)
    }
    if (level > 0)
    {
        out << "LEVEL" << " " << expPoints_[level] << endl;
    }
}


SkillElement * SkillRule::getMax()
{
  return max_;
}



LEARNING_RESULT SkillRule::mayBeStudied(TokenEntity * tokenEntity)
{
    if(isNoLearn_)
    {
//        if(tokenEntity->isTraced())
//        {
//        cout << tokenEntity->print() <<" can't study " << this->print()<<endl;
//        }
        return CANNOT_STUDY_FAILURE;
    }
  int nextLevel =  tokenEntity->getSkillLevel(this);
//  if(nextLevel >6)
//    cout << tokenEntity << " has " << tokenEntity->getSkillPoints(this)<< " of " <<print()<<endl;
  assert(learningParadigm_[nextLevel]);
  return learningParadigm_[nextLevel]->mayStudy(tokenEntity, this);
}



bool SkillRule::teacherRequired(TokenEntity * tokenEntity)
{
  int nextLevel =  tokenEntity->getSkillLevel(this);
  assert(learningParadigm_[nextLevel]);
  return learningParadigm_[nextLevel]->teacherRequired(tokenEntity, this);
}



int  SkillRule::calculateLearningExperience(TokenEntity * tokenEntity, TeachingOffer * teacher = 0)
{
  int nextLevel =  tokenEntity->getSkillLevel(this);
  assert(learningParadigm_[nextLevel]);
  return learningParadigm_[nextLevel]->calculateLearningExperience(tokenEntity, this, teacher);
}



int  SkillRule::calculateUsingExperience(TokenEntity * tokenEntity)
{
    if(isNoExp_)
    {
        return 0;
    }
    int nextLevel =  tokenEntity->getSkillLevel(this);
  return usingParadigm_[nextLevel]->calculateUsingExperience(tokenEntity, this);
}


void SkillRule::addLearningExperience(TokenEntity * tokenEntity, int exp)
{
  int nextLevel = 0;
  SkillElement experience(this,exp);
  assert(learningParadigm_[nextLevel]);
  learningParadigm_[nextLevel]->addLearningExperience(tokenEntity,experience);
}



void SkillRule::addUsingExperience(TokenEntity * tokenEntity, int exp)
{
    if(isNoExp_)
    {
        return;
    }
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
 *  Skill itself is not a Descend
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
  // If different requirement skills  are supported all of them should be tried

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
    if(isAnyoneCanUse_)
    {
        return USING_OK;
    }
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



USING_RESULT SkillRule::use(TokenEntity * tokenEntity, int & useCounter, OrderLine * order)
{

  int level =  tokenEntity->getSkillLevel(this);
  if(tokenEntity->isTraced())
  {
  cout << tokenEntity->print()<<" "<<print()<<" level " <<level<<" "<<usingParadigm_[level]->getProductionDays()<<endl;
  }
   USING_RESULT result = usingParadigm_[level]->use(tokenEntity,this,useCounter,order);
   if( result == UNUSABLE)
   {
       tokenEntity->addReport( new UnaryMessage(unusableSkillReporter, this));
    }

  return result;
}

// This is a shortcut of OO design. This method is for specific use of building skill
USING_RESULT SkillRule::useBuild(UnitEntity * unit, ConstructionEntity * construction )
{

  int level =  unit->getSkillLevel(this);
//  cout <<tokenEntity->print()<<" using skill "<< print()<<" at level "<<level << "  keyword "<<usingParadigm_[level]->getKeyword()<<endl;
  BuildUsingStrategy * strategy = dynamic_cast<BuildUsingStrategy *> (usingParadigm_[level]);
  if(!strategy)
  {
    unit->addReport( new UnaryMessage(unusableBuildingSkillReporter, this));
    return UNUSABLE;
  }
  
  USING_RESULT result = strategy->build(unit,this,construction);
  if( result == UNUSABLE)
  {
    unit->addReport( new UnaryMessage(unusableSkillReporter, this));
  }

  return result;
}


SkillRule * SkillRule::getBasicSkill()
{

  SkillRule * current= this;
    while(current)
    {
      if (current->isBasicSkill())
        return current;
      if(current->getRequirement(1) == 0)
        return 0;
      else
      current = current->getRequirement(1)->getSkill();
    }
    return 0;
}



void    SkillRule::extractKnowledge (Entity * recipient, int level)
{
//    if(tag_=="eawb")
//    {
//        cout<<"Extracting "<<getTag()<<endl;
//    }
  Rule::extractKnowledge(recipient);

// Should not extract knowledge from derivatives.
// Instead of that these knowledge should be extracted from "may learn" skills.
//  vector <SkillLevelElement *>::iterator iter;
//  for(iter = derivatives_[level].begin(); iter != derivatives_[level].end(); iter++)
//  {
//    if(*iter)
//    {
//      int level = (*iter)->getLevel();
//      if(level==0) level =1; // This is workaround for bad data order in skill riles: REQUIRES goes before LEVEL
//      recipient->addSkillKnowledge((*iter)->getSkill(),level);
//    }
//    // No further extraction before skill learned
//  }
  int effectiveLevel = level;
  if(effectiveLevel==0) effectiveLevel=1;
  SkillLevelElement * requirement =requirement_[effectiveLevel];
  if(requirement)
  {
      if(recipient->addSkillKnowledge(requirement->getSkill(),requirement->getLevel()))
      {
        requirement->getSkill()->extractKnowledge(recipient,requirement->getLevel());
      }
  }
  //assert(learningParadigm_[level]);
  if(learningParadigm_[level])
    learningParadigm_[level]->extractKnowledge(recipient);
  if(usingParadigm_[level])
    usingParadigm_[level]->extractKnowledge(recipient);
  if(combatAction_[level])
     combatAction_[level]->extractKnowledge(recipient);
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
    if(studyCost_[level]&&!isNoLearn_)
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
if(isCombat_)
{
  out << " This is combat skill.";
}
if(isElementalMagic_)
{
  out << " This is Elemental Magic skill.";
}
  
 if(requirement_[level])
  {
    out << " Requires "<<    *(requirement_[level])   << " to learn.";
  }
 if(isBasic_)
 {
    out << " This is a basic skill.";
 }

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
    if(!learningParadigm->getName().empty() && ciStringCompare(learningParadigm->getName(),string("Learning") ) )
    {
      out <<" This is a " << learningParadigm->getName()<<".";
    }
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
 if(this->isAnyoneCanUse_)
 {
     out << " Anyone can use this skill.";
 }
 if(this->isNoExp_)
 {
     out << " Using of this skill gives no experience.";
 }

   if(!stats_[level].empty())
   {
     int mana = stats_[level].getMana();
     if(mana) //mana is printed separatelly
     {
      out << " Added capacities: "<<"Mana: "<<mana <<" "<< stats_[level];
     }
     else
     {
       out << " Added capacities: "<< stats_[level];
     }      
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


bool SkillRule::isRacialEnabled(RaceRule * race)
{
  assert (learningParadigm_[1]);
  if((RaceRule *)(learningParadigm_[1]->getRacialClass()) == race)
    return true;
  return false;
}


vector <AbstractData *>  SkillRule::aPrint(int level)
{
    //cout<<"Printing "<<print()<<" on level "<<level <<endl;
    vector <AbstractData *> out;

    out.push_back(new StringData(printLevel(level)));
    out.push_back(new StringData(". "));
    out.push_back(new StringData(description_[level]));
    out.push_back(new StringData(". "));
    BasicLearningStrategy * learningParadigm;
    if(level==0)
    {
        learningParadigm =  learningParadigm_ [level];
    }
    else
    {
        learningParadigm =  learningParadigm_ [level - 1];
    }
    if(learningParadigm)

    {
        if(studyCost_[level]&&!isNoLearn_)
        {
            out.push_back(new StringData(" Requires "));
            out.push_back(new IntegerData(expPoints_[level]/learningParadigm->getPointsPerDay()));
            out.push_back(new StringData(" days and $"));
            out.push_back(new IntegerData(studyCost_[level]));
            out.push_back(new StringData("/day to reach the level. "));
        }
        else
        {
            out.push_back(new StringData(" This skill cannot be studied."));
        }

        Rule * raceClass = learningParadigm->getRacialClass();
        if(raceClass)
        {
            out.push_back(new StringData(" Only "));
            out.push_back(new StringData(raceClass->getName()));
            out.push_back(new StringData(" may know this skill."));
        }

        if(learningParadigm->isSpecialist())
        {
            out.push_back(new StringData(" This is specialist skill."));
        }
    }
    if(isCombat_)
    {
        out.push_back(new StringData(" This is combat skill."));

    }
    if(isElementalMagic_)
    {
        out.push_back(new StringData(" This is Elemental Magic skill."));
    }

    if(requirement_[level])
    {
        out.push_back(new StringData(" Requires "));
        out.push_back(new StringData((requirement_[level])->print()));
        out.push_back(new StringData(" to learn."));
    }
    if(isBasic_)
    {
        out.push_back(new StringData("This is a basic skill. "));
    }

    if(learningParadigm)
    {
        if(learningParadigm->getItemRequired())
        {
            out.push_back(new StringData(" Study of the skill requires the "));
            if(learningParadigm->getItemRequired()->getEquipedNumber())
            {
                out.push_back(new StringData("equiping"));
            }
            else
            {
                out.push_back(new StringData("possession"));
            }
            out.push_back(new StringData(" of "));
            out.push_back(new StringData(learningParadigm->getItemRequired()->getItemType()->getName()));
            out.push_back(new StringData("."));
        }

        if(!learningParadigm->getName().empty() && ciStringCompare(learningParadigm->getName(),string("Learning") ) )
        {
            out.push_back(new StringData(" This is a "));
            out.push_back(new StringData(learningParadigm->getName()));
            out.push_back(new StringData(". "));

        }
    }
    if(stats_[level].getUpkeep())
    {
        out.push_back(new StringData(" Additional upkeep $"));
        out.push_back(new IntegerData(stats_[level].getUpkeep()));
        out.push_back(new StringData("."));
    }

    if(stats_[level].getControlPoints())
    {

        out.push_back(new StringData(" Skill mastery require "));
        out.push_back(new IntegerData(stats_[level].getControlPoints()));
        out.push_back(new StringData(" control points."));
    }

    // ". Use targets "
    if(targetType_)
    {
        out.push_back(new StringData(" Use targets "));
        out.push_back(new StringData(targetType_->getKeyword()));
    }

    BasicUsingStrategy *  usingParadigm = usingParadigm_[level];
    if(usingParadigm)
    {
        //     usingParadigm->printSkillDescription(out);
    }
    if(this->isAnyoneCanUse_)
    {
        out.push_back(new StringData(" Anyone can use this skill."));
    }
    if(this->isNoExp_)
    {
        out.push_back(new StringData(" Using of this skill gives no experience."));
    }

    if(!stats_[level].empty())
    {
        int mana = stats_[level].getMana();
        bool isFirst = true;
        vector<AbstractArray> statPrint = (stats_[level]).aPrint();
        if(mana) //mana is printed separatelly
        {
            out.push_back(new StringData(" Added capacities: Mana: "));
            out.push_back(new IntegerData(mana));
            out.push_back(new StringData(", "));
        }
        else
        {
             out.push_back(new StringData(" Added capacities: "));

        }
        for(vector <AbstractArray>::iterator iter = statPrint.begin();
        iter != statPrint.end(); ++iter)
        {
            if(!isFirst)
            {
                out.push_back(new StringData(", "));
            }
            else
            {
                isFirst = false;
            }
            for(vector <AbstractData *>::iterator iter2 = (*iter).begin();
            iter2 != (*iter).end(); ++iter2)
            {
                out.push_back(*iter2);
            }
    }
 out.push_back(new StringData(". "));
}

    return out;
}

