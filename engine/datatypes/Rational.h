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
using namespace std;
class Parser;


/**Integer scaled by scale factor for keeping fractional parts
  *@author Alex Dribin
  */

class Rational {
public: 
	Rational(int numenator = 0, int denominator = 1);
	~Rational();
  static int scaleFactor;
  friend const Rational operator + (const Rational& leftArgument, const Rational& rightArgument);
  friend const Rational operator - (const Rational& leftArgument, const Rational& rightArgument);
  friend const Rational operator * (const Rational& leftArgument, const Rational& rightArgument);
  friend const Rational operator / (const Rational& leftArgument, const Rational& rightArgument);
  friend const Rational operator * (const Rational& leftArgument, const int& rightArgument);
  friend const Rational operator * (const int& leftArgument, const Rational& rightArgument);
  friend       ostream& operator << ( ostream &out,  Rational rightArgument);
  friend       Rational operator += ( Rational& leftArgument, const Rational& rightArgument);
  friend       Rational operator -= ( Rational& leftArgument, const Rational& rightArgument);
//   Rational& operator  = (const Rational& rightArgument);
//  friend const     bool operator == (const Rational& leftArgument, const Rational& rightArgument);
//  friend const     bool operator != (const Rational& leftArgument, const Rational& rightArgument);
//  friend const     bool operator >= (const Rational& leftArgument, const Rational& rightArgument);
//  friend const     bool operator <= (const Rational& leftArgument, const Rational& rightArgument);
  friend const     bool operator  > (const Rational& leftArgument, const Rational& rightArgument);
//  friend const     bool operator  < (const Rational& leftArgument, const Rational& rightArgument);
  const int getValue() const;
  int roundDown() const;
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

#endif
