/***************************************************************************
                          ReportPrinter.cpp 
                             -------------------
This file is modified version of dgChannel.cpp  from Depression Glass library.
Copyright (c) 2002. Dimitry Kloper <dimka@cs.technion.ac.il> .

    begin                : Mon Oct 13 2003
    Modification         : by Alex Dribin
    email                : alexliza@netvision.net.il
***************************************************************************/


/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "ReportPrinter.h"
ReportPrinter::ReportPrinter() : ostream(0)
{
  buffer_ = new DGD::channelbuf();
  buffer_->pubsetbuf( NULL, 1000 );
  dummy_buffer_.pubsetbuf( NULL, 1000 );
  m_is_open = true;
  init(&dummy_buffer_);
}



ReportPrinter::ReportPrinter(const char * filename) : ostream(new DGD::channelbuf()) , out_(filename)
{
  buffer_ = dynamic_cast<DGD::channelbuf *>(ostream::rdbuf());
  buffer_->pubsetbuf( NULL, 1000 );
  buffer_->assoc(&out_);
  m_is_open = true;
  init(buffer_);
}



ReportPrinter::ReportPrinter(const string & filename): ostream(0), out_(filename.c_str())
{
//  buffer_ = dynamic_cast<DGD::channelbuf *>(ostream::rdbuf());
  buffer_ = new DGD::channelbuf();
  buffer_->pubsetbuf( NULL, 1000 );
  dummy_buffer_.pubsetbuf( NULL, 1000 );
  m_is_open = true;
  init(&dummy_buffer_);
}

ReportPrinter::~ReportPrinter()
{
	close();
  delete buffer_;
}


/**
 * Open and effectively initialize the ReportPrinter. This method calls
 * std::ostream::init() with pointer to buffer_. 
 */
void ReportPrinter::open(const char * filename)
{
   m_is_open = true;
   buffer_->assoc(&out_);
   out_.open(filename);
   init( buffer_ );
}

void ReportPrinter::open(const string & filename)
{
   m_is_open = true;
   buffer_->assoc(&out_);
   out_.open(filename.c_str());
   init( buffer_ );
}

/**
 * Close filestream in the ReportPrinter.
 */
void ReportPrinter::close() {
   m_is_open = false;
   init(&dummy_buffer_);
   out_.close();
}

/**
 * Return ReportPrinter open/close status.
 */
bool ReportPrinter::is_open() const {
   return m_is_open;
}


DGD::channelbuf* ReportPrinter::rdbuf() const {
   return (m_is_open? (DGD::channelbuf*)buffer_ : NULL );
}

/**
 * Return the ReportPrinter buffer. This method allows direct access to the
 * ReportPrinter buffer. See DGD::channelbuf for more info. @note This
 * method must be used carefully since direct access to the ReportPrinter
 * buffer can cause unexpected results.
 */
DGD::channelbuf& ReportPrinter::rdbuf() {
   return *buffer_;
}

/**
 * Change indentation step. @note This method does not affect the
 * indentation level itself.
 * @see channelbuf::indent_step(unsigned int)
 */
void ReportPrinter::indent_step( unsigned int step ) {
   buffer_->indent_step( step );
}

/**
 * Return indentation step.
 * @see channelbuf::indent_step() const
 */
unsigned int ReportPrinter::indent_step() const {
   return buffer_->indent_step();
}

/**
 * Increment indentation level by indentation step.
 * @see channelbuf::incr_indent()
 */
void ReportPrinter::incr_indent() {
   buffer_->incr_indent();
}

/**
 * Decrement indentation level by indentation step.
 * @see channelbuf::decr_indent()
 */
void ReportPrinter::decr_indent() {
   buffer_->decr_indent();
}

/**
 * Change indentation level. @note The parameter need not to be
 * multiple of the indentation level.
 * @see channelbuf::indent(unsigned int)
 */
void ReportPrinter::indent( unsigned int val ) {
   buffer_->indent( val );
}

/**
 * Return indentation level.
 * @see channelbuf::indent() const
 */
unsigned int ReportPrinter::indent() const {
   return buffer_->indent();
}

/**
 * Change minimum line width. The effective indentation level is
 * determined as
 * @verbatim
 * elevel = min( indentation level, max_width()-min_width() );
 * @endverbatim
 * @see channelbuf::min_width(unsigned int)
 */
void ReportPrinter::min_width( unsigned int width ) {
   buffer_->min_width( width );
}

/**
 * Return minimum line width.
 * @see channelbuf::min_width() const
 */
unsigned int ReportPrinter::min_width() const {
   return buffer_->min_width();
}

/**
 * Change maximum line width.
 * @see channelbuf::max_width(unsigned int)
 */
void ReportPrinter::max_width( unsigned int width ) {
   buffer_->max_width( width );
}

/**
 * Return maximum line width.
 * @see channelbuf::max_width() const
 */
unsigned int ReportPrinter::max_width() const {
   return buffer_->max_width();
}

/**
 * Disable or enable character wrapping.
 * @see channelbuf::wrap(bool)
 */
void ReportPrinter::wrap( bool allow ) {
   buffer_->wrap( allow );
}

/**
 * Return character wrapping flag.
 * @see channelbuf::wrap() const
 */
bool ReportPrinter::wrap() const {
   return buffer_->wrap();
}

/**
 * Disable or enable word wrapping.
 * @see channelbuf::word_wrap(bool)
 */
void ReportPrinter::word_wrap( bool allow ) {
   buffer_->word_wrap( allow );
}

/**
 * Return character wrapping flag.
 * @see channelbuf::word_wrap() const
 */
bool ReportPrinter::word_wrap() const {
   return buffer_->word_wrap();

}

/**
 * Change space characters. This method accepts a null-terminated
 * character string. Each character in the string will be considering
 * like a space between words during word wrapping.
 * @see channelbuf::space_chars(const char*)
 */
void ReportPrinter::space_chars(const char* spc ) {
   buffer_->space_chars( spc );
}

///**
// * Return space characters.
// * @see channelbuf::space_chars() const
// */
//std::string ReportPrinter::space_chars() const {
//   return buffer_->space_chars();
//}

void ReportPrinter::setPrefix(char * prefix)
{
  buffer_->setPrefix(prefix);
}



void ReportPrinter::setPrefix(string & prefix)
{
  buffer_->setPrefix(prefix.c_str());
}



