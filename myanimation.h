
#ifndef MYANIMATION_H
#define MYANIMATION_H

#include <AIS_InteractiveContext.hxx>

#include <AIS_Shape.hxx>

#include <QThread>
#include <AIS_Shape.hxx>

#include <QTimer>

class OccView;

class Animation: public QThread{
    Q_OBJECT
public:
    Animation(OccView* view,std::array<Handle(AIS_Shape), 6>& aisShaps,
              std::array<gp_Trsf,6>& MyTrsf);

    void run();

    void setEndPoints(std::array<int, 6>  end_Q, int time);
private:
    OccView* myView;

    std::array<double, 6> start_Q;
    std::array<double, 6> current_Q;
    std::array<double, 6> delta_Q;

    std::array<Handle(AIS_Shape), 6>& aisShapes;
    std::array<gp_Trsf,6>& MyTrsf;

    int frames = 0;
    int count = 0;
signals:
    //void startAnimation();
    void stopAnimation();

public slots:
    void redraw();
};

#endif // MYANIMATION_H
