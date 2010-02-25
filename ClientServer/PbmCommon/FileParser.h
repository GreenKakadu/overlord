/****************************************************************************
                         FileParser.h
                         Object performing file  parsing 
                         -------------------
     begin                : Fri Nov  9 19:24:42 IST 2001
     copyright            : (C) 2001 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/


#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <iostream>
#include <fstream>
#define COMMENT_SYMBOL '#'

using namespace std;

class Parser
{
public:
  explicit Parser( const string & input );
  explicit Parser( const char * input );
  Parser();
  virtual ~Parser();

  bool matchKeyword( const char * keyword );
  bool matchChar( const char );
  bool matchInteger();
  bool matchElement();
  string getParameter();
  string getWord();
  string matchWord();
  int getInteger();
  long int getLongInteger();
  string getText();
  virtual void getLine();
  virtual bool eof();
  virtual void setPosition( long int position );
  virtual long int getPosition();
  void rewind( int shift );
  static const int INTEGER_LENGTH = 7;
  static const int LONG_INTEGER_LENGTH = 11;
  int status;

protected:
  enum
  {
    MAX_LINE_LENGTH = 800
  }; // just using static const  may not work

  char buffer_[MAX_LINE_LENGTH];
  char * input_;
private:

};
#endif
#ifndef FILEPARSER_H
#define FILEPARSER_H

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
protected:

private:
       ifstream *streamInput_;
       bool useAllocatedMemory_;
       int lineNumber_;
};

#endif
