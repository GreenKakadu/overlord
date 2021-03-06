/***************************************************************************
                          MovementMode.h  -  description
                             -------------------
    begin                : Wed Jan 15 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef MOVEMENT_MODE_H
#define MOVEMENT_MODE_H
#include <vector>
#include <string>
#include "GameFacade.h"
#include "MovementVariety.h"
#include "VarietiesCollection.h"


template <class T> class MovementMode : public vector < T>  {
public:
	MovementMode(){ MovementMode<T>::resize(/*movementModes.size()*/5);}
	~MovementMode(){}
  T& operator []       (const string &tag);
  T& operator []       (const int index);
  T& operator []       (const MovementVariety * mode);
	int getModeIndex     (const MovementVariety * mode);
protected:

};
//typedef template <class T> MovementMode<T>::iterator MovementModeIterator;



template <class T> T&  MovementMode<T>::operator [] (const string &tag)
{
	int i;
 for (i = 0; i < gameFacade->movementModes.size(); i++)
   {
      if ( gameFacade->movementModes[i] ->getTag() == tag)
       return  *(this->begin()+ i);

   }
 cerr << "Error: Tag (" << tag  << ") was not found in Movement Rules \n";
 return *(MovementMode<T>::begin()) ;

}



template <class T> T&  MovementMode<T>::operator [] (const int index)
{
    if (index < gameFacade->movementModes.size())
       return  *(MovementMode<T>::begin()+ index);


 cerr << "Error: Index (" << index  << ") is too big in Movement Rules \n";
 return *(MovementMode<T>::begin()) ;

}



template <class T> T&  MovementMode<T>::operator [] (const MovementVariety * mode)
{
	int i;
 for (i = 0; i < gameFacade->movementModes.size(); i++)
   {
      if ( gameFacade->movementModes[i] == mode)
       return  *(MovementMode<T>::begin()+ i);

   }
 cerr << "Error: Tag (" << mode->getTag()  << ") was not found in Movement Rules \n";
 return *(MovementMode<T>::begin()) ;

}



#endif
