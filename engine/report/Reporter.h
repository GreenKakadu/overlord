/***************************************************************************
                          Reporter.h  -  description
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


class Reporter {
public: 
	Reporter(){}
	Reporter(const char *rep1);
	Reporter(const char *rep1,const  char *rep2);
	Reporter(const char *rep1,const  char *rep2,const  char *rep3);
	Reporter(const char *rep1, const char *rep2, const char *rep3, const char *rep4);
	Reporter(const char *rep1, const char *rep2, const char *rep3, const char *rep4, const char *rep5);
	Reporter(const char *rep1, const char *rep2, const char *rep3, const char *rep4, const char *rep5, const char *rep6);
	void print (ostream &out);
	void print (ostream &out, AbstractData * param1);
	void print (ostream &out, AbstractData * param1, AbstractData * param2);
	void print (ostream &out, AbstractData * param1, AbstractData * param2, AbstractData * param3);
	void print (ostream &out, AbstractData * param1, AbstractData * param2, AbstractData * param3, AbstractData * param4);
	void print (ostream &out, AbstractData * param1, AbstractData * param2, AbstractData * param3, AbstractData * param4, AbstractData * param5);

	protected:
	~Reporter(){}
	string rep1_;
	string rep2_;
	string rep3_;
	string rep4_;
	string rep5_;
	string rep6_;
	private:
};

#endif


