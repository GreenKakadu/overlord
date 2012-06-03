/*
    Copyright (c) <year>, <copyright holder>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY <copyright holder> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "PlagueEffect.h"
#include "EffectEntity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "UnaryMessage.h"
#include "FactionEntity.h"
#include "ReportElement.h"

PlagueEffectRule         samplePlagueEffect  ("PLAGUE",  &sampleEffectRule);
extern ReportPattern * plagueReporter;
extern ReportPattern * plagueStartReporter;

PlagueEffectRule::PlagueEffectRule( const  PlagueEffectRule* prototype ) : EffectRule(prototype)
{
     maxLength_=0;
     lethality_=0;
     virality_=0;
     incubationPeriod_=0;
     basicChance_ = 1;
     infectionEnchant_ =0;
     immunityEnchant_ =0;
     illnessDuration_ = gameFacade->getGameConfig()->daysInMonth;
}



PlagueEffectRule::~PlagueEffectRule(){
}

GameData * PlagueEffectRule::createInstanceOfSelf()
{
   return CREATE_INSTANCE<PlagueEffectRule> (this);
}

STATUS
PlagueEffectRule::initialize        ( Parser *parser )
{
        if (parser->matchKeyword("LENGTH"))
 	{
          maxLength_= parser->getInteger();
 	  return OK;
 	}
        if (parser->matchKeyword("VIRALITY"))
 	{
          virality_= parser->getInteger();
 	  return OK;
 	}
        if (parser->matchKeyword("LETHALITY"))
 	{
          lethality_= parser->getInteger();
 	  return OK;
 	}
        if (parser->matchKeyword("INCUBATION_PERIOD"))
 	{
          incubationPeriod_= parser->getInteger();
 	  return OK;
 	}
        if (parser->matchKeyword("INFECTION_ENCHANT"))
 	{
           infectionEnchant_ = gameFacade->enchantments[parser->getWord()];
 	  return OK;
 	}
        if (parser->matchKeyword("IMMUNITY_ENCHANT"))
 	{
           immunityEnchant_ = gameFacade->enchantments[parser->getWord()];
 	  return OK;
 	}

	return EffectRule::initialize(parser );
}

void PlagueEffectRule::terminateEffect(EffectEntity * entity)
{
    // Remove effect from lists

}

void    PlagueEffectRule::preprocessData()
{
}



/*
 * Monthly post-processing
 */
void PlagueEffectRule::postProcessData()
{
    // Get list of all units
    // infect some of them
    // test expiration
    cout << "   ===================  PlagueEffectRule::postProcessData"<<endl;
}



void      PlagueEffectRule::save (ostream &out)
{
       EffectRule::save(out);
       if(maxLength_) out <<"LENGTH" <<" "<< maxLength_<<endl;
       if(virality_) out <<"VIRALITY" <<" "<<virality_ <<endl;
       if(lethality_) out <<"LETHALITY" <<" "<<lethality_ <<endl;
       if(incubationPeriod_) out <<"INCUBATION_PERIOD" <<" "<<incubationPeriod_ <<endl;
       if(infectionEnchant_)
       {
           out<<"INFECTION_ENCHANT"<<" "<< infectionEnchant_->getTag()<<endl;
       }
       if(immunityEnchant_)
       {
           out<<"IMMUNITY_ENCHANT"<<" "<< immunityEnchant_->getTag()<<endl;
       }
}



void   PlagueEffectRule::publicReport (int observation, ReportPrinter &out)
{

}

void PlagueEffectRule::saveData(EffectEntity * entity, ostream &out)
 {

 }
bool PlagueEffectRule::isToBeCreated(LocationEntity * location)
{

    if(location->findEffect(this))
    {
        return false; //Already has plague
    }
   ResourceElementIterator iter;
    ItemRule * deads = gameFacade->items["dead"];
    int numDeads;

  for(iter = location->getResources().begin(); iter != location->getResources().end();iter++)
  {
    if ( (*iter)->getResource() == deads)
      {
        numDeads = (*iter)->getAvailableResource().roundUp();// getResourceAmount();
        long base = (basicChance_ + numDeads * 10 +getInfectedNum(location)*10);
        long plagueChance = base * getVirality() / 100;
        plagueChance = plagueChance * location->getPopulation() / 500;
        if(location->isTraced())
        {
            cout<< "---:--- "<<location->print()<<" numDeads = "<<numDeads<<" numInfected "<< getInfectedNum(location)<<" base = "<<base <<" plagueChance = "<<plagueChance<<endl;
        }
	if(Roll_1Dx(1000) < plagueChance)
	{
            return true;
	}

      }
  }
  return false;
}

