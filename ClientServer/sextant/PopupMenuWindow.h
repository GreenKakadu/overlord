#ifndef POPUPMENUWINDOW_H
#define POPUPMENUWINDOW_H
#include <QWidget>
#include <QFrame>
class AbstractData;
class QVBoxLayout;
class QPushButton;
class ClickableLine;
class ClickableLabel;

class PopupMenuWindow : public QFrame
{
    Q_OBJECT
public:
    explicit PopupMenuWindow(QWidget *parent = 0,Qt::WindowFlags flag =0);
    void addWidget(ClickableLabel * item, bool isNewLine = false);
    ClickableLabel * addMenuItem(QString  menuName);
    void clean();
    void setMyLayout();
    void setMaxWidth(int width);

signals:
    void leftClickedSignal(AbstractData * data);
    void doubleClickedSignal(AbstractData * data);
    void rightClickedSignal(AbstractData * data);
    void clickedSignal(AbstractData * data, QMouseEvent * event);
    void mouseOverSignal(AbstractData * data);
public slots:
    void close();
    void doClose();
    void leftClickedSlot(AbstractData * data);
    void doubleClickedSlot(AbstractData * data);
    void rightClickedSlot(AbstractData * data);
    void clickedSlot(AbstractData * data, QMouseEvent * event);
    void mouseOverSlot(AbstractData * data);
protected:
    bool event ( QEvent * event );
    void resizeEvent(QResizeEvent * /* event */);

    QVBoxLayout *mainLayout;

    ClickableLine * currentLine;

    std::vector<QWidget *> items;
    int maxWidth;
    bool isKeepingOpen;

};

#endif // POPUPMENUWINDOW_H
