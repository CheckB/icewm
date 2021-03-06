#ifndef ATASKS_H_
#define ATASKS_H_

#include "ywindow.h"
#include "wmclient.h"

class TaskPane;
class TaskBarApp;
class IAppletContainer;

class TaskBarApp: public YWindow, public YTimerListener {
public:
    TaskBarApp(ClientData *frame, TaskPane *taskPane, YWindow *aParent);
    virtual ~TaskBarApp();

    virtual bool isFocusTraversable();

    virtual void paint(Graphics &g, const YRect &r);
    virtual void handleButton(const XButtonEvent &button);
    virtual void handleClick(const XButtonEvent &up, int count);
    virtual void handleCrossing(const XCrossingEvent &crossing);
    virtual void handleDNDEnter();
    virtual void handleDNDLeave();
    virtual bool handleTimer(YTimer *t);
    virtual void handleBeginDrag(const XButtonEvent &down, const XMotionEvent &motion);

    ClientData *getFrame() const { return fFrame; }

    void setShown(bool show);
    bool getShown() const { return fShown || fFlashing; }

    void setFlash(bool urgent);

private:
    ClientData *fFrame;
    TaskPane *fTaskPane;
    bool fShown;
    bool fFlashing;
    bool fFlashOn;
    timeval fFlashStart;
    int selected;
    YTimer *fFlashTimer;
    static YTimer *fRaiseTimer;
};

class TaskPane: public YWindow {
public:
    TaskPane(IAppletContainer *taskBar, YWindow *parent);
    ~TaskPane();

    void insert(TaskBarApp *tapp);
    void remove(TaskBarApp *tapp);
    TaskBarApp *addApp(YFrameWindow *frame);
    void removeApp(YFrameWindow *frame);

    void relayout() { fNeedRelayout = true; }
    void relayoutNow();

    virtual void handleClick(const XButtonEvent &up, int count);
    virtual void handleMotion(const XMotionEvent &motion);
    virtual void handleButton(const XButtonEvent &button);
    virtual void paint(Graphics &g, const YRect &r);

    void startDrag(TaskBarApp *drag, int byMouse, int sx, int sy);
    void processDrag(int mx, int my);
    void endDrag();
    TaskBarApp* dragging() const { return fDragging; }

    virtual void handleDrag(const XButtonEvent &down, const XMotionEvent &motion)//LXP
                 {parent()->handleDrag(down,motion);}//LXP
    virtual void handleEndDrag(const XButtonEvent &down, const XButtonEvent &up)//LXP
                 {parent()->handleEndDrag(down,up);}//LXP
private:
    IAppletContainer *fTaskBar;

    typedef YObjectArray<TaskBarApp> AppsType;
    typedef AppsType::IterType IterType;
    AppsType fApps;

    bool fNeedRelayout;

    TaskBarApp *fDragging;
    int fDragX;
    int fDragY;
};

#endif

// vim: set sw=4 ts=4 et:
