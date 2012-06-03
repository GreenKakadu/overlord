/***************************************************************************
                          FileParser.h
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef FILEPARSER_H
#define FILEPARSER_H
#include <iostream>
#include <fstream>
#include "OverlordTypes.h"
#include "LineParser.h"
using namespace std;

class FileParser : public Parser {
public:
       explicit FileParser ( ifstream &input );
       explicit FileParser ( const char * filename );
       explicit FileParser ( const string &filename );
                FileParser ();
               ~FileParser();

       void      getLine();
       int       getLineNumber();
       void      setLineNumber(int num);
       bool      eof();
       void      setPosition         ( long int position);
       long int        getPosition();
       string report();
protected:

private:
       ifstream *streamInput_;
       bool useAllocatedMemory_;
       int lineNumber_;
       string filename_;
};

#endif
