#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H
#include <string>
#include <QLabel>
#include <QString>
#include <QPixmap>
#include "CustomStyles.h"
class AbstractData;
enum HoveringState {
    NO_HOVERING =0,
    ENTERING_HOVERING =1,
    EXITING_HOVERING =2,
    HOVERING_IN =3,
    LAST_HOVERING = 99
};



class ClickableLabel: public QLabel
{
    Q_OBJECT
    public:
    ClickableLabel(AbstractData * data, bool isPlural = false);
    ClickableLabel(int value);
    ClickableLabel(const char * txt);
    ClickableLabel(std::string txt);
    ClickableLabel(QString text);
    ClickableLabel(QPixmap icon, int sizeX =24, int sizeY=24);
    static ClickableLabel * makeClickableString(std::string txt);
    ~ClickableLabel(){}
    inline AbstractData * getData(){return data_;}
    signals:
    void clicked(AbstractData * data, QMouseEvent * event);
    void leftClicked(AbstractData * data);
    void doubleClicked(AbstractData * data);
    void rightClicked(AbstractData * data);
    void mouseOver(AbstractData * data);
    protected:
    void setup(bool toSetIndent = false);
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    void mouseDoubleClickEvent( QMouseEvent * event );
    bool event ( QEvent * event );
    void makeButton(CustomStyle colorSet = GRAY_COLOR);
    AbstractData * data_;
    int timerId_;
    HoveringState state_;
};
#endif // CLICKABLELABEL_H
