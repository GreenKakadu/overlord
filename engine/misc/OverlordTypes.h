/***************************************************************************
                          OverlordTypes.h 
                             -------------------
    begin                : Wen Jul  3 16:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef OVERLORD_TYPES_H
#define OVERLORD_TYPES_H

enum status {OK = 0, IO_ERROR = -1, UNDEFINED =1};
typedef enum status STATUS;

enum order_status {
  SUCCESS      = 0,
  FAILURE      = 1,
  IN_PROGRESS  = 2,
  SUSPENDED    = 3,
  INVALID      = 4
};
typedef enum order_status ORDER_STATUS;

enum learning_result {
  LEARNING_OK           = 0,
  MAX_LEVEL_FAILURE     = 1,
  CANNOT_STUDY_FAILURE  = 2,
  RACE_FAILURE          = 3,
  REQUIREMENT_FAILURE   = 4,
//  TEACHING_REQUIRED     = 5,
//  TEACHING_FAILURE      = 6,
  FOLLOWER_CANNOT_STUDY_LEVEL_FAILURE = 7,
  FOLLOWER_CANNOT_STUDY_SECOND_BASIC_SKILL_FAILURE = 8
                   };
typedef learning_result  LEARNING_RESULT;
//enum movement_result {
//  MOVEMENT_OK           = 0,
//  RACE_CANNOT_MOVE      = 1,
//  OVERLOAD              = 2
//                   };
//typedef movement_result  MOVEMENT_RESULT;
enum using_result {
  USING_OK           = 0,
  UNUSABLE           = 1,
  NO_RESOURCES       = 2
                   };
typedef using_result  USING_RESULT;
enum processing_state {
  NORMAL_STATE = 0,
  SUSPEND      = 1,
  RESUME       = 2
};
typedef enum  processing_state PROCESSING_STATE;

enum order_repetition_state {
  NONE      = 0,
  WAIT      = 1,
  ACTIVE    = 2
};
typedef enum  order_repetition_state ORDER_REPETITION_STATE;
#endif
