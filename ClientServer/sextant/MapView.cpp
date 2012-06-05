#include "MapView.h"

#include <QtGui>
#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif

#include <math.h>
#include "hex.h"
#include "LocationView.h"
#include "LocationEntity.h"
#include <QGraphicsItem>
#include "GameFacade.h"
#include "InnerLocation.h"
#include "MainClientWindow.h" // for GameViewConf
MapView::MapView(const QString &name, LocationView * location, QWidget *parent, struct GameViewConf * configuration)
        : QFrame(parent)
{
    parent_ = parent;
    location_ = location;
    configuration_ = configuration;  
//    zoomLevel = 0;
//    isUsingAntialiasing = FALSE;
//    isUsingOpenGL = FALSE;
//    if(configuration)
//    {
//    zoomLevel = configuration->zoomLevel;
//    isUsingAntialiasing = configuration->isUsingAntialiasing;
//    isUsingOpenGL = configuration->isUsingOpenGL;
//    }
    
    setFrameStyle(Sunken | StyledPanel);
    graphicsView = new QGraphicsView;
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setScene(populateScene());

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/new/prefix1/images/zoomin.png"));
    zoomInIcon->setIconSize(iconSize);
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/new/prefix1/images/zoomout.png"));
    zoomOutIcon->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(configuration_->zoomLevel);
    zoomSlider->setTickPosition(QSlider::TicksRight);

    // Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);


    resetButton = new QToolButton;
    resetButton->setText(tr("Reset"));
    resetButton->setEnabled(false);

    // Label layout
    QHBoxLayout *labelLayout = new QHBoxLayout;
    label = new QLabel(name);
    antialiasButton = new QToolButton;
    antialiasButton->setText(tr("Antialiasing"));
    antialiasButton->setCheckable(true);
    antialiasButton->setChecked(configuration_->isUsingAntialiasing);
    openGlButton = new QToolButton;
    openGlButton->setText(tr("OpenGL"));
    openGlButton->setCheckable(true);
#ifndef QT_NO_OPENGL
    openGlButton->setEnabled(QGLFormat::hasOpenGL());
#else
    openGlButton->setEnabled(false);
#endif
    printButton = new QToolButton;
    printButton->setIcon(QIcon(QPixmap(":/new/prefix1/images/fileprint.png")));

    labelLayout->addWidget(label);
    labelLayout->addStretch();
    labelLayout->addWidget(antialiasButton);
    labelLayout->addWidget(openGlButton);
    labelLayout->addWidget(printButton);

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(labelLayout, 0, 0);
    topLayout->addWidget(graphicsView, 1, 0);
    topLayout->addLayout(zoomSliderLayout, 1, 1);
    topLayout->addWidget(resetButton, 2, 1);
    setLayout(topLayout);

    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(setResetButtonEnabled()));
    connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(setResetButtonEnabled()));
    connect(antialiasButton, SIGNAL(toggled(bool)),
            this, SLOT(toggleAntialiasing()));
    connect(openGlButton, SIGNAL(toggled(bool)), this, SLOT(toggleOpenGL()));
    connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));
    connect(printButton, SIGNAL(clicked()), this, SLOT(print()));

    setupMatrix();
}

QGraphicsView *MapView::view() const
{
    return graphicsView;
}

void MapView::resetView()
{
    zoomSlider->setValue(250);
    setupMatrix();
    graphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    resetButton->setEnabled(false);
}

void MapView::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}

void MapView::setupMatrix()
{
    qreal scale = ::pow(2.0, (zoomSlider->value() - 250) / 50.0);

    QMatrix matrix;
    matrix.scale(scale, scale);

    graphicsView->setMatrix(matrix);
#ifndef QT_NO_OPENGL    
    if(configuration_->isUsingOpenGL)
    {
      graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers))); 
    }
#endif 
    graphicsView->setRenderHint(QPainter::Antialiasing,
                                configuration_->isUsingAntialiasing);
    configuration_->zoomLevel = zoomSlider->value();    
    setResetButtonEnabled();
}

