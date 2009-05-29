/***************************************************************************
                          LocalRecruitOffer.cpp 
                             -------------------
    begin                : Wed Jul 2 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "LocalRecruitOffer.h"
#include "RaceRule.h"
#include "FactionEntity.h"
#include "RaceElement.h"

LocalRecruitOffer::LocalRecruitOffer(int amount, RaceRule * race, int price)
                      :LocalMarketRequest(amount,0, price, SELL)
{
  initialAmount_ = amount;
  value_ = 0;
  race_ = race;
}



LocalRecruitOffer::~LocalRecruitOffer(){
}



string LocalRecruitOffer::print()
{
  return  string("Local opportunity to recruit ") + longtostr(amount_) +
          " of " + race_->print() + " for " + longtostr(price_)  + " coins\n";
}


void  LocalRecruitOffer::produceFactionReport(FactionEntity * faction, ostream &out)
{
  out << RaceElement(race_,initialAmount_).print() << " at $"<< price_;
//  out << amount_ << " " << race_->print() << " at $"<< price_;
  faction->addKnowledge(race_);
}



AbstractData * LocalRecruitOffer::getType() const
{
  return race_;
}


void  LocalRecruitOffer::save(ostream &out)
{
    out << "RECRUIT "<<initialAmount_<<" "<<race_->getTag()<<" "<<price_<<endl;
}
void LocalRecruitOffer::answerMarketRequest(int price, int  amount)
{
  amount_ -= amount;
  value_ += amount * price;
// cout <<" report local recruiting opportunities: ("<<amount_<<")["<< print()<<"] left.\n";
}
