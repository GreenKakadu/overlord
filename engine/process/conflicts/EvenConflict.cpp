/***************************************************************************
                          EvenConflict.cpp 
                             -------------------
    begin                : Tue Apr 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
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
void EvenConflict::resolve(vector  <BasicCompetitiveRequest *> & currentList)
{
 vector  <BasicCompetitiveRequest *>::iterator iter;
 Rational totalRequestValue = 0;
 Rational totalAvailableValue;
 Rational ratio;
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
//    cout << "Request for "<< (*iter)->getValue()<< " " <<(*iter)->getType()->printName() <<endl;

  }
 // compare with available amount
  totalAvailableValue = typicalRequest->getTotalAvailableValue();
//    cout << "Total "<< typicalRequest->getType()->printName() <<" available: "<< totalAvailableValue <<" \n";

  if( totalRequestValue >  totalAvailableValue)
  {
    ratio = totalAvailableValue/ totalRequestValue;
    Rational currentAnswer;
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
//      i = rand() % currentList.size();
//      currentAnswer = currentList[i]->getValue() ;
//      currentList[i]->setValue(currentAnswer +1);
////       currentList[rand() % currentList.size()]->answerRequest(1);
//       totalAvailableValue --;
//    }  
  }

  for(iter = currentList.begin() ; iter != currentList.end();iter++)
    {
      Rational answer((*iter)->getValue());
      (*iter)->answerRequest(answer);
    }
}



Rational EvenConflict::getTotalAvailableValue(ItemRule * item) const
{
  if(item)
    return location_->getAvailableResource(item);
  else
      return 0;  
}
