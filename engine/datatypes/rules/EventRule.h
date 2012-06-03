/* 
 * File:   EventRule.h
 * Author: alex
 * Defines type of event
 * Created on March 29, 2011, 10:34 AM
 */

#ifndef EVENTRULE_H
#define	EVENTRULE_H
#include "Rule.h"
class BasicCondition;

class EventRule : public Rule   {
public:
    EventRule( const string & keyword, GameData * parent);
    EventRule( const EventRule * prototype );
//    EventRule(const EventRule& orig);
    virtual ~EventRule();
      virtual STATUS     initialize      ( Parser *parser);
      void save(ostream &out);
      GameData * createInstanceOfSelf();
    void printDescription(ReportPrinter & out);
    vector <AbstractData *> aPrint();
    AbstractData * getParamType(int num);
    string getParam(int n);
    inline BasicCondition * getCondition() {return condition_;}
    inline int getParamNumber() const {return nParam_;}  
    inline bool isGlobal() const {return isGlobal_;}
private:
    int nParam_;
    BasicCondition * condition_;
    vector <string> params_;
    vector <AbstractData *> paramTypes_;
    bool isGlobal_;
};
extern  EventRule      sampleEventRule;
//extern BasicCondition  privateCondition;
#endif	/* EVENTRULE_H */

