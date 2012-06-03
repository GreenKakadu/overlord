/***************************************************************************
                          Global.cpp  -  Global variables
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 This file contains Overlord Engine definitions
 ***************************************************************************/
#include "Global.h"
#include "DataStorageHandler.h"



const int BLOCK_SIZE = 1024;

//const unsigned SCALE_FACTOR = 100;


bool testMode = false;

// special values
GameData  targetTypeSelf("SELF", &sampleGameData);

ProcessingMode   immediateOrders  (IMMEDIATE_ORDER);
ProcessingMode   requestOrders  (REQUEST_ORDER);
ProcessingMode   stackOrders  (STACK_ORDER);
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
	return tolower(static_cast<unsigned char>(c1))  < tolower(static_cast<unsigned char>(c2));
}


bool ciStringCompare(const std::string& s1,const std::string& s2)
{
  if(s1.size() != s2.size())
    return true;
  if (lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(),ciCharCompare))
  return true;
  if (lexicographical_compare(s2.begin(), s2.end(), s1.begin(), s1.end(),ciCharCompare))
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
///* Converts a long int to a string.
//   'out' must be long enough to take the largest number.
// */
//void longtostr(unsigned long u, char *out)
//{
//        char *p=out,*r;
//	/* Process the number, output to the string in reverse */
//        r = p;
//        do {
//		/* Turn the digit into a character */
//                *p = '0' + u%10;
//                u=u/10;
//                p++;
//        } while(u != 0);
//
//        *p=0;
//        p--;
//        /* Reverse The String. */
//        while(r < p) {
//                *p ^= *r ^= *p ^= *r; /* A Crazy Swap */
//                p--; r++;
//        }
//}
string longtostr(long in)
  {
    int    j = 0;
    unsigned long k = 1000000000;
    char str[12];

    if (in<0)
      {
        str[0] = '-';
        j = 1;
        in = -in;
      }
    if (in>=k)
      {
        str[j] = '0' + ((in / k) % 100);
        j++;
      }
    k /= 10;

    while (k>1)
      {
        if (in>=k)
          {
            str[j] = '0' + ((in % (k*10)) / k);
            j++;
          }
        k /= 10;
      };

    str[j] = '0' + (in % 10);
    j++;
    str[j] = 0;
    return string(str);
  };



// string longtostr(long in)
// {
//         char buffer[12];
//         char *p=buffer, *r;
// 				unsigned long u;
// 	/* Deal with the sign */
//         if (in < 0) {
//                 *p = '-'; p++;
//                 u = (unsigned) in * -1;
//         } else
//                 u = (unsigned) in;
// 
// 
// 	/* Process the number, output to the string in reverse */
//         r = p;
//         do {
// 		/* Turn the digit into a character */
//                 *p = '0' + u%10;
//                 u=u/10;
//                 p++;
//         } while(u != 0);
// 
//         *p=0;
//         p--;
//         /* Reverse The String. */
// cout << "----"<< in<<"---before reverse------->["<<buffer<<"]";
//         while(r < p) {
//                 *p ^= *r ^= *p ^= *r; /* A Crazy Swap */
//                 p--; r++;
//         }
//  cout << "-------after reverse------->["<<buffer<<"]"<<endl;
//        return string(buffer);
// }
// Simple random roll in the interval from 0 to n-1

int Roll_1Dx(int n)
{
  double r = rand();
  r = r/(RAND_MAX+1.0);
  r = r * n;
  //cout<< "    Roll 1D"<<n<<" = "<<(int)r<<endl;
  return (int)r;
 //return (int) ((double)n * rand()/(RAND_MAX+1.0));
}
#ifndef NOLRAND48 
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
#else
long unsigned seed31pmc =1;
int roll_1Dx(int n)
{

  long unsigned int hi, lo;
                                   
  lo = 16807 * (seed31pmc & 0xFFFF);
  hi = 16807 * (seed31pmc >> 16);
       

  lo += (hi & 0x7FFF) << 16;                                  
  lo += hi >> 15;                 

                                   
  if (lo > 0x7FFFFFFF) lo -= 0x7FFFFFFF;
  seed31pmc = (long)lo;
  return ( seed31pmc %n );       
}
#endif

bool isFileExist(string filename)
{
    ifstream * stream = new ifstream(filename.c_str());
    if (*stream)
    {
        stream->close();
        delete stream;
        return true;
    }
    else
    {
        return false;
    }

}
// Makes random name from capitall letters
string makeRandomName(int length)
{
    char buff[length];
//    char n;
    for (int i=0; i<length;++i)
    {
 //      n = 65 + Roll_1Dx(26);
       buff[i] = 65 + Roll_1Dx(26);

    }
    return string(&(buff[0]),length);
}


void notifyAndExit(string filename)
{
   char  buff[120];
#ifdef WINGETCWD
                cout<<"Can't find file "<< filename <<". Working directory is "<<  _getcwd(buff,119) <<endl;
#else
                cout<<"Can't find file "<< filename <<". Working directory is "<<  getcwd(buff,119) <<endl;
#endif
                cout << "Exiting..." << endl;
		exit(1);
}
#include "Element.h"
#include "ItemElement.h"
#include "ToolUseElement.h"
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
             BasicToolUseElement * BasicToolUseElement::headOfFreeList;
             Element2<RaceRule, int > * Element2<RaceRule, int >::headOfFreeList;
             BasicInventoryElement * BasicInventoryElement::headOfFreeList;
             BasicReportElement  * BasicReportElement::headOfFreeList;
             BasicStanceElement * BasicStanceElement::headOfFreeList;
//             StanceElement * StanceElement::headOfFreeList;
             BasicResourceElement * BasicResourceElement::headOfFreeList;
             BasicMovementElement * BasicMovementElement::headOfFreeList;
             BasicSkillUseElement * BasicSkillUseElement::headOfFreeList;
             BasicConstructionWorksElement * BasicConstructionWorksElement::headOfFreeList;
             SwapRequestElement * SwapRequestElement::headOfFreeList;
             BasicWeatherElement * BasicWeatherElement::headOfFreeList;
//             BasicEventElement * BasicEventElement::headOfFreeList;
#else
template  <> BasicSkillElement * BasicSkillElement::headOfFreeList = 0;
template  <> BasicEnchantmentElement * BasicEnchantmentElement::headOfFreeList = 0;
template  <> BasicItemElement * BasicItemElement::headOfFreeList = 0;
template  <> BasicToolUseElement * BasicToolUseElement::headOfFreeList = 0;
template  <> BasicRaceElement * BasicRaceElement::headOfFreeList = 0;
template  <> BasicInventoryElement * BasicInventoryElement::headOfFreeList = 0;
template  <> BasicReportElement  * BasicReportElement::headOfFreeList = 0;
template  <> BasicStanceElement * BasicStanceElement::headOfFreeList = 0;
template  <> BasicResourceElement * BasicResourceElement::headOfFreeList = 0;
template  <> BasicMovementElement * BasicMovementElement::headOfFreeList = 0;
template  <> BasicSkillUseElement * BasicSkillUseElement::headOfFreeList = 0;
template  <> BasicConstructionWorksElement * BasicConstructionWorksElement::headOfFreeList = 0;
template  <> SwapRequestElement * SwapRequestElement::headOfFreeList = 0;
template  <> BasicWeatherElement * BasicWeatherElement::headOfFreeList = 0;
//template  <> BasicEventElement * BasicEventElement::headOfFreeList = 0;
#endif

