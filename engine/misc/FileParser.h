/***************************************************************************
                          FileParser.h 
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef FILEPARSER_H
#define FILEPARSER_H
#include <iostream>
#include <fstream>
//#include "SystemTypes.h"
#include "OverlordTypes.h"
#include "Parser.h"
using namespace std;

class FileParser : public Parser {
public: 
       explicit FileParser ( ifstream &input );
       explicit FileParser ( const char * filename );
       explicit FileParser ( const string &filename );
                FileParser ();
               ~FileParser();

       void      getLine();
       bool      eof();
       void      setPosition         ( long int position);
       long int        getPosition();
		STATUS status;
protected:

private:
       ifstream *streamInput_;
 bool useAllocatedMemory_;
};

#endif
