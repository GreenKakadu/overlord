/***************************************************************************
                          FileParser.cpp
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "FileParser.h"
extern bool testMode;
extern string longtostr(long in);

FileParser::FileParser()
{
	useAllocatedMemory_ = false;
	 lineNumber_ = 0;
}

//=====================================================================
//
// FileParser class extends  interface of Parser enabling
// parsing from files and file streams
//
// Throws exeption BasicExeption fileError if can't open file.
//
//=====================================================================
FileParser::FileParser (ifstream &input)
{
 streamInput_ = &input;
 useAllocatedMemory_ = false;
 status = OK;
 lineNumber_ = 0;
 filename_ = "";
}



FileParser::FileParser (const char * filename)
{
 streamInput_= new ifstream(filename);
 if(*streamInput_)
       {
 			useAllocatedMemory_ = true;
 			status = OK;
		}
	else
		{
 			status = IO_ERROR;
			cerr << "Can't open file " << filename <<" "<<status<< endl;
		}
	lineNumber_ = 0;
        filename_ = string(filename);
}



FileParser::FileParser (const string &filename)
{
 streamInput_= new ifstream(filename.c_str());
if(*streamInput_)
       {
 			useAllocatedMemory_ = true;
      status = OK;
		}
	else
		{
 			status = IO_ERROR;
			cerr << "Can't open file " << filename << endl;
		}
	lineNumber_ = 0;
        filename_ = filename;
}


FileParser::~FileParser ()
{
  // For debugging.
#ifdef TEST_MODE
  if(testMode)
	{
 		 if (streamInput_->eof())
    	streamInput_->clear();
  		streamInput_->seekg(0); //  In order to know what was destructed
  		streamInput_->getline (buffer_, MAX_LINE_LENGTH);
  		input_ = buffer_;
 		cout << "Parser ["<< input_ <<"] Destroyed"<<endl;
	}
 #endif

 streamInput_->close();
 if(useAllocatedMemory_ )  delete streamInput_;
}
void FileParser::getLine()
{
   streamInput_->getline (buffer_, MAX_LINE_LENGTH,'\n');
   input_ = buffer_;
   lineNumber_++;
int i;
	i = (int)(streamInput_->gcount());
  if(i<2)
    return;
	if ( buffer_[i-2] == '\r')
		{
        	buffer_[i-2]  = '\0';
		}

}

bool FileParser::eof()
{
 return streamInput_->eof();
}


// This function will not update line number. It should be done separately.
void FileParser::setPosition ( long int  position )
{
  if(streamInput_->eof()) // clear error flags if EOF was reached
    streamInput_->clear();
  streamInput_->seekg ( position );

}

long int  FileParser::getPosition()
{
  return (long int)(streamInput_->tellg());
}



int       FileParser::getLineNumber()
{
 return lineNumber_;
}



void      FileParser::setLineNumber(int num)
{
	lineNumber_ = num;
}
string FileParser::report()
{
   return filename_ + ":"+ longtostr(getLineNumber());
}
