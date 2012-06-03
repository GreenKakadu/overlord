/* 
 * File:   TimeStamp.cpp
 * Author: alex
 * 
 * Created on June 16, 2010, 1:04 PM
 */

#include "TimeStamp.h"
#include "LineParser.h"
extern string longtostr(long u);
TimeStamp::TimeStamp()
{
    turn_ = 0;
    day_ = 0;
}

TimeStamp::TimeStamp(const TimeStamp& orig)
{
    turn_ = orig.turn_;
    day_ = orig.day_;

}

TimeStamp::~TimeStamp()
{
}



string TimeStamp::print()
{
   return string("Day ") + longtostr(day_) + " of turn " + longtostr(turn_);
}



void TimeStamp::save(ostream &out)
{
  out <<"TIMESTAMP"<<" "<<turn_ <<" "<<day_<<endl;
}



TimeStamp * TimeStamp::load(Parser *parser)
{
   int turn = parser->getInteger();
   int day =  parser->getInteger();
   return (new TimeStamp(turn,day));
}



void TimeStamp::init(Parser *parser)
{
   turn_ = parser->getInteger();
   day_ =  parser->getInteger();
}


ostream & operator << ( ostream &out, TimeStamp  rightArgument)
{
  rightArgument.save(out);	return out;
}

bool operator ==(const TimeStamp& leftArgument, const TimeStamp& rightArgument)
{
    if (leftArgument.turn_ == rightArgument.turn_)
        return (leftArgument.day_ == rightArgument.day_);
    else
        return false;
}

bool operator !=(const TimeStamp& leftArgument, const TimeStamp& rightArgument)
{
    if (leftArgument.turn_ == rightArgument.turn_)
        return (leftArgument.day_ != rightArgument.day_);
    else
        return true;
}

bool operator >=(const TimeStamp& leftArgument, const TimeStamp& rightArgument)
{
    if (leftArgument.turn_ == rightArgument.turn_)
        return (leftArgument.day_ >= rightArgument.day_);
    else
        return (leftArgument.turn_ >= rightArgument.turn_);
}

bool operator <=(const TimeStamp& leftArgument, const TimeStamp& rightArgument)
{
    if (leftArgument.turn_ == rightArgument.turn_)
        return (leftArgument.day_ <= rightArgument.day_);
    else
        return (leftArgument.turn_ <= rightArgument.turn_);

}

bool operator>(const TimeStamp& leftArgument, const TimeStamp& rightArgument)
{
    if (leftArgument.turn_ == rightArgument.turn_)
        return (leftArgument.day_ > rightArgument.day_);
    else
        return (leftArgument.turn_ > rightArgument.turn_);

}

bool operator<(const TimeStamp& leftArgument, const TimeStamp& rightArgument)
{
    if (leftArgument.turn_ == rightArgument.turn_)
        return (leftArgument.day_ < rightArgument.day_);
    else
        return (leftArgument.turn_ < rightArgument.turn_);

}

