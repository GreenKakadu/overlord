/***************************************************************************
                          FileParser.cpp  
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "FileParser.h"
extern bool testMode;


FileParser::FileParser()
{
	useAllocatedMemory_ = false;
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
			cout << "Can't open file " << filename << endl;
		}
}



FileParser::FileParser (const string &filename)
{
 streamInput_= new ifstream(filename.c_str());
if(*streamInput_)
       {
 			useAllocatedMemory_ = true;
		}
	else
		{
 			status = IO_ERROR;
			cout << "Can't open file " << filename << endl;
		}
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

int i;
	i = streamInput_->gcount();
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

void FileParser::setPosition ( long int  position )
{
  if(streamInput_->eof()) // clear error flags if EOF was reached 
    streamInput_->clear(); 
  streamInput_->seekg ( position );

}
long int  FileParser::getPosition()
{
  return streamInput_->tellg();
}
