/***************************************************************************
                          Rule.h  -  description
                             -------------------
    begin                : Thu Jan 23 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef RULE_H
#define RULE_H
#include <iostream>
#include <vector>
#include "GameData.h"
#include "ReportPrinter.h"
//#include "RulesCollection.h"
#include "MovementBonusAttribute.h"


class BasicCondition;
class Entity;

class Rule : public GameData  {
public:
    Rule(const Rule * prototype);
    Rule(const string & keyword, GameData * parent) :
                                            GameData(keyword, parent) {}
    virtual STATUS initialize(Parser *parser);
    virtual     void      save (ostream &out);
    string getCollectionKeyword();
//    void bindCondition(BasicCondition * condition);
//    void checkConditions(Entity * entity);
    virtual void extractKnowledge(Entity * recipient, int parameter = 0);
    virtual void printDescription(ReportPrinter & out);
    virtual vector <AbstractData *> aPrint();
    int getMovementBonus(MovementVariety * mode);

    virtual string getPluralName() const { return string("");}
    inline void setTag(const string &tag) {  tag_ = tag; }
protected:

    virtual ~Rule() {}
    inline void setName(const string &name) { name_ = name;   }
    inline void setDescription(const string &description) { description_ = description; }
//    vector <BasicCondition *> conditions_;
    vector <Rule *> explicitKnowledge_;
    // Attributes
    MovementBonusAttribute movementBonuses_;

};

#endif