void MapView::toggleOpenGL()
{
#ifndef QT_NO_OPENGL
    graphicsView->setViewport(openGlButton->isChecked() ?
            new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
    configuration_->isUsingOpenGL = !configuration_->isUsingOpenGL;
    if(configuration_->isUsingOpenGL)
    cout<<"OpenGL now is TRUE "<<configuration_->isUsingOpenGL<<endl;
    else
    {
      cout<<"OpenGL now is FALSE "<<configuration_->isUsingOpenGL<<endl;  
    }
#endif
}

void MapView::toggleAntialiasing()
{
    graphicsView->setRenderHint(QPainter::Antialiasing,
                                antialiasButton->isChecked());
    configuration_->isUsingAntialiasing = ! configuration_->isUsingAntialiasing;
}

void MapView::print()
{
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        graphicsView->render(&painter);
    }
}

void MapView::zoomIn()
{
    configuration_->zoomLevel++;
    zoomSlider->setValue(zoomSlider->value() + 1);
}

void MapView::zoomOut()
{
    configuration_->zoomLevel--;
    zoomSlider->setValue(zoomSlider->value() - 1);
}

QGraphicsScene *  MapView::populateScene()
{
    QGraphicsScene * scene = new QGraphicsScene;
    LocationEntity * location;

    // Populate scene
    int xx =0,yy = 0;
    int nitems = 0;


    for (int i=0; i < gameFacade->locations.size();i++)
    {
        location = gameFacade->locations[i];
        if(location ==0)
            continue;
        xx = location->getX();
        yy = location->getY();
        if((xx==0)&&(yy==0)) // This is inner location
          {
            // Get outer location
            LocationEntity * outerLocation = location->getOuterLocation();

              if(outerLocation==0)
                {
                  cout<<"Can'r find outer location for "<<location->print()<<endl;
                  continue;
                }

            InnerLocation * item = new InnerLocation(location,QPixmap(":/new/prefix1/icons/Castle.png"));
            item->setPos(QPointF(outerLocation->getX()*150 +125, outerLocation->getY() * -87 +43));

            if(location_ == 0) printf("locationTab == 0\n ");
            else
            {
                connect(item,SIGNAL(sendLocData(LocationEntity *)),
                        location_,SLOT(leftClickOnLocation(LocationEntity*)));
            }


            scene->addItem(item);
            items.push_back(item);

            continue;
          }
        TerrainRule * terrain = location->getTerrain();
        QColor color(terrain->getR(),terrain->getG(),terrain->getB());
        Hex *item = new Hex(color,xx ,yy ,location);
        if(location_ == 0) printf("locationTab == 0\n ");
        else
        {
            connect(item,SIGNAL(sendLocData(LocationEntity *)),
                    location_,SLOT(updateLocation(LocationEntity*)));
        }
        connect(item,SIGNAL(sendMousePos(int,int)),
                this,SLOT(sendMousePos(int,int)));
        item->setPos(QPointF(xx*150, yy * -87));

        scene->addItem(item);
        items.push_back(item);

        ++nitems;
    }
    return scene;
}
 void MapView::sendMousePos(int x, int y)
    {
     emit sendMousePosSignal(x, y);
    }



void  MapView::center( QGraphicsItem * item)
{
    graphicsView->centerOn(item);
}

void  MapView::centerOnHex( LocationEntity * location)
{
    if(location ==0)
    {
        return;
    }
    for(std::vector <QGraphicsItem *>::iterator iter=items.begin(); iter != items.end(); ++iter)
    {
        Hex * hex = dynamic_cast< Hex *>(*iter);
        if(hex)
        {
            if(hex->getLocation() == location)
            {
                    graphicsView->centerOn(hex);
                    cout<<"Found "<<endl;
                    return;
            }
        }
    }
//   cout << location->print()<< " has "<< getInnerLocationNumber(location)<<" inner locations"<<endl;
   cout<<"Location "<< location->print()<< "not found."<<endl;
}

//void LocButton :: clicked() {
//    QGraphicsItem * item = map_->view()->itemAt(x_,y_);
//    emit clickLabel(item);
//}
