/***************************************************************************
                          Title.h  

                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef TITLE_H
#define TITLE_H
#include "GameData.h"

//#include "Skill.h"

class TitleRule : public GameData
{
    public:
      TitleRule (const string & keyword, GameData * parent );
      TitleRule (const TitleRule * prototype );
      TitleRule ( );

      //   Skill*      getSkill()        const;
      int         getCost()          const;
      int         getControl()       const;
      int         getType()          const;
      int         getRange()         const;
      
      //        void setSkill       ( Skill *skill);
      void setCost        ( int cost);
      void setControl     ( int control);
      void setType        ( int type);
      void setRange       ( int range);
      virtual STATUS initialize      ( Parser *parser);
      
      GameData * createInstanceOfSelf();
      void print();
    protected:
      //   Skill  *skill_;             // reference to skill  needed posses this title
   int cost_;                  //cost to challenge for the title
   int control_;               //number of control points title added to faction
   int type_;                  //There are type 0 through type 3.
   int range_;                 //the number of hexes dominated by this title

   private:
};


class MinorTitleRule : public TitleRule
{
public:
   MinorTitleRule (const string & keyword, GameData * parent );
   MinorTitleRule (const MinorTitleRule * prototype  );
   MinorTitleRule ( );
   GameData * createInstanceOfSelf();
private:
};



class MajorTitleRule : public TitleRule
{
public:
   MajorTitleRule (const string & keyword, GameData * parent );
   MajorTitleRule (const MajorTitleRule * prototype   );
   MajorTitleRule ( );
   GameData * createInstanceOfSelf();
private:
};



class StandardTitleRule : public TitleRule
{
public:
   StandardTitleRule (const string & keyword, GameData * parent );
   StandardTitleRule (const StandardTitleRule  * prototype   );
   StandardTitleRule ( );
   GameData * createInstanceOfSelf();
private:
};



class OverlordTitleRule : public TitleRule
{
public:
   OverlordTitleRule (const string & keyword, GameData * parent );
   OverlordTitleRule (const OverlordTitleRule * prototype   );
   OverlordTitleRule ( );
   GameData * createInstanceOfSelf();
   void print();
private:
};

#endif
