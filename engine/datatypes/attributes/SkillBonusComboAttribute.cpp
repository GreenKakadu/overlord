/***************************************************************************
                          SkillBonusComboAttribute.cpp  -  description
                             -------------------
    begin                : Feb 16 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "SkillBonusComboAttribute.h"
#include "BonusElement.h"
#include "MovementVariety.h"
#include "Entity.h"


SkillBonusComboAttribute::SkillBonusComboAttribute()
{
 studyBonuses_ = SkillBonusAttribute("STUDY_BONUS", " Study bonuses");
 learningBonuses_ = SkillBonusAttribute("LEARNING_LEVEL_BONUS"," Skill level bonuses");
 productionBonuses_ = SkillBonusAttribute("USE_BONUS"," Production bonuses");
}






SkillBonusComboAttribute::~SkillBonusComboAttribute(){
}



STATUS
SkillBonusComboAttribute::initialize        ( Parser *parser )
{
		studyBonuses_.initialize(parser);
		learningBonuses_.initialize(parser);
		productionBonuses_.initialize(parser);
   return OK;
}

void SkillBonusComboAttribute::save(ostream &out)
{
    studyBonuses_.save(out);
    learningBonuses_.save(out);
    productionBonuses_.save(out);
}

void SkillBonusComboAttribute::clean()
{
   studyBonuses_.clean(); 
   learningBonuses_.clean();
   productionBonuses_.clean();
}


int SkillBonusComboAttribute::getProductionBonus(SkillRule * skill)
{
	return productionBonuses_.getSkillBonus(skill);
}


int SkillBonusComboAttribute::getStudyBonus(SkillRule * skill)
{
	return studyBonuses_.getSkillBonus(skill);
}


int SkillBonusComboAttribute::getLearningBonus(SkillRule * skill)
{
	return learningBonuses_.getSkillBonus(skill);
}


vector <AbstractArray>  SkillBonusComboAttribute::aPrintReport()
{
    vector <AbstractArray> out;


    if(!studyBonuses_.isEmpty())
    {
        vector <AbstractArray> study = studyBonuses_.aPrintReport();
        for ( vector<AbstractArray >::iterator iter = study.begin();
        iter != study.end(); iter++)
        {
            out.push_back(*iter);

        }
    }

    if(!learningBonuses_.isEmpty())
    {
        vector <AbstractArray> learning = learningBonuses_.aPrintReport();
        for ( vector<AbstractArray >::iterator iter = learning.begin();
        iter != learning.end(); iter++)
        {
            out.push_back(*iter);

        }

    }
    if(!productionBonuses_.isEmpty())
    {
    vector <AbstractArray> product = productionBonuses_.aPrintReport();
    for ( vector<AbstractArray >::iterator iter = product.begin();
    iter != product.end(); iter++)
    {
        out.push_back(*iter);
    }
}
    return out;
}








void    SkillBonusComboAttribute::extractKnowledge (Entity * recipient, int parameter)
{
 	productionBonuses_.extractKnowledge(recipient, 1);
	studyBonuses_.extractKnowledge(recipient, 1);
	learningBonuses_.extractKnowledge(recipient, 1);
}






ostream& SkillBonusComboAttribute::report(ostream &out)
{
	productionBonuses_.report(out);
	studyBonuses_.report(out);
	learningBonuses_.report(out);
		return out;
}

