/***************************************************************************
                          BasicConflict.cpp 
                             -------------------
    begin                : Fri Apr 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "BasicConflict.h"
#include "BasicCompetitiveRequest.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
extern bool testMode;

BasicConflict::BasicConflict(LocationEntity * location)
{
  location_ = location;
}



BasicConflict::~BasicConflict(){
}



/** add new request without checks */
void BasicConflict::insertRequest(BasicCompetitiveRequest *request)
{
  requests_.push_back(request);
}



/** add new request if it was'n already added */
void BasicConflict::addRequest(BasicCompetitiveRequest *request)
{
 vector  <BasicCompetitiveRequest *>::iterator iter;
  UnitEntity * unit = request->getRequestingUnit();
  for(iter = requests_.begin() ; iter != requests_.end();iter++)
  {
    if( (*iter)->getRequestingUnit() == unit)
    {
//      int old = (*iter)->getValue();
//      int add =  request->getValue();
//      int newv = old + request->getValue();
//      cout << "Summarizing requests for "<< (*iter)->getRequestingUnit()->print()<< " was "<< old<< " after addition of" <<add<<" it is "<<newv <<endl;
//      cout << "Summarizing requests for "<< (*iter)->getRequestingUnit()->print()<< " was "<< old<< " after addition " <<newv <<endl;
//      (*iter)->setValue( newv);
//      cout << "Check requests: it is " << (*iter)->getValue() <<endl;
      
      (*iter)->setValue((*iter)->getValue() + request->getValue() );
      return;
    }
  }
  requests_.push_back(request);
}

//void BasicConflict::processRequest(UnitEntity * )
//{
//
//}

void BasicConflict::process()
{
 vector  <BasicCompetitiveRequest *>::iterator iter;
 vector  <BasicCompetitiveRequest *> currentList;
 BasicCompetitiveRequest * currentRequest;
// cout << "<><><><><><><><><><>   Processing Basic conflict "<<endl;
 while (!requests_.empty())
 {
 // Extract all identical requests into temporary array, removing them from  requests_
  currentRequest = *(requests_.begin());

//  cout<< "-- root request --"<< currentRequest->print()<<endl;
  for(iter = requests_.begin() ; iter != requests_.end();)
  {
    if(!(*iter)->isValid())
    {
       iter = requests_.erase(iter);
       continue;
    }
//  cout << "--==-- comparing with --" << (*iter)->print()<<endl;
    
    if(currentRequest->isEqualTo(*iter) )
      {
//  cout << "--==--==-- yes --" << (*iter)->print()<<endl;
       currentList.push_back(*iter);
       iter = requests_.erase(iter);
      }
     else
      iter++; 
  }
  if(!currentList.empty())
    {
      resolve(currentList);
    }
  else
  {
    //cerr << location_->print()<<" Request list is empty\n"; 
  }
// Here all request object deleted and  currentList cleaned
  for(iter = currentList.begin() ; iter != currentList.end();++iter)
  {
    (*iter)->free();
  }
 currentList.clear();
 }
}



void BasicConflict::resolve (vector  <BasicCompetitiveRequest *> & currentList)
{
// vector  <BasicCompetitiveRequest *>::iterator iter;
// Rational totalRequestValue = 0;
// Rational totalAvailableValue;
// Rational ratio;
// // Calculate total request.
//
//  for(iter = currentList.begin() ; iter != currentList.end();iter++)
//  {
//    totalRequestValue += (*iter)->getValue();
//  }
// // compare with available amount
//  totalAvailableValue = (*(currentList.begin()))->getTotalAvailableValue();
//  if( totalRequestValue <=  totalAvailableValue)
//  {
//
//  }
//  else
//  {
//    ratio = (100 *  totalAvailableValue) / totalRequestValue;
//  }
 // calculate partials for all participants
 // perform operation
}
