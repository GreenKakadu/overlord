/***************************************************************************
                          RationalNumber.cpp
                             -------------------
    begin                : Sun May 11 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "RationalNumber.h"
#include <iostream>
#include "LineParser.h"
using namespace std;


RationalNumber::RationalNumber(int numenator, int denominator)
{
  numenator_ = numenator;
  denominator_ = denominator;
//  assert(denominator_);
  if(denominator_ == 0) cout << " DIVIDE BY 0 ERROR in rational number \n";
  cancel_();
}



RationalNumber::RationalNumber(const RationalNumber & rational) : AbstractData()
{
  numenator_ = rational.getNumenator();
  denominator_ = rational.getDenominator();
}



void RationalNumber::cancel_()
{
  int greatestCommonDenominator = findGCD(numenator_, denominator_);
  numenator_ = numenator_ / greatestCommonDenominator;
  denominator_ = denominator_ / greatestCommonDenominator;
}



int RationalNumber::findGCD(int x, int y)
{
        if (x == 0)
            return y;
        else
          return findGCD(y%x, x);
}



RationalNumber::~RationalNumber(){
}

//********************************************************************
//
//   Operators
//
//********************************************************************

const RationalNumber operator + (const RationalNumber& leftArgument, const RationalNumber& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
    return RationalNumber(leftArgument.numenator_ + rightArgument.numenator_, leftArgument.denominator_);
  else
  {
    return RationalNumber(leftArgument.numenator_ * rightArgument.denominator_ +
                    rightArgument.numenator_ * leftArgument.denominator_,
                    leftArgument.denominator_ * rightArgument.denominator_);
 }
}



const RationalNumber operator - (const RationalNumber& leftArgument, const RationalNumber& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
    return RationalNumber(leftArgument.numenator_ - rightArgument.numenator_, leftArgument.denominator_);
  else
  {
    return RationalNumber(leftArgument.numenator_ * rightArgument.denominator_ -
                    rightArgument.numenator_ * leftArgument.denominator_,
                    leftArgument.denominator_ * rightArgument.denominator_);
 }
}



const RationalNumber operator * (const RationalNumber& leftArgument, const RationalNumber& rightArgument)
{
    return RationalNumber(leftArgument.numenator_ * rightArgument.numenator_,
                    leftArgument.denominator_ * rightArgument.denominator_);
}



const RationalNumber operator / (const RationalNumber& leftArgument, const RationalNumber& rightArgument)
{
    return RationalNumber(leftArgument.numenator_ * rightArgument.denominator_,
                    leftArgument.denominator_ * rightArgument.numenator_);
}



const RationalNumber operator * (const RationalNumber& leftArgument, const int& rightArgument)
{
 return RationalNumber(leftArgument.numenator_ * rightArgument, leftArgument.denominator_);
}



const RationalNumber operator * (const int& leftArgument, const RationalNumber& rightArgument)
{
  return RationalNumber(leftArgument * rightArgument.numenator_, rightArgument.denominator_);
}



RationalNumber operator += ( RationalNumber& leftArgument, const RationalNumber& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
       leftArgument.numenator_ += rightArgument.numenator_;
  else
  {
    leftArgument.numenator_   = leftArgument.numenator_ * rightArgument.denominator_ +
                              rightArgument.numenator_ * leftArgument.denominator_;
    leftArgument.denominator_ = leftArgument.denominator_ * rightArgument.denominator_;
  }
 leftArgument.cancel_();
 return  leftArgument;
}



RationalNumber operator -= ( RationalNumber& leftArgument, const RationalNumber& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
       leftArgument.numenator_ -= rightArgument.numenator_;
  else
  {
    leftArgument.numenator_   = leftArgument.numenator_ * rightArgument.denominator_ -
                              rightArgument.numenator_ * leftArgument.denominator_;
    leftArgument.denominator_ = leftArgument.denominator_ * rightArgument.denominator_;
  }
 leftArgument.cancel_();
 return  leftArgument;
}



     bool operator == (const RationalNumber& leftArgument, const RationalNumber& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
       return (leftArgument.numenator_ == rightArgument.numenator_);
  else
    return false;
}



     bool operator == (const RationalNumber& leftArgument, const int& rightArgument)
{
  return ((leftArgument.denominator_ == 1) && (leftArgument.numenator_ == rightArgument));
}



     bool operator != (const RationalNumber& leftArgument, const RationalNumber& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
       return (leftArgument.numenator_ != rightArgument.numenator_);
  else
    return true;
}



     bool operator >= (const RationalNumber& leftArgument, const RationalNumber& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
       return (leftArgument.numenator_ >= rightArgument.numenator_);
  else
    return (leftArgument.numenator_ * rightArgument.denominator_ >=
            rightArgument.numenator_ * leftArgument.denominator_);
}



     bool operator <= (const RationalNumber& leftArgument, const RationalNumber& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
       return (leftArgument.numenator_ <= rightArgument.numenator_);
  else
    return (leftArgument.numenator_ * rightArgument.denominator_ <=
            rightArgument.numenator_ * leftArgument.denominator_);
}



     bool operator  > (const RationalNumber& leftArgument, const RationalNumber& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
       return (leftArgument.numenator_ > rightArgument.numenator_);
  else
    return (leftArgument.numenator_ * rightArgument.denominator_ >
            rightArgument.numenator_ * leftArgument.denominator_);
}



     bool operator  < (const RationalNumber& leftArgument, const RationalNumber& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
       return (leftArgument.numenator_ < rightArgument.numenator_);
  else
    return (leftArgument.numenator_ * rightArgument.denominator_ <
            rightArgument.numenator_ * leftArgument.denominator_);
}



//********************************************************************
//
//   Input/output
//
//********************************************************************
ostream & operator << ( ostream &out,  RationalNumber rightArgument)
{
  rightArgument.save(out);	return out;
}



string RationalNumber::print()
{
    char buffer[30];
 if (!(numenator_ % denominator_))
    sprintf(buffer,"%d",getValue());// May use hand-made convertor itoa
 else
    sprintf(buffer,"%d %d/%d",getValue(),numenator_ % denominator_,denominator_);// May use hand-made convertor itoa
      return string(buffer);

}

bool RationalNumber::load(Parser * parser)
{
 int num = parser->getInteger();
  if(parser->matchChar('('))
  {
    int residue = parser->getInteger();
    parser->getWord();
    int denom = parser->getInteger();
    if(denom == 0)
    {
      numenator_ = num;
      denominator_ = 1;
      return false;
    }
    parser->matchChar(')');
    numenator_ = num * denom +  residue;
    denominator_ =  denom;
    return true;
  }
  else
  {
    numenator_ = num;
    denominator_ = 1;
    return true;
  }
}



/** Returns integer part */
 int RationalNumber::getValue() const
{
 return numenator_ / denominator_;
}

bool RationalNumber::isInteger()
{
 return  (numenator_ % denominator_ == 0);
}



void RationalNumber::save(ostream & out)
{
 out << getValue();
 if(numenator_ == 0)
  return;
 if (numenator_ % denominator_)
  out << "("<<numenator_ % denominator_<<" of "<< denominator_<<")";
}





int RationalNumber::roundDown() const
{
  return  numenator_/denominator_ ;
}



int RationalNumber::roundUp() const
{
  return  (numenator_ + denominator_ -1)/denominator_ ;
}



bool RationalNumber::isEmpty()
{
  return(!numenator_);
}
