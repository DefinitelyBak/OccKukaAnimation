/*
*    Copyright (c) 2018 Shing Liu All Rights Reserved.
*
*           File : occQt.cpp
*         Author : Shing Liu(eryar@163.com)
*           Date : 2018-01-08 21:00
*        Version : OpenCASCADE7.2.0 & Qt5.7.1
*
*    Description : Qt main window for OpenCASCADE.
*/

#include "occQt.h"
#include "V3d_Viewer.hxx"
#include "occView.h"

#include <QToolBar>
#include <QTreeView>
#include <QMessageBox>
#include <QDockWidget>

#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Pln.hxx>

#include <gp_Lin2d.hxx>

#include <Geom_ConicalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_CylindricalSurface.hxx>

#include <GCE2d_MakeSegment.hxx>

#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TColgp_Array1OfPnt2d.hxx>

#include <BRepLib.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>

#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>

#include <TopoDS_CompSolid.hxx>

#include <AIS_Animation.hxx>
#include <AIS_AnimationObject.hxx>
#include <STEPControl_Reader.hxx>

occQt::occQt(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    createMainWidget();
    createActions();
    createMenus();
    createToolBars();
}

occQt::~occQt()
{

}

void occQt::createActions()
{
    //Push button
    connect(ButtonStartAinmation, &QPushButton::clicked, this, &occQt::PushButton);

    //Start animation
    connect(this, &occQt::InitAnimation, myOccView, &OccView::slotStartAnim);

    // File
    connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));

    // View
    connect(ui.actionZoom, SIGNAL(triggered()), myOccView, SLOT(zoom()));
    connect(ui.actionPan, SIGNAL(triggered()), myOccView, SLOT(pan()));
    connect(ui.actionRotate, SIGNAL(triggered()), myOccView, SLOT(rotate()));

    connect(ui.actionReset, SIGNAL(triggered()), myOccView, SLOT(reset()));
    connect(ui.actionFitAll, SIGNAL(triggered()), myOccView, SLOT(fitAll()));

    // Help
    connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));
}

void occQt::createMenus()
{}

void occQt::createToolBars()
{
    QToolBar* aToolBar = addToolBar(tr("&Navigate"));
    aToolBar->addAction(ui.actionZoom);
    aToolBar->addAction(ui.actionPan);
    aToolBar->addAction(ui.actionRotate);

    aToolBar = addToolBar(tr("&View"));
    aToolBar->addAction(ui.actionReset);
    aToolBar->addAction(ui.actionFitAll);


    aToolBar = addToolBar(tr("Help"));
    aToolBar->addAction(ui.actionAbout);
}

void occQt::createMainWidget(){
    myOccView = new OccView(this);

    myOccView->setMinimumSize(650,100);

    lable_q1 = new QLabel("Q1", this);
    lable_q2 = new QLabel("Q2", this);
    lable_q3 = new QLabel("Q3", this);
    lable_q4 = new QLabel("Q4", this);
    lable_q5 = new QLabel("Q5", this);
    lable_q6 = new QLabel("Q6", this);
    lable_time = new QLabel("Time", this);

    lineEdit_q1 = new QLineEdit(this);
    lineEdit_q2 = new QLineEdit(this);
    lineEdit_q3 = new QLineEdit(this);
    lineEdit_q4 = new QLineEdit(this);
    lineEdit_q5 = new QLineEdit(this);
    lineEdit_q6 = new QLineEdit(this);
    lineEdit_time = new QLineEdit(this);

    ButtonStartAinmation = new QPushButton("Start",this);

    QHBoxLayout *mainLayout = new QHBoxLayout();

    QWidget* rightWidget = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout;

    rightLayout->addWidget(lable_q1);
    rightLayout->addWidget(lineEdit_q1);
    rightLayout->addWidget(lable_q2);
    rightLayout->addWidget(lineEdit_q2);
    rightLayout->addWidget(lable_q3);
    rightLayout->addWidget(lineEdit_q3);
    rightLayout->addWidget(lable_q4);
    rightLayout->addWidget(lineEdit_q4);
    rightLayout->addWidget(lable_q5);
    rightLayout->addWidget(lineEdit_q5);
    rightLayout->addWidget(lable_q6);
    rightLayout->addWidget(lineEdit_q6);
    rightLayout->addWidget(lable_time);
    rightLayout->addWidget(lineEdit_time);
    rightLayout->addStretch(1);
    rightLayout->addWidget(ButtonStartAinmation);

    rightWidget->setLayout(rightLayout);

    mainLayout->addWidget(myOccView);
    mainLayout->addWidget(rightWidget);

    centralWidget()->setLayout(mainLayout);
}

void occQt::about()
{
    QMessageBox::about(this, tr("About occQt"),
        tr("<h2>occQt 2.0</h2>"
        "<p>Copyright &copy; 2014 eryar@163.com"
        "<p>occQt is a demo applicaton about Qt and OpenCASCADE."));
}

//Signals

void occQt::PushButton(){
    std::array<double, 8> input_Q;
    qDebug() << (input_Q[0] = 0);
    qDebug() << (input_Q[1] = lineEdit_q1->displayText().toDouble() );
    qDebug() << (input_Q[2] = lineEdit_q2->displayText().toDouble() );
    qDebug() << (input_Q[3] = lineEdit_q3->displayText().toDouble() );
    qDebug() << (input_Q[4] = lineEdit_q4->displayText().toDouble() );
    qDebug() << (input_Q[5] = lineEdit_q5->displayText().toDouble() );
    qDebug() << (input_Q[6] = lineEdit_q6->displayText().toDouble() );
    qDebug() << (input_Q[7] =lineEdit_time->displayText().toDouble());


    emit InitAnimation(input_Q);
}
