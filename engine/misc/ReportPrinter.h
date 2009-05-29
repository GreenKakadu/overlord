/***************************************************************************
                          ReportPrinter.h
                             -------------------
This file is modified version of dgChannel.h  from Depression Glass library.
Copyright (c) 2002. Dimitry Kloper <dimka@cs.technion.ac.il> .

    begin                : Mon Oct 13 2003
    Modification         : by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or      *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

/**
  Prints report to file wrapping text at specified position
  */
#ifndef REPORT_PRINTER_H
#define REPORT_PRINTER_H


#include <iostream>
#include <fstream>
#include <string>
#define  _STLPORT
#include "dgChannelBuf.h"
using namespace std;

class ReportPrinter : public std::ostream {
public:
	ReportPrinter(const string & filename);
	ReportPrinter(const char * filename);
	ReportPrinter();
	~ReportPrinter();
  void               open(const string & filename);
  void               open(const char * filename);
  void               close();
  bool               is_open() const;
  DGD::channelbuf*      rdbuf() const;
  DGD::channelbuf&      rdbuf();

   void         indent_step( unsigned int step );
   unsigned int indent_step() const;
   void         incr_indent();
   void         decr_indent();
   void         indent( unsigned int val );
   unsigned int indent() const;

   void         min_width( unsigned int width );
   unsigned int min_width() const;
   void         max_width( unsigned int width );
   unsigned int max_width() const;

   void         wrap( bool allow );
   bool         wrap() const;
   void         word_wrap( bool allow );
   bool         word_wrap() const;

   void         space_chars(const char* spc = " \t");
//   std::string  space_chars() const;

   void setPrefix(char * prefix);
   void setPrefix(string & prefix);

   private:
      bool m_is_open;
      DGD::channelbuf  * buffer_;
      DGD::channelbuf   dummy_buffer_;
             ofstream out_;
};


#endif
