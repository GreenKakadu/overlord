/*
    Copyright (c) 2009 Alex Dribin
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

#ifndef PLAGUEEFFECT_H
#define PLAGUEEFFECT_H
#include "EffectRule.h"
class EnchantmentRule;

class PlagueEffectRule: public EffectRule
{
  public:
      PlagueEffectRule (const string & keyword, EffectRule * parent ) : EffectRule(keyword, parent){}
      PlagueEffectRule ( const PlagueEffectRule * prototype );
	   ~PlagueEffectRule();
      virtual GameData * createInstanceOfSelf();
      virtual STATUS  initialize      ( Parser *parser );
      bool isToBeCreated(LocationEntity * location);
       virtual EffectEntity * createEffect(LocationEntity * location);
               void terminateEffect(EffectEntity * entity);
       virtual void      save (ostream &out);
       virtual void     preprocessData();
       virtual void     postProcessData();
       virtual void      publicReport (int observation, ReportPrinter &out);
        void saveData(EffectEntity * entity, ostream &out);
       void preProcessEffect(EffectEntity * entity);
       void postProcessEffect(EffectEntity * entity);
       void dailyPreProcessEffect(EffectEntity * entity);
       void reportEffectStart(EffectEntity * entity);
       string reportPublicAppearance(EffectEntity * entity);
//       static EffectEntity * makeNewPlague ();
inline  int getMaxLength(){return maxLength_;}
inline  int getlethality(){return lethality_;}
inline  int getVirality(){return virality_;}
inline  int getIncubationPeriod(){return incubationPeriod_;}
inline int getIllnessDuration(){return illnessDuration_;}
int getInfectedNum(LocationEntity * location);
protected:
    int maxLength_;
    int lethality_;
    int virality_;
    int illnessDuration_;
    int incubationPeriod_;
    int basicChance_;
    EnchantmentRule * infectionEnchant_;
    EnchantmentRule * immunityEnchant_;
};

#endif // PLAGUEEFFECT_H
