/***************************************************************************
                          EvenConflict.cpp 
                             -------------------
    begin                : Tue Apr 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "EvenConflict.h"
#include "BasicCompetitiveRequest.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
extern int Roll_1Dx(int n);
void EvenConflict::resolve(vector  <BasicCompetitiveRequest *> & currentList)
{
 vector  <BasicCompetitiveRequest *>::iterator iter;
 RationalNumber totalRequestValue = 0;
 RationalNumber totalAvailableValue;
 RationalNumber ratio;
 BasicCompetitiveRequest * typicalRequest;
 // Calculate total request.
// cout << "Resolving EvenConflict\n";
 typicalRequest = *(currentList.begin());
 if(typicalRequest == 0)
  {
    cout << "Conflict has no requests\n";
  return;
  }
  for(iter = currentList.begin() ; iter != currentList.end();iter++)
  {
    totalRequestValue += (*iter)->getValue(); 
//    cout << "Request for "<< (*iter)->getValue()<< " " <<(*iter)->getType()->print() <<endl;

  }
 // compare with available amount
  totalAvailableValue = typicalRequest->getTotalAvailableValue();
//    cout << "Total "<< typicalRequest->getType()->print() <<" available: "<< totalAvailableValue <<" \n";

  if( totalRequestValue >  totalAvailableValue)
  {
    ratio = totalAvailableValue/ totalRequestValue;
    RationalNumber currentAnswer;
    for(iter = currentList.begin() ; iter != currentList.end();iter++)
    {
      currentAnswer = ((*iter)->getValue() *  ratio);
      (*iter)->setValue(currentAnswer);
//      (*iter)->answerRequest( currentAnswer);
      totalAvailableValue -= currentAnswer;
    }
  // random redistribution of what's left
  if(totalAvailableValue > 0 )
        cout << "   ... and the residue is "<< totalAvailableValue<<endl;
//    while(totalAvailableValue)
//    {
//      i = Roll_1Dx(currentList.size());
//      currentAnswer = currentList[i]->getValue() ;
//      currentList[i]->setValue(currentAnswer +1);
////       currentList[Roll_1Dx(currentList.size())]->answerRequest(1);
//       totalAvailableValue --;
//    }  
  }

  for(iter = currentList.begin() ; iter != currentList.end();iter++)
    {
      RationalNumber answer((*iter)->getValue());
//cout << "EvenConflict::resolve "<< (*iter)->print()<<endl;
      (*iter)->answerRequest(answer);
    }
}



RationalNumber EvenConflict::getTotalAvailableValue(ItemRule * item) const
{
  if(item)
    return location_->getAvailableResource(item);
  else
      return 0;  
}
