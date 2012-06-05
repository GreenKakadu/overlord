#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H
#include <QtGui/QPushButton>
class InfoWindow;
//class QDialog;
class QDockWidget;
class AbstractData;
class GameData;
class LocationEntity;
class TokenEntity;
class Rule;
class RaceRule;
class TitleRule;
class ItemRule;
class SkillRule;
class LocationView;
class FactionView;
class UnitView;
class MapView;
class MainClientWindow;
class FactionEntity;
class PopupMenuWindow;
class OrderWindow;
class OrderLine;

class ViewManager: public QObject
{
    Q_OBJECT
public:
    ViewManager(MainClientWindow * parent);
    //======================================================
    bool showRuleInfo(AbstractData * data);
    bool showSkillInfo(AbstractData * data);
    void showLocationInfo(AbstractData * data);
    void showFactionInfo(AbstractData * data);
    void showTokenInfo(AbstractData * data);
    // Context Pop-up menu
    bool showSkillPopupMenu(AbstractData * data);
    void showItemPopupMenu(AbstractData * data);
    void showRacePopupMenu(AbstractData * data);
    void showTitlePopupMenu(AbstractData * data);
    void showUnitPopupMenu(AbstractData * data);
    void showConstructionPopupMenu(AbstractData * data);
    void showLocationPopupMenu(AbstractData * data);
    void showFactionPopupMenu(AbstractData * data);
    
    void showEditOrderDialog(OrderLine * order);
signals:
    void leftClickedSignal(AbstractData * data);
    void doubleClickedSignal(AbstractData * data);
    void rightClickedSignal(AbstractData * data);
    void clickedSignal(AbstractData * data, QMouseEvent * event);
    void mouseOverSignal(AbstractData * data);
public slots:
        void showMapView();
        void showLocationView();
        void showUnitView();
        void showFactionView();
        void close();
        void openView();

        void updateView();
// Item menu
        void useItemSignalHandler(AbstractData*);
        void buyItemSignalHandler(AbstractData*);
        void sellItemSignalHandler(AbstractData*);
        void equipItemSignalHandler(AbstractData*);
        void unequipItemSignalHandler(AbstractData*);
        void giveItemSignalHandler(AbstractData*);
        void getItemSignalHandler(AbstractData*);
        void swapItemSignalHandler(AbstractData*);
 // Skill menu
      void useSkillSignalHandler(AbstractData*);        
      void learnSkillSignalHandler(AbstractData*);        
      void teachSkillSignalHandler(AbstractData*);        
      void forgetSkillSignalHandler(AbstractData*); 
// Race menu      
      void recruitSignalHandler(AbstractData*);       
// Title menu      
      void claimTitleSignalHandler(AbstractData*);       
      void releaseTitleSignalHandler(AbstractData*);       
      void bestowTitleSignalHandler(AbstractData*);       
      void revokeTitleSignalHandler(AbstractData*);       
// Unit menu  

      void acceptUnitSignalHandler(AbstractData*);       
      void attackUnitSignalHandler(AbstractData*);       
      void bestowUnitSignalHandler(AbstractData*);       
      void ejectUnitSignalHandler(AbstractData*);       
      void getFromUnitSignalHandler(AbstractData*);       
      void giveToUnitSignalHandler(AbstractData*);       
//      void oathToUnitSignalHandler(AbstractData*);       
      void promoteUnitSignalHandler(AbstractData*);       
      void stackToUnitSignalHandler(AbstractData*);       
      void stanceToUnitSignalHandler(AbstractData*);       
      void swapWithUnitSignalHandler(AbstractData*);       
      void teachUnitSignalHandler(AbstractData*);       
      void targetUnitSignalHandler(AbstractData*);       
      
// Construction menu      
      void attackConstructionSignalHandler(AbstractData*);       
      void enterConstructionSignalHandler(AbstractData*);       
      void exitConstructionSignalHandler(AbstractData*);       
      void targetConstructionSignalHandler(AbstractData*);       
      void buildConstructionSignalHandler(AbstractData*);       
// Location menu      
      void attackLocationSignalHandler(AbstractData*);       
      void moveToLocationSignalHandler(AbstractData*);       
      void marchToLocationSignalHandler(AbstractData*);       
      void caravanToLocationSignalHandler(AbstractData*);       
      void targetLocationSignalHandler(AbstractData*);       
// Faction menu      
      void stanceSignalHandler(AbstractData*);       
      void oathSignalHandler(AbstractData*);       
        // Selected objects
    inline GameData * getActiveData(){return activeData_;} 
    inline void setActiveData(GameData * data){activeData_ = data;} 
    
    inline ItemRule * getSelectedItem(){return activeItem;}
    inline RaceRule * getSelectedRace(){return activeRace;}
    inline TitleRule * getSelectedTitle(){return activeTitle;}
    inline SkillRule * getSelectedSkill(){return activeSkill;}
    inline TokenEntity * getSelectedToken(){return tokenSelected_;}
    inline LocationEntity * getSelectedLocation(){return locationSelected_;}
    inline FactionEntity * getSelectedFaction(){return factionSelected_;}
private slots:
    void showMousePos(int x, int y);
    void leftClickedSignalHandler(AbstractData * data);
    void doubleClickedSignalHandler(AbstractData * data);
    void rightClickedSignalHandler(AbstractData * data);
    void mouseOverSignalHandler(AbstractData * data);
    void clickedSignalHandler(AbstractData * data, QMouseEvent * event );
signals:
    void goToLocation(LocationEntity * location);
    void goToFaction(FactionEntity * faction);
    void goToUnit(TokenEntity * unit);
    void goToRule(Rule * rule);
    void closeInfoWindow();
    void closeOrderWindow();
    void closePopupMenu();
    void updateUnitView(TokenEntity * token);
private:
    MainClientWindow * parent_;
    InfoWindow * infoWindow;
    PopupMenuWindow * popupMenuWindow;
     OrderWindow * orderWindow;
    // Layout
    QDockWidget * leftDock;
    QDockWidget * center;
    QDockWidget * rightDock1;
    QDockWidget * rightDock2;
    MapView * map;
    LocationView * locationView_;
    UnitView * unitView_;
    FactionView * factionView_;
    bool isOpened_;
// Selected
    GameData * activeData_;
         ItemRule  * activeItem;
         SkillRule  * activeSkill;
         RaceRule  * activeRace;
         TitleRule  * activeTitle;
         TokenEntity * tokenSelected_;
         LocationEntity * locationSelected_;
         FactionEntity * factionSelected_;
};

#endif // VIEWMANAGER_H
