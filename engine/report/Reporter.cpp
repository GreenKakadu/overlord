/***************************************************************************
                          Reporter.cpp  -  description
                             -------------------
    begin                : Wed Jan 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "Reporter.h"
#include "AbstractData.h"
Reporter::Reporter(const char *rep1)
{
 rep1_ = rep1;
// rep2_ = "";	
}
Reporter::Reporter(const char *rep1, const char *rep2)
{
 rep1_ = rep1;
 rep2_ = rep2;
}
Reporter::Reporter(const char *rep1, const char *rep2, const char *rep3)
{
 rep1_ = rep1;
 rep2_ = rep2;
 rep3_ = rep3;
}

Reporter::Reporter(const char *rep1, const char *rep2, const char *rep3, const char *rep4)
{
 rep1_ = rep1;
 rep2_ = rep2;
 rep3_ = rep3;
 rep4_ = rep4;
}
Reporter::Reporter(const char *rep1, const char *rep2, const char *rep3, const char *rep4, const char *rep5)
{
 rep1_ = rep1;
 rep2_ = rep2;
 rep3_ = rep3;
 rep4_ = rep4;
 rep5_ = rep5;
}
Reporter::Reporter(const char *rep1, const char *rep2, const char *rep3, const char *rep4, const char *rep5, const char *rep6)
{
 rep1_ = rep1;
 rep2_ = rep2;
 rep3_ = rep3;
 rep4_ = rep4;
 rep5_ = rep5;
 rep6_ = rep6;
}



void Reporter::print (ostream &out)
{
	out << rep1_ << endl;
}

void Reporter::print (ostream &out, AbstractData * param1)
{
	out << rep1_ << param1->printName() << rep2_ << endl;
}


void Reporter::print (ostream &out, AbstractData * param1, AbstractData * param2)
{
	out << rep1_ << param1->printName() << rep2_ << param2->printName() << rep3_  << endl;
}


void Reporter::print (ostream &out, AbstractData * param1, AbstractData * param2, AbstractData * param3)
{
	out << rep1_ << param1->printName() << rep2_ << param2->printName() << rep3_  << param3->printName() << rep4_  << endl;
}

void Reporter::print (ostream &out, AbstractData * param1, AbstractData * param2, AbstractData * param3, AbstractData * param4)
{
	out << rep1_ << param1->printName() << rep2_ << param2->printName() << rep3_  << param3->printName() << rep4_  << param4->printName() << rep5_  << endl;
}

void Reporter::print (ostream &out, AbstractData * param1, AbstractData * param2, AbstractData * param3, AbstractData * param4, AbstractData * param5)
{
	out << rep1_ << param1->printName() << rep2_ << param2->printName() << rep3_  << param3->printName() << rep4_  << param4->printName() << rep5_    << param5->printName() << rep6_  << endl;
}


