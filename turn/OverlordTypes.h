/***************************************************************************
                          OverlordTypes.h 
                             -------------------
    begin                : Wen Jul  3 16:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef OVERLORDTYPES_H
#define OVERLORDTYPES_H

enum status {OK = 0, ERROR = -1 };
typedef enum status STATUS;

enum order_status {
  SUCCESS = 0,
  FAILURE = 1,
  INVALID = 2
};
typedef enum order_status ORDER_STATUS;
typedef unsigned long UINT32;
typedef unsigned  UINT; // Size doesn't matter.  Select the most appropriate for processor.
#endif

