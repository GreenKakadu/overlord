/***************************************************************************
                          ParameterHandler.h 
                    .
                             -------------------
    begin                : Thu Aug  8 12:16:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef PARAMETER_HANDLER_H
#define PARAMETER_HANDLER_H
#include <iostream>
#include <string>
#include "Parser.h"
class ParameterHandler {
    public:
                    ParameterHandler(Parser * parser);
                    ParameterHandler();
           virtual ~ParameterHandler();
           virtual void setParameter();
	   virtual void saveParameter(ostream &out);
	   virtual void getParameter() const;
    protected:
    private:

};
class IntegerParameterHandler : public ParameterHandler
{
    public:
           IntegerParameterHandler(Parser * parser);
           ~IntegerParameterHandler();
           void setParameter(int parameter);
	   void getParameter(int & parameter) const;
 	   void saveParameter(ostream &out);
   protected:
	   int parameter_;
    private:

};
class StringParameterHandler : public ParameterHandler
{
    public:
           StringParameterHandler(Parser * parser);
           ~StringParameterHandler();
           void setParameter(const string & parameter);
	   void getParameter(string &parameter) const;
 	   void saveParameter(ostream &out);
   protected:
	   string parameter_;
    private:

};

#endif
