/***************************************************************************
                          AbstractData.cpp 
                             -------------------
    begin                : Tue Mar 11 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "AbstractData.h"

 vector <AbstractData *> AbstractData::aPrint()
{
  vector <AbstractData *> v; 
  v.push_back(this); 
  return v;
}