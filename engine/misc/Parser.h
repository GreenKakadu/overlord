/***************************************************************************
                          Parser.h
			Object performing string parsing
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef PARSER_H
#define PARSER_H
#include <string>
#include "OverlordTypes.h"
#include "Symbols.h"
#include "RationalNumber.h"
using namespace std;

class Parser {
public: 
       explicit Parser ( const string &input );
       explicit Parser ( const char * input );
                Parser ();
       virtual        ~Parser();

       bool     matchKeyword       ( const char * keyword );
       bool     matchChar          (const char);
       bool     matchInteger();
       bool     matchElement();
       string   getParameter();
       string   getWord ();
       string   matchWord ();
       int      getInteger ();
       RationalNumber getRationalNumber ();
       string   getText ();
       virtual void      getLine();
       virtual bool      eof();
       virtual void      setPosition         (long int  position);
       virtual long int        getPosition();
               void  rewind(int shift);
static const int INTEGER_LENGTH = 6;
STATUS status;
protected:
enum { MAX_LINE_LENGTH = 800}; // just using static const  may not work
char buffer_[MAX_LINE_LENGTH];
char * input_;
private:

};

#endif
