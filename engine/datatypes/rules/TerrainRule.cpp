/***************************************************************************
                          TerrainRule.cpp  
                             -------------------
    begin                : Mon Nov 12 13:34:00 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "StringData.h"
#include "TerrainRule.h"
#include "SkillRule.h"
#include "GameConfig.h"

TerrainRule   sampleTerrain   ("TERRAIN",  &sampleGameData);
//BasicRulesCollection  terrains(new DataStorageHandler(gameConfig.getTerrainsFile()));
//BasicRulesCollection  terrains(new DataStorageHandler("terrains.rules"),&sampleTerrain);




TerrainRule * findTerrainByTag(const string &tag)
{
 return GET_FROM_COLLECTION<TerrainRule>(&(gameFacade->terrains),tag);
}

TerrainRule::TerrainRule ( const TerrainRule * prototype ) : Rule(prototype)
{
  optimalPopulation_ =0;
  landWalk_ =0;
  buildEnabled_ = true;
  	colorR_ = 255;
	colorG_= 255;
	colorB_ = 255;
  }


GameData * TerrainRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<TerrainRule> (this);
}


STATUS 
TerrainRule::initialize        ( Parser *parser )
{
  GameData::initialize(parser);
	
  if (parser->matchKeyword ("OPTIMA") )
    {
      optimalPopulation_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword ("LANDWALK") )
    {
      landWalk_ = gameFacade->skills[parser->getWord()];
      return OK;
    }
  if (parser->matchKeyword("RESOURCE"))
	{
        ResourceElement * newResource = ResourceElement::readElement(parser);
        if(newResource)
        {
            for(vector <ResourceElement *>::iterator iter =potentialResources.begin();
                    iter != potentialResources.end(); ++iter )
            {
                if((*iter)->getResource() == newResource->getResource())// Overwrite existing resource record
                {
                    (*iter)->setResourceAmount(newResource->getResourceAmount());
                    return OK;
                }
            }
          potentialResources.push_back(newResource);
        }
	  return OK;
	}
  if (parser->matchKeyword ("NOBUILD") )
    {
      buildEnabled_ = false;
      return OK;
    }
  if (parser->matchKeyword ("COLOR_R") )
    {
	  colorR_ =  parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword ("COLOR_G") )
    {
	  colorG_ =  parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword ("COLOR_B") )
    {
	  colorB_ =  parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword ("DAYS") )
    {
      MovementVariety * movementMode = gameFacade->movementModes[parser->getWord()];
       if(movementMode)
            movementTime_[movementMode] = parser->getInteger();
      return OK;
    }
		skillBonuses_.initialize(parser);
		movementBonuses_.initialize(parser);
  Rule::initialize(parser);
      return OK;

 }


void TerrainRule::save(ostream &out)
{
  Rule::save(out);
   if(optimalPopulation_) out<<"OPTIMA " <<optimalPopulation_ <<endl;
    if(landWalk_) out<<"LANDWALK " << landWalk_->getTag()<<endl;
    for(vector <ResourceElement *>::iterator iter = potentialResources.begin();
            iter != potentialResources.end();++iter)
    {
      out<<"RESOURCE ";
      (*iter)->save(out);
    }
    if(!buildEnabled_) out<<"NOBUILD" <<endl;
    if(colorR_) out<<"COLOR_R " << colorR_<<endl;
    if(colorG_) out<<"COLOR_G " <<colorG_ <<endl;
    if(colorB_) out<<"COLOR_B " <<colorB_ <<endl;
  for(int i =0; i <gameFacade->movementModes.size(); ++i)
  {
    if(movementTime_[i]) out << "DAYS "<<(gameFacade->movementModes[i])->getTag()
            <<" " <<movementTime_[i] <<endl;
  }
skillBonuses_.save(out);
movementBonuses_.save(out);
}


void TerrainRule::cleanAttributes()
{
      Rule::cleanAttributes();
        skillBonuses_.clean();
}


int TerrainRule::getTravelTime(MovementVariety * mode)
{

  return   movementTime_[mode];

}
//bool TerrainRule::mayMove(MovementVariety * mode, UnitEntity * unit)
//{return true;}



void TerrainRule::printDescription(ReportPrinter & out)
{
    out << print()<< ": "<< getDescription()<<".";    
    if(! buildEnabled_) out << " Construction of buildings  is not allowed here.";
}



vector <AbstractData *> TerrainRule::aPrint()
{
  vector <AbstractData *> out;
  out.push_back(new StringData(getDescription()));
  if(optimalPopulation_)
  {
  out.push_back(new StringData(" Typical population of "));
  out.push_back(new StringData(getName()));
  out.push_back(new StringData(" is "));
  out.push_back(new IntegerData(optimalPopulation_));
}
  else
  {
      out.push_back(new StringData(getName()));
        out.push_back(new StringData(" are usualy unpopulated."));
  }
  out.push_back(new StringData(""));
  out.push_back(new StringData(""));

  if(! buildEnabled_)
  {
  out.push_back(new StringData("Construction of buildings  is not possible here."));
  }
  if(landWalk_)
  {
    out.push_back(new StringData(landWalk_->getName()));
    out.push_back(new StringData(" skill may be used for surviving in "));
    out.push_back(new StringData(getName()));
    out.push_back(new StringData(". "));
  }

  bool isFirst = true;
  for(vector <ResourceElement *>::iterator iter = potentialResources.begin();
  iter != potentialResources.end();++iter)
  {

      if(!(*iter)->getResource()->getProspectSkill())
      {
          if(isFirst)
          {
              isFirst = false;
          }
          else
          {
              out.push_back(new StringData(", "));
          }
          out.push_back(new StringData((*iter)->getResource()->getName()));
      }
  }
  if(!isFirst)
  {
    out.push_back(new StringData(" may be found here."));
  }

  out.push_back(new StringData(" "));
  for(int i =0; i <gameFacade->movementModes.size(); ++i)
  {
      if(movementTime_[i])
      {

          out.push_back(new StringData((gameFacade->movementModes[i])->getName()));
          out.push_back(new StringData(" in "));
          out.push_back(new StringData(getName()));
          out.push_back(new StringData(" usually takes "));
          out.push_back(new IntegerData(movementTime_[i]));
          out.push_back(new StringData(" days. "));
      }
  }






  vector<AbstractArray> v = skillBonuses_.aPrintReport();
//  isFirst = true;

  for(vector<AbstractArray>::iterator iter= v.begin(); iter != v.end(); iter++)
    {
//      if(!isFirst)
//      {
//      out.push_back(new StringData(", "));
//        }
//      else
//      {
//          isFirst = false;
//      }
      for(AbstractArray::iterator iter2= (*iter).begin(); iter2 != (*iter).end(); iter2++)
        {
            out.push_back(*iter2);
        }

    }

if(!movementBonuses_.isEmpty())
  {
    vector<AbstractArray> v = movementBonuses_.aPrintReport();
    isFirst = true;
    for(vector<AbstractArray>::iterator iter= v.begin(); iter != v.end(); iter++)
      {
        if(!isFirst)
        {
        out.push_back(new StringData(", "));
          }
        else
        {
            isFirst = false;
        }
        for(AbstractArray::iterator iter2= (*iter).begin(); iter2 != (*iter).end(); iter2++)
          {
              out.push_back(*iter2);
          }

      }

}


  return out;
}


int TerrainRule::getProductionBonusValue(SkillRule * skill)
{
  return skillBonuses_.getProductionBonus(skill);
}



int TerrainRule::getLearningBonus(SkillRule * skill)
{
  return skillBonuses_.getLearningBonus(skill);
}



int TerrainRule::getStudyBonus(SkillRule * skill)
{
  return skillBonuses_.getStudyBonus(skill);
}



// Implementation-specific routine for determining aquatic locations
bool TerrainRule::isAquatic()
{
   return(this == gameFacade->terrains.findByTag("lake",false) ||
	 			this == gameFacade->terrains.findByTag( "ocea",false));

}
