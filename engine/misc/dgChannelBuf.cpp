//
// $Id$
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// This file is part of Depression Glass library.
//
// Copyright (c) 2002. Dimitry Kloper <dimka@cs.technion.ac.il> . 
//
// dgChannelBuf.cpp -- implementation for dgChannelBuf.h
//

/**
 * @file dgChannelBuf.cpp
 *
 * implementation of DGD::channelbuf
 */

#include "dgChannelBuf.h"

namespace DGD {

/** 
 * Sets or resets channelbuf internal buffer.
 * @param ptr - if NULL causes automatic buffer allocation.
 * @param size - if zero DefaultBufferSize is used. Otherwise
 * defines the allocated buffer size.
 */
channelbuf::Parent* 
channelbuf::setbuf( char_type* ptr , std::streamsize size)
{
   if( pbase() != NULL )
   {
      delete pbase();
   }
   
   char_type* buffer = NULL;
   std::streamsize bsize = 0;

   if( size == 0 ) {
      bsize = DefaultBufferSize;
   } else {
      bsize = size;
   }

   if( ptr == NULL ) {
      buffer = new char_type[ bsize ];
   } else {
      buffer = ptr;
   }

   setp( buffer, buffer + bsize );
   return this;
}

/** 
 * Synchronizes (i.e. flushes) the buffer.  
 * @return always 0
 */

int channelbuf::sync() {
   overflow( traits_type::eof() );

   if( pptr() != pbase() ) {
      propagate( pbase(), pptr() );
      setp( pbase(), epptr() );
      m_word_pos = pbase();
      m_column += pptr()-pbase();
   }

   post_process();

   return 0;
}

/**
 * Called when there is no write position.  This function overrides
 * the same function in std::streambuf. This function is called
 * every time there is no enough space in the internal buffer. 
 *
 * Once called, this function processes the content of the buffer,
 * makes formatting decisions and then propagates the formatted output
 * to associated streams.
 *
 * @param ch - character which failed to be pushed into stream. could
 * be set to char_traits::eof value to indicate empty value.
 *
 * @return always 0
 * @see DGD::channel
 */
channelbuf::int_type channelbuf::overflow( int_type ch ) {   
   const char_type* pos = pbase();
   bool run_flag = true;

   do {
        unsigned int indent = std::min( m_indent, m_max_width - m_min_width );

        if( m_column < indent && pos < pptr() ) // add Indent
          {
            propagate( prefix_, prefix_ + prefixLength_); // added by Alex Dribin
            propagate( ' ', indent - m_column - prefixLength_); // added by Alex Dribin
//            propagate( ' ', indent - m_column );
	          m_column = indent;	 
          }

        const char_type* nearest = NULL;
        const char_type* space_pos = find_one_of( pos, pptr()-pos, 
						m_spaces.c_str() );
        const char_type* nl_pos    = find_one_of( pos, pptr()-pos, "\r\n" );
      
        bool space_nearest = (        
	          ( space_pos != NULL ) &&
	          ( nl_pos == NULL || space_pos < nl_pos ) );
        
        if( space_nearest ) nearest = space_pos;  // nearest space
      
        bool nl_nearest = ( 
			   ( nl_pos != NULL ) &&
			   ( space_pos == NULL || nl_pos < space_pos ) );
        
        if( nl_nearest ) nearest = nl_pos;    // nearest new line
      
        if( nearest != NULL )
          {
	    if( m_column + (nearest-pos+1) < m_max_width )  //no wrapping
              {
		propagate( pos, nearest+1 );
		if( nl_nearest )  // New line
                  {
		    m_column = 0;
		    m_line++;
		  }
                else
                  {
		    m_word_pos = nearest+1;
		    m_column += nearest-pos+1 + ((*nearest=='\t')?7:0);
		  }
		pos = nearest + (is_dos_eol( nearest )?2:1);
	      }
	    
	    else if( m_column + (nearest-pos+1) == m_max_width )
	      {
		propagate( pos, nearest );
		if( m_wrap )
		  {	       
		    pos = nearest;
		    propagate( '\n', 1 );
                    // Temporary Modification by Alex Dribin
                    // skip space at the beginning of new line
		    if(*pos == ' ')
		      {
			pos++;
		      }
                    // End of Temporary modification
		    m_column = 0;
		    m_line++;
		  } 
		else 
		  {
		    if( nl_nearest ) 
		      {
			propagate( '\n', 1 );
			m_column = 0;
			m_line++;
		      } 
		    else 
		      {
			m_column += nearest-pos+1 + ((*nearest=='\t')?7:0);
		      }
		    pos = nearest + (is_dos_eol( nearest )?2:1);
		  }
		m_word_pos = pos;
	      } 
	    else
	      { // m_column + (nearest-pos+1) > m_max_width
		if( m_wrap )
		  {
		    if( m_column >= m_max_width )
		      {
			propagate( '\n', 1 );
			m_column = 0;
			m_line++;
		      }
		    else
		      if( m_word_wrap )
			{
			  unsigned int prop_size =
			    std::min( (unsigned)(m_word_pos-pos), 
				      m_max_width-m_column );
			  propagate( pos, pos + prop_size );
			  propagate( '\n', 1 );
			  pos += prop_size;
			  m_column = 0;
			  m_line++;
			  m_word_pos = nearest+1;
			}
		      else
			{
			  propagate( pos, pos + m_max_width-m_column );
			  propagate( '\n', 1 );
			  pos += m_max_width-m_column;
			  m_column = 0;
			  m_line++;
			  m_word_pos = nearest+1;
	                }
	          }
		
		else
		  {
		    propagate( pos, nearest+1 );
		    if( nl_nearest )
		      {
			m_column = 0;
			m_line++;
		      }
		    else
		      {
			m_column += nearest-pos+1 + ((*nearest=='\t')?7:0);
		      }
		    m_word_pos = nearest+1;
		    pos = nearest + (is_dos_eol( nearest )?2:1);
		  }
	      }
	  } 

	else // Nearest == NULL
	  {
	    unsigned int remaining_len = pptr()-pos;
	    if( remaining_len < (unsigned)(epptr()-pbase()) ) 
	      {
		if( remaining_len > 0 ) 
		  {
		    if( pos > pbase() )
		      traits_type::move( pbase(), pos, remaining_len );
		    setp( pbase(), epptr() );
		    pbump( remaining_len );
		  } 
		else 
		  {
		    setp( pbase(), epptr() );
		  }
		m_word_pos = pbase();
		if( ch != traits_type::eof() )
		  sputc( ch );
		run_flag = false; // break the main loop

	      } 
	    else 
	      {
		if( m_column >= m_max_width ) 
		  {
		    propagate( '\n', 1 );
		    m_column = 0;
		    m_line++;
		  } 
		else 
		  if( m_wrap ) 
		    {
		      propagate( pos, pos + (m_max_width - m_column) );
		      propagate( '\n', 1 );
		      pos += m_max_width - m_column;
		      m_column = 0;
		      m_line++;
		    } 
		  else 
		    {
		      propagate( pos, pos + remaining_len );
		      m_column += remaining_len;
		      pos += remaining_len;
		    }
		
		m_word_pos = pbase();
	      }	 
	  }
	
   } while( run_flag );

   post_process();

   return 0;
}
 
/**
 * Propagate character string to associated stream. 
 * @param begin - pointer to beginning of propagated string
 * @param end   - pointer to the end of the propagated string (one
 * character after the end!)
 */
void channelbuf::propagate( const char_type* begin, const char_type* end ) {
   unsigned long length = end-begin;
   m_bytes += length;
   output_->write( begin, length );
}

/**
 * Propagate given character to associated stream. 
 * @param ch - character to propagate
 * @param size - number of duplications.
 */
void channelbuf::propagate( const char_type ch, std::streamsize size ) {
   m_bytes += size;
      for( int x = 0; x < size; ++x )
        output_ ->write( &ch, 1 );
}

/**
 * This is a virtual callback. This function is called every time overflow() or
 * sync() finished their work, so the channelbuf is in consistent state. 
 * Subclasses can overload this function in order to add functionality.
 *
 * @see overflow()
 * @see sync()
 */
void channelbuf::post_process() {
}
 
/**
 * Default constructor 
 */
channelbuf::channelbuf() :
   Parent(),
   m_line(0),
   m_column(0),
   m_indent(0),
   m_indent_step( DefaultIndentStep ),
   m_min_width( DefaultMinWidth ),
   m_max_width( DefaultMaxWidth ),
   m_wrap( true ),
   m_word_wrap( true ),
   m_word_pos(NULL),
   prefix_(0), // added by Alex Dribin
   prefixLength_(0), // added by Alex Dribin
   m_spaces( " \t" ),
   m_bytes(0) {
}

/**
 * Destructor. Note that channelbuf tries to sync itself before being 
 * destructed.
 */
channelbuf::~channelbuf() {
   sync();
}

/**
 * Associate given stream.
 */
void channelbuf::assoc( std::ostream* dgs ) {
  output_ = dgs;
}


/**
 * Return current channel line
 */
unsigned int channelbuf::line() const {
   return m_line;
}

/**
 * Return current channel column
 */
unsigned int channelbuf::column() const {
   return m_column;
}

/**
 * Change indentation step. @note sync() is called before the value is
 * actually changed. So this operation affects only future input.
 * @see channel::indent_step(unsigned int)
 */
void channelbuf::indent_step( unsigned int step ) {
   sync();
   m_indent_step = step;
}

/**
 * Return indentation step value.
 * @see channel::indent_step() const
 */
unsigned int channelbuf::indent_step() const {
   return m_indent_step;
}

/**
 * Increment indentation level by value of indentation step. @note
 * sync() is called before the value is actually changed. So this
 * operation affects only future input.
 * @see channel::incr_indent()
 */
void channelbuf::incr_indent() {
   sync();
   m_indent += m_indent_step;
}

/**
 * Decrement indentation level by value of indentation step. @note
 * sync() is called before the value is actually changed. So this
 * operation affects only future input.
 * @see channel::decr::indent()
 */
void channelbuf::decr_indent() {
   sync();
   m_indent -= m_indent_step;
}

/**
 * Change indentation level directly. @note sync() is called before
 * the value is actually changed. So this operation affects only
 * future input.
 * @see channel::indent(const unsigned int)
 */
void channelbuf::indent( const unsigned int val ) {
   sync();
   m_indent = val;
}

/**
 * Return indentation level value.
 * @see channel::indent() const
 */
unsigned int channelbuf::indent() const {
   return m_indent;
}

/**
 * Change minimum line width. @note sync() is called before
 * the value is actually changed. So this operation affects only
 * future input.
 * @see channel::min_width(unsigned int)
 */
void channelbuf::min_width( unsigned int width ) {
   sync();
   m_min_width = width;
}


/**
 * Return minimum line width value.
 * @see channel::min_width() const
 */
unsigned int channelbuf::min_width() const {
   return m_min_width;
}

/**
 * Change maximum line width. @note sync() is called before
 * the value is actually changed. So this operation affects only
 * future input.
 * @see channel::max_width(unsigned int)
 */

void channelbuf::max_width( unsigned int width ) {
   sync();
   m_max_width = width;
}

/**
 * Return maximum line width value.
 * @see channel::max_width() const
 */
unsigned int channelbuf::max_width() const {
   return m_max_width;
}

/**

 * Set character wrapping. @note sync() is called before
 * the value is actually changed. So this operation affects only
 * future input.
 * @see channel::wrap(bool)
 */
void channelbuf::wrap( bool allow ) {
   sync();
   m_wrap = allow;
}

/**
 * Return character wrapping state.
 * @see channel::wrap() const
 */
bool channelbuf::wrap() const {
   return m_wrap;
}


/**
 * Set word wrapping. @note sync() is called before
 * the value is actually changed. So this operation affects only
 * future input.
 * @see channel::word_wrap(bool)
 */
void channelbuf::word_wrap( bool allow ) {
   sync();
   m_word_wrap = allow;
}

/**
 * Return word wrapping state.
 * @see channel::word_wrap() const
 */
bool channelbuf::word_wrap() const {
   return m_word_wrap;
}


/**
 * Set space character set. @note sync() is called before
 * the value is actually changed. So this operation affects only
 * future input.
 * @see channel::space_chars(const char*)
 */

void channelbuf::space_chars(const char* spc ) {
   sync();
   m_spaces = spc;
}

/**
 * Return space characters set.
 * @see channel::space_chars() const
 */
std::string channelbuf::space_chars() const {
   return m_spaces;
}

/**
 * Return current channel line and column.
 */
channelbuf::position_type channelbuf::position() const {
   return position_type( m_line, m_column );
}

/**
 * Return amount of bytes written to the output stream
 */
unsigned long channelbuf::bytes_written() const {
   return m_bytes;
}

/**
 * strchr() equivalent. Given string s with length n, finds a first
 * occurrence of any character found in null-terminated string c_set.
 */
channelbuf::char_type* channelbuf::find_one_of( const char_type* s, 
						const unsigned int n,
						const char_type* c_set ) const
{
   const char_type* res = NULL;

   while( *c_set != '\0' ) {
      const char_type* local_res = traits_type::find( s, n, *c_set );
      if( res == NULL )
	 res = local_res;
      else if( local_res != NULL )
	 res = std::min( res, local_res );
      c_set++;
   }
   
   return (char_type*)res;
}

bool channelbuf::is_dos_eol( const char_type* p ) const {
   if( (*p == '\n' && *(p+1) == '\r') || (*p == '\r' && *(p+1) == '\n') )
      return true;
   return false;
}

/** prefix vill be inserted into beginning of each line
 * indent increased by preffix length.
 * setPrefix(0) will cancel prefix insertion.
 *  added by Alex Dribin 
 */
void channelbuf::setPrefix(const char * prefix)
{
   unsigned int prefixLength = 0;
   if(prefix != 0)
           prefixLength = strlen(prefix);
   
   if(prefixLength > m_max_width - m_min_width)
      std::cerr << "Prefix is too long (" << prefixLength << ")\n";
   else
   { 
    sync();
    m_indent += prefixLength - prefixLength_;
    prefix_ = prefix;
    prefixLength_ = prefixLength;
   }
  
}



/**
 * @page tutor_channelbuf DGD Basics - channelbuf
 *
 * DGD::channelbuf is a basic class in the library. In fact, DGD is
 * started from the single class. Channelbuf can be used as
 * stand-alone std::streambuf for formatting output. Lets consider the
 * example:
 * @include stand_alone_channelbuf.cpp
 * For this example the formatter is declared as a local variable. In
 * practice this is usually not so good idea because std::streambuf is
 * usually registered in some std::ostream. The channelbuf constructor
 * does not allocate any memory. So line
 * @skipline pubsetbuf
 * allocates buffer of 100 bytes in size. The first NULL parameter
 * will cause internal memory allocation, while non-NULL first
 * parameter will be treated as a pointer to the already allocated
 * buffer.
 * 
 * Note that formatter makes no \c physical output, it just makes
 * formatting. To achieve the physical output we must associate
 * channelbuf with physical stream, for example std::cout:
 * @skipline assoc
 *
 * Now, when the formatter is defined and associated with standard
 * output we can make some output. But channelbuf can't do output
 * itself. So we need an output stream:
 * @skipline ostream
 * The next line will produce the following:
 * @code
 * Hello! This is actally very very very very very very very very very very
 * very very very very very very very very long line!
 * @endcode
 *
 * Note how the formatter split the line in two. By default it has
 * word wrapping turned on and maximum line length 79. You can easily
 * change the defaults. Read the next chapter to learn that.
 */

/**
 * @page tutor_formatting DGD Basics - Formatting
 * 
 * Channelbuf makes no physical output. Instead, it makes some
 * formatting on what it gets and then pipes already formatted output
 * to the associated physical stream. In this chapter we will learn
 * how the formatting is done. Consider the following example:
 * @include formatting.cpp
 * Note that in this example DGD::channel is used instead of directly
 * using DGD::channelbuf. DGD::channel is a output stream which hides
 * complexities of using channelbuf and encapsulates all of its
 * functionality.
 * Note also that DGD::channel always has name and must be explicitly
 * opened and associated with physical stream:
 * @skipline assoc
 * @line open
 *
 *
 * @section wrapping Wrapping
 *
 * The very basic feature of the DGD formatting is a maximum line
 * length and wrapping. By default word wrapping is turned on and
 * maximum line length is 79, so the following line:
 * @skip formatter
 * @until endl
 * will produce the following output:
 * @code
 * Hello! This is actally very very very very very very very very ver very very
 * very very very very very very very long line!longer then 79!
 * @endcode
 * The maximum line width can be easily changed. For example the line:
 * @skipline max_width
 * and same output will produce the following:
 * @code
 * Hello! This is another very very very
 * very very very very very very very very
 * very very very very very very very
 * long line!longer then 40!
 * @endcode
 * Note how DGD breaks the line. The line is split at spaces between
 * words, not at the exact 40x positions. This is called word
 * wrapping. The word wrapping can be turned off:
 * @skipline word_wrap
 * The output will look like this:
 * @code
 * Hello! This is another very very very ve
 * ry very very very very very very very ve
 * ry very very very very very very long li
 * ne!longer then 40!
 * @endcode
 * If you don't want the lines being wrapped you can turn the wrapping
 * off by using DGD::channel::wrap(bool) method. 
 *

 * You can tell DGD what characters are considered white spaces to
 * change word wrapping behavior:
 * @skip space_chars
 * @until endl
 * The DGD::space_chars() method receives a string containing a set of
 * characters which will be considered as white spaces. The output
 * will look like this:
 * @code
 * a;b
 * ;c;
 * d;e
 * ;f;
 * g;h
 * @endcode
 * Note that here word wrapping is enabled and maximum width is
 * defined to 4, but there are only 3 characters in a line. This is
 * because the '\\n' in the end of each line is considered as a
 * separate character.
 *
 * @section indentation Indentation 
 *
 * DGD output can be indented. This means that you can define amount
 * of blanks filled before any actual line of output. Consider the
 * following code:
 * @skip max_width
 * @until }
 * @skipline }
 * The following output will be produced:
 * @code
 * word word word word word word word word word word word word
 * word word word word
 *        word word word word word word word word word word
 *        word word word word word
 *                word word word word word word word word
 *                word word word word word word word
 *        word word word word word word word word word word
 *        word word word word word
 * word word word word word word word word word word word word
 *  word word word
 * @endcode
 * Note that the filled blanks are considered as part of the line so
 * overall line width does not exceed the 60 characters limit. 
 * 
 * This code demonstrates indentation level
 * mechanism. DGD::channel::incr_indent() and
 * DGD::channel::decr_indent() methods increment/decrement the
 * current indentation level. The actual number of filled blanks is
 * calculated at the beginning of each line (thus, the incr_indent() and
 * decr_indent() take no effect until beginning of the next line). The
 * indentation level value is multiplied by indentation step value and
 * the calculated amount of blanks is filled to the output. The
 * indentation step can be controlled by
 * DGD::channel::indent_step(unsigned) method. The default indentation
 * step value is 8.
 * 
 * You can set the exact amount of indentation spaces using
 * DGD::channel::indent(unsigned) method.
 *
 * Note that theoretically the indentation level could exceed the
 * maximum line width. In this case DGD considers minimum line width
 * property of the DGD::channel or DGD::channelbuf. Minimum line width
 * tells DGD to skip indentation and keep the actual output line at least
 * "min_width" characters wide. Minimum width is controlled by
 * DGD::channel::min_width(unsigned) method. The default value for
 * minimum width is 20.
 */

}; // end of namespace DGD


// 
// Local Variables:
// compile-command: "make dgChannelBuf.obj"
// End:
//



