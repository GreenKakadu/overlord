/***************************************************************************
                          Parser.h 
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef PARSER_H
#define PARSER_H
/* #include <string.h> */
#include <string>
#include "Symbols.h"
class Parser {
public: 
       explicit Parser ( const string &input );
       explicit Parser ( const char * input );
                Parser ();
       virtual        ~Parser();

       bool    matchKeyword       ( const char * keyword );
       bool    matchChar          (const char);
       bool     matchInteger();
       string  getParameter();
       string  getWord ();
       int     getInteger ();
       string  getText ();
       virtual void      getLine();
       virtual bool      eof();
       virtual void      setPosition         (unsigned long  position);
       virtual unsigned long        getPosition();
static const unsigned INTEGER_LENGTH = 6;
protected:
enum { MAX_LINE_LENGTH = 800}; // just using static const  may not work
char buffer_[MAX_LINE_LENGTH];
char * input_;
private:

};

#endif
