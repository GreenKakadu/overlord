/***************************************************************************
                          MarketConflict.cpp 
                             -------------------
    begin                : Wed Jun 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "Parser.h"
#include "MarketConflict.h"
#include "MarketRequest.h"
#include "LocationEntity.h"
#include "UnitEntity.h"
extern bool testMode;
MarketConflict::MarketConflict(LocationEntity * location): BasicConflict(location)
{
}
MarketConflict::~MarketConflict(){
}


void MarketConflict::addRequest(BasicCompetitiveRequest *request)
{
#ifdef TEST_MODE
  if(testMode) cout << "         Adding market request of "<<  request->print()<<endl;
#endif
  requests_.push_back(request);
}


//void MarketConflict::addBuyRequest(BasicCompetitiveRequest *request)
//{
//  buyRequests_.push_back(request);
//}
//
//
//void MarketConflict::addSellRequest(BasicCompetitiveRequest *request)
//{
//  sellRequests_.push_back(request);
//}

void MarketConflict::resolve (vector  <BasicCompetitiveRequest *> & currentList)
{
  vector  <MarketRequest *>  sellOffers;
  vector  <MarketRequest *>   buyOffers;
  vector  <BasicCompetitiveRequest *>::iterator iter;
  vector  <MarketRequest *>::iterator marketIter;
   int totalValue;
   int finalPrice;
#ifdef TEST_MODE
  if(testMode)   cout << endl<<"<><><><><><><><><><>   Resolving Market conflict at "<<location_->printName()<<endl;
#endif
//  Sort out sellOffers and buyOffers
  for(iter = currentList.begin(); iter != currentList.end(); ++iter)
  {
    MarketRequest * request = dynamic_cast<MarketRequest *>(*iter);
    if(request == 0)
    {
      cout << "ERROR: Market Conflict resolution detected wrong request type from "
            << (*iter)->getRequestingUnit()->printName()<<endl;
      continue;
      }
#ifdef TEST_MODE
  if(testMode) cout << "                       request for "<< request->print()<<endl;  
#endif
  
    if( request->isBuying()) 
      {
        buyOffers.push_back(request);
      }
    else if( request->isSelling())  
      {
         sellOffers.push_back(request);
      }
    else // request can't be processed 
      {
//       cout << "    ERROR                   request for "<< request->print()<<endl;
        continue;
      }
    }

  if(sellOffers.empty() || buyOffers.empty())
  {
    return; 
  }

    
  //  Sort  sellOffers and buyOffers by price if price equal by skill

  sort(buyOffers.begin(), buyOffers.end(), offerNotLessThan);
  sort(sellOffers.begin(), sellOffers.end(), offerNotLessThan);
  
  //  get sellMin and BuyMax
  MarketRequest * sellMin = sellOffers.front();
  MarketRequest * buyMax =  buyOffers.back();

  if (sellMin->getPrice() >  buyMax->getPrice())
  {
//        cout <<" No sales. Local price returns to default value \n";
//        cout <<" SellMin = "<< sellMin->getPrice() <<" BuyMax = "<< buyMax->getPrice()<<" \n";
    return;
  }
  else
  {  
  //  remove too low buy and too high sell  offers
    for(marketIter = buyOffers.begin(); marketIter != buyOffers.end(); )
    {
      if( (*marketIter)->getPrice() < sellMin->getPrice())
        {
        buyOffers.erase (marketIter);
        }
      else
          break;
    }

    for(marketIter = sellOffers.end(); marketIter != sellOffers.begin(); )
    {
      marketIter--;
      if( (*marketIter)->getPrice() > buyMax->getPrice())
        {
        sellOffers.erase (marketIter);
        }
      else
        break;
    }
    finalPrice = calculateFinalPrice(buyOffers,sellOffers);
//    cout << "Final price for "<< buyMax->getType()->printName()<<" is "<< finalPrice<<endl;
  }
  // Distribute goods, answering reports and calculating total (for selliing only)
   int buyAmount = mayBuyByPrice(buyOffers,finalPrice);
   int sellAmount = maySellByPrice(sellOffers,finalPrice);
   int finalAmount =  MIN(buyAmount,sellAmount);
   sellAmount = finalAmount;
   buyAmount =  finalAmount;
   int currentAmount;
  for(marketIter = sellOffers.begin(); marketIter != sellOffers.end(); ++marketIter)
  {
    currentAmount = (*marketIter)->getAmount();
    if (sellAmount > currentAmount)
     {
       (*marketIter)->answerMarketRequest(finalPrice,currentAmount);
       totalValue += finalPrice * currentAmount;
       sellAmount -= currentAmount;
     }
    else
        {
          currentAmount = sellAmount;
          (*marketIter)->answerMarketRequest(finalPrice,currentAmount);
          totalValue += finalPrice * currentAmount;
          sellAmount -= currentAmount;
          break;
        }
    }
  for(marketIter = buyOffers.end(); marketIter != buyOffers.begin(); )
  {
    marketIter--;
    currentAmount = (*marketIter)->getAmount();
    if (buyAmount > currentAmount)
     {
       (*marketIter)->answerMarketRequest(finalPrice,currentAmount);
       buyAmount -= currentAmount;
     }
    else
        {
          currentAmount = buyAmount;
          (*marketIter)->answerMarketRequest(finalPrice,currentAmount);
          buyAmount -= currentAmount;
          break;
        }
    }

// Here all request object deleted
     location_->updateTotalMarketValue(totalValue);

}

int MarketConflict::maySellByPrice(vector  <MarketRequest *> & sellOffers, int currentPrice)
{
  vector  <MarketRequest *>::iterator marketIter;
  int sellAmount = 0;
  for(marketIter = sellOffers.begin(); marketIter != sellOffers.end(); ++marketIter)
  {
    if( (*marketIter)->getPrice() <= currentPrice)
      sellAmount += (*marketIter)->getAmount();
    else
        break;
    }
    return sellAmount;
}

int MarketConflict::mayBuyByPrice(vector  <MarketRequest *> & buyOffers, int currentPrice)
{
  vector  <MarketRequest *>::iterator marketIter;
  int buyAmount = 0;
  for(marketIter = buyOffers.end(); marketIter != buyOffers.begin(); )
  {
    marketIter--;
    if( (*marketIter)->getPrice() >= currentPrice)
      buyAmount += (*marketIter)->getAmount();
    else
       break;
    }
    return buyAmount;
}



int MarketConflict::calculateFinalPrice(vector  <MarketRequest *> & buyOffers,
                           vector  <MarketRequest *> & sellOffers)
{
int currentPrice;
int buyAmount;
int sellAmount;

#ifdef TEST_MODE
  vector  <MarketRequest *>::iterator marketIter;
  if(testMode)
  {
    cout << "========Buy offers ========\n";
    for(marketIter = buyOffers.begin(); marketIter != buyOffers.end(); ++marketIter)
    {
       cout << (*marketIter)->print();
    }
    cout << "========Sell offers ========\n";
    for(marketIter = sellOffers.begin(); marketIter != sellOffers.end(); ++marketIter)
    {
       cout << (*marketIter)->print();
    }
  }
#endif

vector  <MarketRequest *>::iterator currentBuyIter = buyOffers.begin();

  if(currentBuyIter == buyOffers.end())
  {
        cout << "ERROR buy list is empty \n";
          return 0;
  }

   currentPrice = buyOffers.front()->getPrice();
// Find equilibrium price
    for(;;)
  {
  //  Price = lowest Buy price.
  //  Calculate total sell and buy amounts for currentPrice.
   buyAmount = mayBuyByPrice(buyOffers,currentPrice);
   sellAmount = maySellByPrice(sellOffers,currentPrice);
//   cout << "For price "<< currentPrice<<" may sell "<< sellAmount<< " and buy " << buyAmount<<endl;
  if(buyAmount <= sellAmount)
        {
          return currentPrice;
        }
   buyAmount = mayBuyByPrice(buyOffers,currentPrice +1);
   sellAmount = maySellByPrice(sellOffers,currentPrice +1);
  if(buyAmount <= sellAmount)
        {
          return currentPrice; // not currentPrice + 1 !
                 // Prices are integers so +1 stands here for any small grid step.
                 //  Idea is that if currentPrice is a maximal buyPrice of some offer
                 // rising price by any small number will set this offer out of game
                 // so we can assume that everything was sold by this price but higher
                 // price offers got an advantage
        }
  //  if still Buy > Sell   Price = next Buy price
  else
    {
      currentBuyIter++;
      if(currentBuyIter == buyOffers.end())
      {
        // ERROR
        cout << "ERROR in finding equilibrium price \n";
        return 0;
        }
      currentPrice = (*currentBuyIter)->getPrice();
    }

  }
  cout << "ERROR in finding equilibrium price (2)\n";
  return 0;
}

bool offerNotLessThan(const MarketRequest* request1,
                  const MarketRequest* request2)
{
     return request2->getPrice() >= request1->getPrice();
//   return request1.isGreaterThan(request2);
}
