
#include "myanimation.h"

Animation::Animation(OccView* view, std::array<Handle(AIS_Shape), 6>& aisShaps,
                     std::array<gp_Trsf,6>& MyTrsf):
    myView(view), aisShapes(aisShapes), MyTrsf(MyTrsf)
{
    for(int& q: start_Q){
        q = 0;
    }
}

void Animation::setEndPoints(std::array<int, 6>  end_Q, int time){
    for(int i = 0; i < 6; i++){
        delta_Q[i] = double(end_Q[i] - start_Q[i])/time;
    }

    frames = time * 60;

    start_Q = end_Q;
}

void Animation::run(){
    while(count < frames);

    count = 0;
    //emit stopAnimation();
}

void Animation::redraw(){
    for(int i = 0; i < 6; i++){

    }
}

