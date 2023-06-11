
#include "myanimation.h"

#include <V3d_View.hxx>

Animation::Animation(OccView* view,std::array<Handle(AIS_Shape), 7>& aisShapes,
                     std::array<gp_Ax1,7>& Axis):
    myView(view), MyAx(Axis), aisShapes(aisShapes)
{
    for(double& q: start_Q){
        q = 0;
    }
}

void Animation::setEndPoints(std::array<double, 7>  end_Q_points, double time){

    qDebug() << "from second thread? with love";

    for(int i = 0; i < 7; i++){
        qDebug() << end_Q_points[i];
        delta_Q[i] = (end_Q_points[i] - start_Q[i])/(time * 60);
    }

    for(double i: delta_Q)
        qDebug() << i;

    frames = (int)time * 60;

    end_Q = end_Q_points;
    current_Q = start_Q;
    emit startAnimation();
    qDebug() << "start anim";
}

void Animation::run(){
;
    qDebug() << "Frames: " << frames;
    while(count < frames){
        //qDebug() << "count: " << count << "Frames: " << frames;
    };

    start_Q = end_Q;
    count = 0;
    emit stopAnimation();
}

void Animation::redraw(){


    for(int i = 0; i < 7; i++){
        current_Q[i]+= delta_Q[i];
        qDebug() << current_Q[1];
        myTrsf[i].SetRotation(MyAx[i], current_Q[i] * M_PI/180);
    }


    aisShapes[0]->SetLocalTransformation(myTrsf[0]);
    aisShapes[1]->SetLocalTransformation(myTrsf[0]*myTrsf[1]);
    aisShapes[2]->SetLocalTransformation((myTrsf[0]*myTrsf[1])*myTrsf[2]);
    aisShapes[3]->SetLocalTransformation(((myTrsf[0]*myTrsf[1])*myTrsf[2])*myTrsf[3]);
    aisShapes[4]->SetLocalTransformation((((myTrsf[0]*myTrsf[1])*myTrsf[2])*myTrsf[3])*myTrsf[4]);
    aisShapes[5]->SetLocalTransformation(((((myTrsf[0]*myTrsf[1])*myTrsf[2])*myTrsf[3])*myTrsf[4])*myTrsf[5]);
    aisShapes[6]->SetLocalTransformation((((((myTrsf[0]*myTrsf[1])*myTrsf[2])*myTrsf[3])*myTrsf[4])*myTrsf[5])*myTrsf[6]);

    myView->getContext()->LastActiveView()->Redraw();
    count++;

    //qDebug() << "Redraw: " << count;
}

