#ifndef MAP_VIEW_H
#define MAP_VVIEW_H
#include <vector>
#include <QFrame>
#include <QLabel>
#include <QtGui/QGraphicsItem>
class QGraphicsView;
//class QLabel;
class QSlider;
class QToolButton;
class LocationView;
class QGraphicsScene;
class QGraphicsItem;
class LocationEntity;

class MapView : public QFrame
{
    Q_OBJECT
public:
    MapView(const QString &name, LocationView * location, QWidget *parent, struct GameViewConf * configuration);

    QGraphicsView *view() const;
signals:
    void sendMousePosSignal(int,int);
public slots:
    void centerOnHex( LocationEntity * location);
private slots:
    void resetView();
    void center( QGraphicsItem * item);
 //   void center( LocationEntity * location);
    void setResetButtonEnabled();
    void setupMatrix();
    void toggleOpenGL();
    void toggleAntialiasing();
    void print();
    void zoomIn();
    void zoomOut();
    void sendMousePos(int x, int y);
    
private:
    QGraphicsView *graphicsView;
    QGraphicsScene *  populateScene();
    QLabel *label;
    QToolButton *openGlButton;
    QToolButton *antialiasButton;
    QToolButton *printButton;
    QToolButton *resetButton;
    QSlider *zoomSlider;
    LocationView * location_;
    QWidget *parent_;
    std::vector <QGraphicsItem *> items;
    struct GameViewConf * configuration_;
};

//// Button for centering map on this location
//class LocButton : public QLabel
//{
//    Q_OBJECT
//public:
//    LocButton(int x, int y, MapView * map)
//    {x_ = x; y_ = y; map_ = map;
//        connect(this,SIGNAL(linkActivated(Qstring &)),this, SLOT(clicked()));
//        connect(this,SIGNAL(clickLabel(QGraphicsItem * item)),
//                map, SLOT(center(QGraphicsItem * item)));
//    }
//    //	virtual ~LocButton();
//         signals:
//    void clickLabel(QGraphicsItem * item);
//         private slots:
//    void clicked();
//
//private:
//    MapView * map_;
//    int x_;
//    int y_;
//
//};

#endif
