/* 
 * File:   TimeStamp.h
 * In-game time structure for use in PBeM
 * Author: alex
 *
 * Created on June 16, 2010, 1:04 PM
 */

#ifndef _TIMESTAMP_H
#define	_TIMESTAMP_H
#include <string>
#include <iostream>
class Parser;
using namespace std;

class TimeStamp {
public:
    TimeStamp();
    TimeStamp(int turn, int day){turn_ = turn; day_ = day;}
    TimeStamp(const TimeStamp& orig);
    virtual ~TimeStamp();
    void setTime(int turn, int day){turn_ = turn; day_ = day;}
    void setTime(const TimeStamp & time){turn_ = time.turn_; day_ = time.day_;}
    string print();
    void save(ostream &out);
    TimeStamp * load(Parser *parser);
    void init(Parser *parser);
  friend       ostream& operator << ( ostream &out,  TimeStamp rightArgument);
  friend      bool operator == (const TimeStamp& leftArgument, const TimeStamp& rightArgument);
  friend      bool operator != (const TimeStamp& leftArgument, const TimeStamp& rightArgument);
  friend      bool operator >= (const TimeStamp& leftArgument, const TimeStamp& rightArgument);
  friend      bool operator <= (const TimeStamp& leftArgument, const TimeStamp& rightArgument);
  friend      bool operator  > (const TimeStamp& leftArgument, const TimeStamp& rightArgument);
  friend      bool operator  < (const TimeStamp& leftArgument, const TimeStamp& rightArgument);
private:
    int turn_;
    int day_;

};

#endif	/* _TIMESTAMP_H */

