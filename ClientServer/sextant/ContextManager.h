#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H
#include <QWidget>
class AbstractData;
class LocationEntity;
class PopupMenuWindow;
class MainClientWindow;
class OrderWindow;
class InfoWindow;
class ItemRule;

class ContextManager : public QObject
{
        Q_OBJECT
public:
    explicit ContextManager(MainClientWindow * parent );
    ~ContextManager(){}
public slots:
    void leftClickedSignalHandler(AbstractData * data);
    void doubleClickedSignalHandler(AbstractData * data);
    void rightClickedSignalHandler(AbstractData * data);
    void mouseOverSignalHandler(AbstractData * data);
    void clickedSignalHandler(AbstractData * data, QMouseEvent * event );
    void useItemSignalHandler(AbstractData*);
    void giveItemSignalHandler(AbstractData*);
signals:
        void closeOrderWindow();
public:
        void showItemPopupMenu(AbstractData * data);
private:
    MainClientWindow * parent_;
    LocationEntity * selectedLocation;
    PopupMenuWindow * popupMenuWindow;
    PopupMenuWindow * p1;
    OrderWindow * orderWindow;
    InfoWindow * infoWindow;
     ItemRule  * activeItem;
};

#endif // CONTEXTMANAGER_H
