/***************************************************************************
                          ReportPattern.h  -  description
                             -------------------
    begin                : Wed Jan 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef REPORTER_H
#define REPORTER_H
#include <string>
#include <vector>
#include <iostream>
using namespace std;
class AbstractData;


class ReportPattern {
public:
	ReportPattern(){}
	ReportPattern(const char *rep1);
	ReportPattern(const char *rep1,const  char *rep2);
	ReportPattern(const char *rep1,const  char *rep2,const  char *rep3);
	ReportPattern(const char *rep1, const char *rep2, const char *rep3, const char *rep4);
	ReportPattern(const char *rep1, const char *rep2, const char *rep3, const char *rep4, const char *rep5);
	ReportPattern(const char *rep1, const char *rep2, const char *rep3, const char *rep4, const char *rep5, const char *rep6);
	void printReport (ostream &out);
	void printReport (ostream &out, AbstractData * param1);
	void printReport (ostream &out, AbstractData * param1, AbstractData * param2);
	void printReport (ostream &out, AbstractData * param1, AbstractData * param2, AbstractData * param3);
	void printReport (ostream &out, AbstractData * param1, AbstractData * param2, AbstractData * param3, AbstractData * param4);
	void printReport (ostream &out, AbstractData * param1, AbstractData * param2, AbstractData * param3, AbstractData * param4, AbstractData * param5);

	protected:
	~ReportPattern(){}
	string rep1_;
	string rep2_;
	string rep3_;
	string rep4_;
	string rep5_;
	string rep6_;
	private:
};

#endif


