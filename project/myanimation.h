
#ifndef MYANIMATION_H
#define MYANIMATION_H

#include "occView.h"

#include <AIS_InteractiveContext.hxx>

#include <AIS_Shape.hxx>

#include <QThread>
#include <AIS_Shape.hxx>

#include <QDebug>

class OccView;

class Animation: public QThread{
    Q_OBJECT
public:
    Animation(OccView* view,std::array<Handle(AIS_Shape), 7>& aisShaps,
              std::array<gp_Ax1,7>& Axis);

    void run();

    void setEndPoints(std::array<double, 7>  end_Q, double time);
private:
    OccView* myView;

    std::array<gp_Trsf,7> myTrsf;
    std::array<gp_Ax1,7>& MyAx;

    std::array<double, 7> start_Q;
    std::array<double, 7> end_Q;
    std::array<double, 7> current_Q;
    std::array<double, 7> delta_Q;

    std::array<Handle(AIS_Shape), 7>& aisShapes;

    int frames = 0;
    int count = 0;
signals:
    void startAnimation();
    void stopAnimation();

public slots:
    void redraw();
};

#endif // MYANIMATION_H
