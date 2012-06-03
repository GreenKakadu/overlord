/* 
 * File:   EventElement.cpp
 * Author: alex
 * 
 * Created on April 5, 2011, 5:35 PM
 */

#include "EventElement.h"
// GCC versions earlier than 3.4 use template static member
// declaration syntax incompartible with later varsions
#ifdef GCC_OLD
             BasicEventElement * BasicEventElement::headOfFreeList;
#else
template  <> BasicEventElement * BasicEventElement::headOfFreeList = 0;
#endif



