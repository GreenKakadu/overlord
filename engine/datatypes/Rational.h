/***************************************************************************
                          Rational.h
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

#ifndef SCALED_INTEGER_H
#define SCALED_INTEGER_H
#include <iostream>
#include "AbstractData.h"
using namespace std;
class Parser;


/**Rational Number
  *@author Alex Dribin
  */

class Rational :public AbstractData {
public: 
	Rational(int numenator = 0, int denominator = 1);
	Rational(const Rational & rational);
	~Rational();
  inline void clean(){ delete this;}
  friend const Rational operator + (const Rational& leftArgument, const Rational& rightArgument);
  friend const Rational operator - (const Rational& leftArgument, const Rational& rightArgument);
  friend const Rational operator * (const Rational& leftArgument, const Rational& rightArgument);
  friend const Rational operator / (const Rational& leftArgument, const Rational& rightArgument);
  friend const Rational operator * (const Rational& leftArgument, const int& rightArgument);
  friend const Rational operator * (const int& leftArgument, const Rational& rightArgument);
  friend       ostream& operator << ( ostream &out,  Rational rightArgument);
  friend       Rational operator += ( Rational& leftArgument, const Rational& rightArgument);
  friend       Rational operator -= ( Rational& leftArgument, const Rational& rightArgument);
  friend const     bool operator == (const Rational& leftArgument, const Rational& rightArgument);
  friend const     bool operator == (const Rational& leftArgument, const int& rightArgument);
  friend const     bool operator != (const Rational& leftArgument, const Rational& rightArgument);
  friend const     bool operator >= (const Rational& leftArgument, const Rational& rightArgument);
  friend const     bool operator <= (const Rational& leftArgument, const Rational& rightArgument);
  friend const     bool operator  > (const Rational& leftArgument, const Rational& rightArgument);
  friend const     bool operator  < (const Rational& leftArgument, const Rational& rightArgument);
  const int getValue() const;
  int roundDown() const;
  int roundUp() const;
  inline int getNumenator() const {return numenator_;}
  inline int getDenominator() const {return denominator_;}
  bool isInteger();
  string printName();
  static int findGCD(int x, int y);
  void save(ostream & out);
  bool load(Parser * parser);
  bool isEmpty();
private:
  void cancel_();
  int numenator_;
  int denominator_;  
};
  ostream& operator << ( ostream &out,  Rational rightArgument);

#endif
