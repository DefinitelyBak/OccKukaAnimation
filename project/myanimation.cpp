
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

// Расчёт приращения изменения координат
    for(int i = 0; i < 7; i++){
        qDebug() << end_Q_points[i];
        delta_Q[i] = (end_Q_points[i] - start_Q[i])/(time * 60);
    }

// Расчёт количества кадров
    frames = (int)time * 60;

// Изменение обобщенных координат
    end_Q = end_Q_points;
    current_Q = start_Q;
// Отправка сигнала запуска работы таймера
    emit startAnimation();
}

void Animation::run(){

    // Цикл работающий до завершения отрисвоки всех кадров
    while(count < frames){};

    // Обновление начальных координат манипулятора
    start_Q = end_Q;
    // Обнуление счётчика кадрлов
    count = 0;
    // Отправка сигнала завершения работы таймера
    emit stopAnimation();
}

void Animation::redraw(){

    // Инициализация матриц поворота текущими значениями обощенных координат
    for(int i = 0; i < 7; i++){
        current_Q[i]+= delta_Q[i];
        myTrsf[i].SetRotation(MyAx[i], current_Q[i] * M_PI/180);
    }

    // Установка отображаемых фигур в положения полученные перемножением матриц преобразований
    gp_Trsf ax1 = myTrsf[0]*myTrsf[1];
    gp_Trsf ax2 = ax1 * myTrsf[2];
    gp_Trsf ax3 = ax2 * myTrsf[3];
    gp_Trsf ax4 = ax4 * myTrsf[4];
    gp_Trsf ax5 = ax4 * myTrsf[5];
    gp_Trsf ax6 = ax2 * myTrsf[6];

    aisShapes[0]->SetLocalTransformation(myTrsf[0]);
    aisShapes[1]->SetLocalTransformation(ax1);
    aisShapes[2]->SetLocalTransformation(ax2);
    aisShapes[3]->SetLocalTransformation(ax3);
    aisShapes[4]->SetLocalTransformation(ax4);
    aisShapes[5]->SetLocalTransformation(ax5);
    aisShapes[6]->SetLocalTransformation(ax6);

    myView->getContext()->LastActiveView()->Redraw();
    count++;
}

