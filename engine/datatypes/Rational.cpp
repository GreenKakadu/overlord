/***************************************************************************
                          Rational.cpp 
                             -------------------
    begin                : Sun May 11 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "Rational.h"
#include <iostream>
#include "Parser.h"
using namespace std;


Rational::Rational(int numenator, int denominator)
{
  numenator_ = numenator;
  denominator_ = denominator;
//  assert(denominator_);
  if(denominator_ == 0) cout << " DIVIDE BY 0 ERROR in rational number \n";
  cancel_();
}



Rational::Rational(const Rational & rational) : AbstractData()
{
  numenator_ = rational.getNumenator();
  denominator_ = rational.getDenominator();
}



void Rational::cancel_()
{
  int greatestCommonDenominator = findGCD(numenator_, denominator_);
  numenator_ = numenator_ / greatestCommonDenominator;
  denominator_ = denominator_ / greatestCommonDenominator;
}



int Rational::findGCD(int x, int y)
{
        if (x == 0)
            return y;
        else
          return findGCD(y%x, x);
}



Rational::~Rational(){
}

//********************************************************************
//
//   Operators
//
//********************************************************************

const Rational operator + (const Rational& leftArgument, const Rational& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
    return Rational(leftArgument.numenator_ + rightArgument.numenator_, leftArgument.denominator_);
  else
  {
    return Rational(leftArgument.numenator_ * rightArgument.denominator_ +
                    rightArgument.numenator_ * leftArgument.denominator_,
                    leftArgument.denominator_ * rightArgument.denominator_);
 }
}



const Rational operator - (const Rational& leftArgument, const Rational& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
    return Rational(leftArgument.numenator_ - rightArgument.numenator_, leftArgument.denominator_);
  else
  {
    return Rational(leftArgument.numenator_ * rightArgument.denominator_ -
                    rightArgument.numenator_ * leftArgument.denominator_,
                    leftArgument.denominator_ * rightArgument.denominator_);
 }
}



const Rational operator * (const Rational& leftArgument, const Rational& rightArgument)
{
    return Rational(leftArgument.numenator_ * rightArgument.numenator_,
                    leftArgument.denominator_ * rightArgument.denominator_);
}



const Rational operator / (const Rational& leftArgument, const Rational& rightArgument)
{
    return Rational(leftArgument.numenator_ * rightArgument.denominator_,
                    leftArgument.denominator_ * rightArgument.numenator_);
}



const Rational operator * (const Rational& leftArgument, const int& rightArgument)
{
 return Rational(leftArgument.numenator_ * rightArgument, leftArgument.denominator_);
}



const Rational operator * (const int& leftArgument, const Rational& rightArgument)
{
  return Rational(leftArgument * rightArgument.numenator_, rightArgument.denominator_);
}



Rational operator += ( Rational& leftArgument, const Rational& rightArgument)
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



Rational operator -= ( Rational& leftArgument, const Rational& rightArgument)
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


    
const     bool operator == (const Rational& leftArgument, const Rational& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
       return (leftArgument.numenator_ == rightArgument.numenator_);
  else
    return false;
}



const     bool operator == (const Rational& leftArgument, const int& rightArgument)
{
  return ((leftArgument.denominator_ == 1) && (leftArgument.numenator_ == rightArgument));
}



const     bool operator != (const Rational& leftArgument, const Rational& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
       return (leftArgument.numenator_ != rightArgument.numenator_);
  else
    return true;
}



const     bool operator >= (const Rational& leftArgument, const Rational& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
       return (leftArgument.numenator_ >= rightArgument.numenator_);
  else
    return (leftArgument.numenator_ * rightArgument.denominator_ >=
            rightArgument.numenator_ * leftArgument.denominator_);
}



const     bool operator <= (const Rational& leftArgument, const Rational& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
       return (leftArgument.numenator_ <= rightArgument.numenator_);
  else
    return (leftArgument.numenator_ * rightArgument.denominator_ <=
            rightArgument.numenator_ * leftArgument.denominator_);
}



const     bool operator  > (const Rational& leftArgument, const Rational& rightArgument)
{
  if(leftArgument.denominator_ == rightArgument.denominator_)
       return (leftArgument.numenator_ > rightArgument.numenator_);
  else
    return (leftArgument.numenator_ * rightArgument.denominator_ >
            rightArgument.numenator_ * leftArgument.denominator_);
}



const     bool operator  < (const Rational& leftArgument, const Rational& rightArgument)
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
ostream & operator << ( ostream &out,  Rational rightArgument)
{
  rightArgument.save(out);	return out;
}



string Rational::printName()
{
    char buffer[30];
 if (!(numenator_ % denominator_))
    sprintf(buffer,"%d",getValue());// May use hand-made convertor itoa
 else   
    sprintf(buffer,"%d %d/%d",getValue(),numenator_ % denominator_,denominator_);// May use hand-made convertor itoa
      return string(buffer);

}

bool Rational::load(Parser * parser)
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
const int Rational::getValue() const
{
 return numenator_ / denominator_;
}

bool Rational::isInteger()
{
 return  (numenator_ % denominator_ == 0); 
}



void Rational::save(ostream & out)
{
 out << getValue();
 if(numenator_ == 0)
  return;
 if (numenator_ % denominator_)
  out << "("<<numenator_ % denominator_<<" of "<< denominator_<<")";
}





int Rational::roundDown() const
{
  return  numenator_/denominator_ ;
}



int Rational::roundUp() const
{
  return  (numenator_ + denominator_ -1)/denominator_ ;
}



bool Rational::isEmpty()
{
  return(!numenator_);  
}
