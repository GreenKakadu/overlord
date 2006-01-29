/***************************************************************************
                          Global.cpp  -  Global variables
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 This file contains Overlord Engine definitions
 ***************************************************************************/
#include "Global.h"
#include "DataStorageHandler.h"


int currentDay = 1;
const int BLOCK_SIZE = 1024;
//const unsigned SCALE_FACTOR = 100;

GameConfig gameConfig;
RuleIndex ruleIndex;
bool testMode = false;

// special values
GameData  targetTypeSelf("SELF", &sampleGameData);

ProcessingMode   immediateOrders  (IMMEDIATE_ORDER);
ProcessingMode  stackOrders  (STACK_ORDER);
ProcessingMode 	 dayOrders  (DAY_LONG_ORDER);

ItemRule * cash;
ItemRule * food;

MovementVariety * walkingMode;
MovementVariety * swimingMode;
MovementVariety * flyingMode;
StanceVariety *  allied;

// Case-insensitive string comparison ( Scott Meyers)
#include <cctype>
#include <string>
bool ciCharCompare(char c1, char c2)
{
	return tolower(static_cast<unsigned char>(c1))  != tolower(static_cast<unsigned char>(c2));
}


bool ciStringCompare(const std::string& s1,const std::string& s2)
{
  if(s1.size() != s2.size())
    return true;
  if (lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(),ciCharCompare))
  return true;
	else
	return false;
}
bool ciStringCompareN(const std::string& s1,const std::string& s2, const int N)
{
  int i;
  for (i = 0; i< N ; i++)
  {
    if (ciCharCompare(s1[i], s2[i]))
    return true;
  }
	return false ;
}
/* Converts a long int to a string.
   'out' must be long enough to take the largest number.
 */
void longtostr(unsigned long u, char *out)
{
        char *p=out,*r;
	/* Process the number, output to the string in reverse */
        r = p;
        do {
		/* Turn the digit into a character */
                *p = '0' + u%10;
                u=u/10;
                p++;
        } while(u != 0);

        *p=0;
        p--;
        /* Reverse The String. */
        while(r < p) {
                *p ^= *r ^= *p ^= *r; /* A Crazy Swap */
                p--; r++;
        }
}



string longtostr(long in)
{
        char buffer[12];
        char *p=buffer, *r;
				unsigned long u;
	/* Deal with the sign */
        if (in < 0) {
                *p = '-'; p++;
                u = (unsigned) in * -1;
        } else
                u = (unsigned) in;


	/* Process the number, output to the string in reverse */
        r = p;
        do {
		/* Turn the digit into a character */
                *p = '0' + u%10;
                u=u/10;
                p++;
        } while(u != 0);

        *p=0;
        p--;
        /* Reverse The String. */
        while(r < p) {
                *p ^= *r ^= *p ^= *r; /* A Crazy Swap */
                p--; r++;
        }
        return string(buffer);
}
// Simple random roll in the interval from 0 to n-1

int Roll_1Dx(int n)
{
 return (int) ((double)n * rand()/(RAND_MAX+1.0));
}

/**
 ** ROLL_1DX
 **	Roll the very large dice.
 ** Correct for any disparities in lrand's distribution.
 ** The "dice" rolled is 0 to n-1.
 ** Code of Vincent Archer
 **/
int roll_1Dx(int n)
{
static int	old_n = 0;
static long	max;
long		rnd;
/*
 * Go
 */
	if (old_n != n) {
		old_n = n;
		max = n;
		while ((max & 0xFF000000) == 0)
			max <<= 1;
	}
	while (1) {
		rnd = lrand48() >> 6;
		if (rnd < max)
			break;
	}
	return rnd % n;
}

#include "Element.h"
#include "ItemElement.h"
#include "RaceElement.h"
#include "ReportElement.h"
#include "ResourceElement.h"
#include "MovementElement.h"
#include "ConstructionWorksElement.h"
#include "EnchantmentElement.h"
#include "SkillUseElement.h"
#include "WeatherElement.h"

// Template instantiation
//BonusElement Should not be instantiated because it is alreary instantiated as SkillElement (they have identical types)

// GCC versions earlier than 3.4 use template static member declaration syntax incompartible with later varsions
#ifdef GCC_OLD
             BasicSkillElement * BasicSkillElement::headOfFreeList;
             BasicEnchantmentElement * BasicEnchantmentElement::headOfFreeList;
             BasicItemElement * BasicItemElement::headOfFreeList;
             Element2<RaceRule, int > * Element2<RaceRule, int >::headOfFreeList;
             BasicInventoryElement * BasicInventoryElement::headOfFreeList;
             BasicReportElement  * BasicReportElement::headOfFreeList;
             StanceElement * StanceElement::headOfFreeList;
             BasicResourceElement * BasicResourceElement::headOfFreeList;
             BasicMovementElement * BasicMovementElement::headOfFreeList;
             BasicSkillUseElement * BasicSkillUseElement::headOfFreeList;
             BasicConstructionWorksElement * BasicConstructionWorksElement::headOfFreeList;
             SwapRequestElement * SwapRequestElement::headOfFreeList;
             BasicWeatherElement * BasicWeatherElement::headOfFreeList;
#else
template  <> BasicSkillElement * BasicSkillElement::headOfFreeList;
template  <> BasicItemElement * BasicItemElement::headOfFreeList;
template  <> BasicRaceElement * BasicRaceElement::headOfFreeList;
template  <> BasicInventoryElement * BasicInventoryElement::headOfFreeList;
template  <> BasicReportElement  * BasicReportElement::headOfFreeList;
template  <> StanceElement * StanceElement::headOfFreeList;
template  <> BasicResourceElement * BasicResourceElement::headOfFreeList;
template  <> BasicMovementElement * BasicMovementElement::headOfFreeList;
template  <> BasicSkillUseElement * BasicSkillUseElement::headOfFreeList;
template  <> BasicConstructionWorksElement * BasicConstructionWorksElement::headOfFreeList;
template  <> SwapRequestElement * SwapRequestElement::headOfFreeList;
template  <> BasicWeatherElement * BasicWeatherElement::headOfFreeList;
#endif

