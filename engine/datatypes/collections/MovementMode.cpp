/***************************************************************************
                          MovementMode.cpp  -  description
                             -------------------
    begin                : Wed Jan 15 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "MovementMode.h"
#include "GameFacade.h"

//template <class T> MovementMode<T>::MovementMode()
//{
//// Provide that vector has size equal to number of movement modes
//	reserve(movements.size());
//}
//template <class T> MovementMode<T>::~MovementMode()
//{
//
//}
//template <class T> T&  MovementMode<T>::operator [] (const string &tag)
//{
// vector<T>::iterator iter;
// for (iter = begin(); iter != end(); iter++)
//   {
//      if ((*iter) ->getTag() == tag)
//       return  (*iter);
//
//   }
// cerr << "Error: Tag (" << tag  << ") was not found in Movement Rules \n";
// return 0 ;
//
//}
template <class T> int MovementMode<T>::getModeIndex(const MovementVariety * mode)
{
    int i;
    for( i = 0; i< gameFacade->movementModes.size();i++)
    {
       if(gameFacade->movementModes[i] == mode)
        return i;
    }
      return -1;
}
