/***************************************************************************
                          RationalNumber.h
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

#ifndef SCALED_INTEGER_H
#define SCALED_INTEGER_H
#include <iostream>
#include "AbstractData.h"
using namespace std;
class Parser;


/**RationalNumber Number
  *@author Alex Dribin
  */

class RationalNumber :public AbstractData {
public: 
	RationalNumber(int numenator = 0, int denominator = 1);
	RationalNumber(const RationalNumber & rational);
	~RationalNumber();
  inline void clean(){ delete this;}
  friend const RationalNumber operator + (const RationalNumber& leftArgument, const RationalNumber& rightArgument);
  friend const RationalNumber operator - (const RationalNumber& leftArgument, const RationalNumber& rightArgument);
  friend const RationalNumber operator * (const RationalNumber& leftArgument, const RationalNumber& rightArgument);
  friend const RationalNumber operator / (const RationalNumber& leftArgument, const RationalNumber& rightArgument);
  friend const RationalNumber operator * (const RationalNumber& leftArgument, const int& rightArgument);
  friend const RationalNumber operator * (const int& leftArgument, const RationalNumber& rightArgument);
  friend       ostream& operator << ( ostream &out,  RationalNumber rightArgument);
  friend       RationalNumber operator += ( RationalNumber& leftArgument, const RationalNumber& rightArgument);
  friend       RationalNumber operator -= ( RationalNumber& leftArgument, const RationalNumber& rightArgument);
  friend      bool operator == (const RationalNumber& leftArgument, const RationalNumber& rightArgument);
  friend      bool operator == (const RationalNumber& leftArgument, const int& rightArgument);
  friend      bool operator != (const RationalNumber& leftArgument, const RationalNumber& rightArgument);
  friend      bool operator >= (const RationalNumber& leftArgument, const RationalNumber& rightArgument);
  friend      bool operator <= (const RationalNumber& leftArgument, const RationalNumber& rightArgument);
  friend      bool operator  > (const RationalNumber& leftArgument, const RationalNumber& rightArgument);
  friend      bool operator  < (const RationalNumber& leftArgument, const RationalNumber& rightArgument);
   int getValue() const;
  int roundDown() const;
  int roundUp() const;
  inline int getNumenator() const {return numenator_;}
  inline int getDenominator() const {return denominator_;}
  bool isInteger();
  string print();
  static int findGCD(int x, int y);
  void save(ostream & out);
  bool load(Parser * parser);
  bool isEmpty();
private:
  void cancel_();
  int numenator_;
  int denominator_;  
};
  ostream& operator << ( ostream &out,  RationalNumber rightArgument);

#endif
