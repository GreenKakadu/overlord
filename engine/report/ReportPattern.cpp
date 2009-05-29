/***************************************************************************
                          ReportPattern.cpp  -  description
                             -------------------
    begin                : Wed Jan 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "ReportPattern.h"
#include "AbstractData.h"
#include "FileParser.h"
vector<ReportPattern *> * reportPatternsRegistry = 0;
//vector<ReportPattern *> ReportPattern::reportPatternsRegistry;
const string ReportPattern::keyword = "TEXT_PATTERN";

ReportPattern::ReportPattern(const string & keytag)
{
// cout << "ReportPattern "<< keytag << " created.\n";

  keytag_ = keytag;
  if(reportPatternsRegistry == 0)
  {
    reportPatternsRegistry = new vector<ReportPattern *>;
  }
  reportPatternsRegistry->push_back(this);
//  for (vector<ReportPattern *>::iterator iter = reportPatternsRegistry->begin();
//             iter != reportPatternsRegistry->end(); ++iter)
//             {
//               cout << "         Current report pattern |"<<(*iter)->getKeytag()<<"|\n";
//             }

}



STATUS ReportPattern::initialize(const string & fileName)
{
//  cout << "Initialization of ReportPatterns started from file "<< fileName << " .\n";
  STATUS status = OK;
  FileParser * parser = new FileParser(fileName);
	if (parser->status != OK)
			{
				status = IO_ERROR;
				return status;
			}
	status = OK;
  string tag;
  ReportPattern * currentPattern;
  do //Find class keyword  definition
    {
      	parser->getLine();
    	  if (parser->matchKeyword (keyword.c_str()) )
       	{
          tag = parser->getWord();
          if(tag.empty())
            continue;
          currentPattern = 0;
          for (vector<ReportPattern *>::iterator iter = reportPatternsRegistry->begin();
               iter != reportPatternsRegistry->end(); ++iter)
               {
//                 cout << "         Checking report pattern |"<<(*iter)->getKeytag()<<"|\n";
                 if ((*iter)->getKeytag() == tag)
                   currentPattern = *iter;
               }
          if(currentPattern)
          {
            currentPattern->rep1_ = parser->getParameter();
            currentPattern->rep2_ = parser->getParameter();
            currentPattern->rep3_ = parser->getParameter();
            currentPattern->rep4_ = parser->getParameter();
            currentPattern->rep5_ = parser->getParameter();
            currentPattern->rep6_ = parser->getParameter();
          }
          else
          {
            cout << "Can't find report pattern with keytag ["<<tag<<"].\n";
          }
        }
    } while (!  parser->eof() );
  delete parser;
  return status;
}



void ReportPattern::printReport (ReportPrinter &out)
{
	out << rep1_ << endl;
}

void ReportPattern::printReport (ReportPrinter &out, AbstractData * param1)
{
	out << rep1_ << param1->print() << rep2_ << endl;
}


void ReportPattern::printReport (ReportPrinter &out, AbstractData * param1, AbstractData * param2)
{
	out << rep1_ << param1->print() << rep2_ << param2->print() << rep3_  << endl;
}


void ReportPattern::printReport (ReportPrinter &out, AbstractData * param1, AbstractData * param2, AbstractData * param3)
{
	out << rep1_ << param1->print() << rep2_ << param2->print() << rep3_  << param3->print() << rep4_  << endl;
}

void ReportPattern::printReport (ReportPrinter &out, AbstractData * param1, AbstractData * param2, AbstractData * param3, AbstractData * param4)
{
	out << rep1_ << param1->print() << rep2_ << param2->print() << rep3_  << param3->print() << rep4_  << param4->print() << rep5_  << endl;
}

void ReportPattern::printReport (ReportPrinter &out, AbstractData * param1, AbstractData * param2, AbstractData * param3, AbstractData * param4, AbstractData * param5)
{
	out << rep1_ << param1->print() << rep2_ << param2->print() << rep3_  << param3->print() << rep4_  << param4->print() << rep5_    << param5->print() << rep6_  << endl;
}


