/***************************************************************************
                          SkillRule.cpp  -  description
                             -------------------
    begin                : Tue Dec 10 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "SkillRule.h"
#include "ItemElement.h"
#include "SkillElement.h"
#include "SkillLevelElement.h"
#include "UnitEntity.h"
#include "BasicLearningStrategy.h"
#include "PrototypeManager.h"
#include "Reporter.h"
#include "UnaryPattern.h"
extern void longtostr(unsigned long u, char *out);
extern BasicLearningStrategy   sampleLearning;
extern BasicUsingStrategy   sampleUsing;
extern Reporter * unusableSkillReporter;
const int SkillRule::maxSkillLevel;

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
     isCombat_ = false;
     isMagic_ = false;
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
					cout << "Error : for "<<printName() <<" Skill Level is higher than maximum\n";
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
//  if (parser->matchKeyword ("CONSUME") )
//    {
//			if(parser->matchElement())
//			(resources_[currentLevel_]).push_back( new ItemElement(parser));
////			(resources_[currentLevel_])->save(cout); cout << endl;
//      return OK;
//    }
  if (parser->matchKeyword ("LEARNING_PARADIGM") )
    {
        string keyword = parser->getWord();
       	GameData * temp =  GameData::prototypeManager->findInRegistry(keyword);
			if(temp == 0)
				{
					cout << "Unknown learning paradigm " << keyword  << " for skill " << printName()<< endl;
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
       	GameData * temp =  GameData::prototypeManager->findInRegistry(keyword);
			if(temp == 0)
				{
					cout << "Unknown using paradigm " << keyword  << " for skill " << printName()<< endl;
				}
			else
				{
  				usingParadigm_[currentLevel_] =
                  dynamic_cast<BasicUsingStrategy *>(temp ->createInstanceOfSelf ());
//          cout << printName()<<" at level "<<currentLevel_ << " using keyword "<<usingParadigm_[currentLevel_]->getKeyword()<<endl;        
        }
      return OK;
    }

//  if (parser->matchKeyword ("") )
//    {
//       = parser->getInteger();
//      return OK;
//    }
     stats_[currentLevel_].initialize(parser);
     learningParadigm_[currentLevel_]->initialize(parser);
     usingParadigm_[currentLevel_]->initialize(parser); 
      return OK;

 }
SkillElement * SkillRule::getMax()
{
  if (max_ == 0)
   max_ = new SkillElement (this, expPoints_[currentLevel_]);
  return max_;
}

LEARNING_RESULT SkillRule::mayStudy(UnitEntity * unit)
{
  int nextLevel =  unit->getSkillLevel(this);
//  if(nextLevel >6)
//    cout << unit << " has " << unit->getSkillPoints(this)<< " of " <<printName()<<endl;
  return learningParadigm_[nextLevel]->mayLearn(unit, this);
}

bool SkillRule::teacherRequired(Entity * unit)
{
  int nextLevel =  unit->getSkillLevel(this);
  return learningParadigm_[nextLevel]->teacherRequired(unit, this);
}



int  SkillRule::calculateLearningExperience(UnitEntity * unit, TeachingOffer * teacher = 0)
{
  int nextLevel =  unit->getSkillLevel(this);
  return learningParadigm_[nextLevel]->calculateLearningExperience(unit, this, teacher);
}
int  SkillRule::calculateUsingExperience(UnitEntity * unit)
{
  int nextLevel =  unit->getSkillLevel(this);
  return usingParadigm_[nextLevel]->calculateUsingExperience(unit, this);
}
void SkillRule::addLearningExperience(UnitEntity * unit, int exp)
{
  int nextLevel = 0;
  SkillElement experience(this,exp);
  learningParadigm_[nextLevel]->addLearningExperience(unit,experience);
}

void SkillRule::addUsingExperience(UnitEntity * unit, int exp)
{
  int nextLevel = 0;
  SkillElement experience(this,exp);
  usingParadigm_[nextLevel]->addUsingExperience(unit,experience);
}

//void SkillRule::addRecursiveExperience(UnitEntity * unit, SkillElement & skill)
//{
//  learningParadigm_[nextLevel]->addRecursiveLearningExperience(unit,skill);
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
     
void SkillRule::print()
{
    cout  << getName();
    cout << " [" << getTag()  << "] ";
}

/** Determines if current skill is in the tree growing from the given skill  */
bool SkillRule::isDescendFrom(SkillRule * root, int level) 
{
  SkillRule * current;
  int tryLevel; 
  for (tryLevel = 0; tryLevel< level; tryLevel++)
  {
    for(current = this; current != 0; current = current ->getRequirement(tryLevel)->getSkill())
      {
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


int SkillRule::getStudyCost(UnitEntity * const unit) 
{
  return  studyCost_[unit->getSkillLevel(this)];
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
     out << printName();
}

string SkillRule::printLevel(int level)
{
   switch (level)
   {
     case 0:
      return string( "novice ") + printName();
      break;
     case 1:
      return string( "1st ") + printName();
      break;
     case 2:
      return string( "2nd ") + printName();
      break;
     case 3:
      return string( "3rd ") + printName();
      break;
     default:
     {
      char buffer[12];
      longtostr(level,buffer);
//		  sprintf(buffer,"%d",level);// May use hand-made convertor itoa
      return string( buffer) + "th "+ printName();
      }
     }
    return  string(""); 
}
USING_RESULT     SkillRule::mayUse(UnitEntity * unit)
{
  int nextLevel =  unit->getSkillLevel(this);
  if(usingParadigm_[nextLevel])
    return usingParadigm_[nextLevel]->mayUse(unit, this);
  else
    return  UNUSABLE;
}



void SkillRule::reportUse(USING_RESULT result, UnitEntity * unit, Order * OrderId)
{
  int level =  unit->getSkillLevel(this);
   if(usingParadigm_[level])
        usingParadigm_[level]->reportUse(result, unit,OrderId);
}



bool SkillRule::use(UnitEntity * unit, Order * OrderId)
{
  int level =  unit->getSkillLevel(this);
//  cout <<unit->printName()<<" using skill "<< printName()<<" at level "<<level << "  keyword "<<usingParadigm_[level]->getKeyword()<<endl;
   if(usingParadigm_[level]->use(unit,OrderId))
      return true;
   else   
   {
       unit->addReport( new UnaryPattern(unusableSkillReporter, this));
    }
  
  return false;
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

void SkillRule::printDescription(int level, ostream & out)
{
  assert(level);
 printLevel(level,out);
 out << ": "<< description_[level]<<".";
 
 if(learningParadigm_ [level - 1])
  {
    out << " Requires " << expPoints_[level]/learningParadigm_ [level-1]->getPointsPerDay() <<" days and $" <<studyCost_[level]<<"/day to reach the level. ";
  }
 if(requirement_[level - 1])
  {
    out << " Requires ";
    requirement_[level-1]->print(out);
    out << " to learn.";
  }
  else
    if(level == 1)
    out << " This is a basic skill.";
}
