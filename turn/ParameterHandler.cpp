/***************************************************************************
                          ParameterHandler.cpp 
                    .
                             -------------------
    begin                : Thu Aug  8 12:16:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "ParameterHandler.h"

ParameterHandler::ParameterHandler(Parser * parser)
{
}
ParameterHandler::ParameterHandler()
{
}
ParameterHandler::~ParameterHandler()
{
}
void
ParameterHandler::getParameter() const
{  
}

void
ParameterHandler::setParameter() 
{  
}

void
ParameterHandler::saveParameter(ostream &out) 
{  
}

/***************************************************************************/


IntegerParameterHandler::IntegerParameterHandler(Parser * parser)
{
  if(parser -> matchInteger())
    parameter_ = parser -> getInteger();
  else
    parameter_ = 0;
}
IntegerParameterHandler::~IntegerParameterHandler()
{

}
void
IntegerParameterHandler::setParameter(int parameter)
{
  parameter_ = parameter;
  
}
void
IntegerParameterHandler::getParameter(int & parameter) const
{
  parameter = parameter_;
  
}
void
IntegerParameterHandler::saveParameter(ostream &out)
{
  out << ' ' << parameter_;
  
}
/***************************************************************************/
StringParameterHandler::StringParameterHandler(Parser * parser)
{
  parameter_ = parser -> getParameter();
}
StringParameterHandler::~StringParameterHandler()
{

}
void
StringParameterHandler::setParameter(const string &  parameter)
{
  parameter_ = parameter;
  
}
void
StringParameterHandler::getParameter(string &  parameter) const
{
  parameter =  parameter_;
  
}
void
StringParameterHandler::saveParameter(ostream &out)
{
  out << ' ' << parameter_;
  
}