EffectEntity * PlagueEffectRule::createEffect(LocationEntity * location)
{
  EffectEntity  * newEffect   = dynamic_cast<EffectEntity *> (createByKeyword(entityKeyword_));
  if(newEffect)
  {
    if(gameFacade->effects.addNew(newEffect) != OK)
      {
        cout << "Failed to add new effect \n";
        return 0;
      }
     newEffect->setEffectType(this);
     newEffect->setName(name_);
     newEffect->setEffectExpiration(getMaxLength());
     location->addEffect(newEffect);
     newEffect->explicitInitialization();
  }

      cout << " ---:---> PlagueEffectRule createEffect "<<newEffect->print()
              <<" at "<< location->print()<<endl;
       reportEffectStart(newEffect);
return newEffect;


}

void PlagueEffectRule::dailyPreProcessEffect(EffectEntity * entity)
{
   // Infect all units
    vector<UnitEntity *> unitsPresent = entity->getLocation()->unitsPresent();
    for(UnitIterator iter= unitsPresent.begin(); iter != unitsPresent.end(); ++iter)
    {
        if((*iter)->hasEnchantment(infectionEnchant_)||(*iter)->hasEnchantment(immunityEnchant_))//already infected of have immunity
        {
            continue;
        }
        if(Roll_1Dx(1000) <(getVirality() * 10) /gameFacade->getGameConfig()->daysInMonth)
        { // infect and add immunity (it would be etter to add immunity when infection expires but...)
        (*iter)->addEnchantment(new EnchantmentElement(infectionEnchant_,(incubationPeriod_ + illnessDuration_) * ((*iter)->getFiguresNumber())));
        (*iter)->addEnchantment(new EnchantmentElement(immunityEnchant_,VERY_BIG_NUMBER));
        cout << (*iter)->print() <<" is infected by "<<entity->print() <<endl;
        }
    }
}


void PlagueEffectRule::preProcessEffect(EffectEntity * entity)
{
  if(entity->isTraced())
  {
      cout << " ---:---> PlagueEffectRule preProcessing "<<entity->print()
              <<" at "<< entity->getLocation()->print()<<endl;
  }
}
void PlagueEffectRule::postProcessEffect(EffectEntity * entity)
{
  if(entity->isTraced())
  {
      cout << " ---:---> PlagueEffectRule postProcessing "<<entity->print()
              <<" at "<< entity->getLocation()->print()<<endl;
  }
  LocationEntity * loc =entity->getLocation();
  if(loc)
  {
      int pop = loc->getPopulation();
      loc->setPopulation((loc->getPopulation() * (100 - this->getlethality()))/100);
  }
  entity->getLocation()->getPopulation();
}
void PlagueEffectRule::reportEffectStart(EffectEntity * entity)
{
    FactionEntity * faction;
    LocationEntity * location = entity->getLocation();
    assert(location);
    ReportMessage * report = new UnaryMessage(plagueStartReporter,location);
    vector<UnitEntity *> localUnits = location->unitsPresent();
      cout << " ---:---> PlagueEffect report of plague at "<<location->print()<<endl;
    for(UnitIterator iter= localUnits.begin(); iter != localUnits.end(); ++iter )
    {
       //cout << " ---:---> PlagueEffectRule report of "<<(*iter)->print()<<endl;
       faction = (*iter)->getFaction();        
       faction->updateReports(new ReportElement(report,faction));
    }



      //entity->getLocation()->addReport(new UnaryMessage(plagueReporter,entity->getLocation()));
}

string PlagueEffectRule::reportPublicAppearance(EffectEntity * entity)
{
      return string("Plague is ravaging this area. ");
}

int PlagueEffectRule::getInfectedNum(LocationEntity * location)
{
    int num = 0;
    vector<UnitEntity *> units = location->unitsPresent();
    for(UnitIterator iter= units.begin(); iter != units.end(); ++iter)
    {
        if((*iter)->hasEnchantment(infectionEnchant_))
        {
            num++;
        }
    }

return num;
}

