
#ifndef MYANIMATION_H
#define MYANIMATION_H

#include <AIS_InteractiveContext.hxx>

#include <AIS_Shape.hxx>

#include <QThread>
#include <AIS_Shape.hxx>

#include <QTimer>

class OccView;

class Thread: public QThread{
public:
    Thread(OccView* vvv, Handle(AIS_Shape) rrr);

    void run();

private:
    OccView* myOccView;
    Handle(AIS_Shape) anAisBox;

    int count = 0;
};


class MyAnimation
{
public:
    MyAnimation();

private:

};

#endif // MYANIMATION_H
