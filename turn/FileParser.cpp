/***************************************************************************
                          FileParser.cpp  
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "FileParser.h"



FileParser::FileParser()
{
}

//=====================================================================
//
// FileParser class extends  interface of Parser enabling
// parsing from files and file streams
//=====================================================================
FileParser::FileParser (ifstream &input)
{
 streamInput_ = &input;
}


SYMBOLS_H
FileParser::FileParser (const char * filename)
{
 streamInput_= new ifstream(filename);
 assert(*streamInput_);
}



FileParser::FileParser (const string &filename)
{
 streamInput_= new ifstream(filename.c_str());
 assert(*streamInput_);
}



FileParser::~FileParser ()
{
  // For debugging.
  if (streamInput_->eof()) 
    streamInput_->clear(); 
  streamInput_->seekg(0); //  In order to know what was destructed
  streamInput_->getline (buffer_, MAX_LINE_LENGTH);
  input_ = buffer_;     
 cout << "Parser ["<< input_ <<"] Destroyed"<<endl;
 // End of debugging section


 streamInput_->close();
 delete streamInput_;
}
void FileParser::getLine()
{
   streamInput_->getline (buffer_, MAX_LINE_LENGTH);
   input_ = buffer_;
}

bool FileParser::eof()
{
 return streamInput_->eof();
}

void FileParser::setPosition ( unsigned long  position )
{
  if(streamInput_->eof()) // clear error flags if EOF was reached 
    streamInput_->clear(); 
  streamInput_->seekg ( position );

}
unsigned long  FileParser::getPosition()
{
  return streamInput_->tellg();
}
