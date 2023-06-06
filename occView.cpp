/*
*    Copyright (c) 2018 Shing Liu All Rights Reserved.
*
*           File : OccView.cpp
*         Author : Shing Liu(eryar@163.com)
*           Date : 2018-01-08 21:00
*        Version : OpenCASCADE7.2.0 & Qt5.7.1
*
*    Description : Qt widget for OpenCASCADE viewer.
*/

#include <OpenGl_GraphicDriver.hxx>

#include "occView.h"

#include <QMenu>
#include <QMouseEvent>
#include <QRubberBand>
#include <QStyleFactory>

#include <V3d_View.hxx>

#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>

#include <AIS_Shape.hxx>
#include <AIS_Animation.hxx>
#include <AIS_AnimationObject.hxx>

#include <STEPControl_Reader.hxx>

#include <BRepPrimAPI_MakeBox.hxx>

#include "TopoDS_Solid.hxx"

#define WNT

#ifdef WNT
    #include <WNT_Window.hxx>
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
    #include <Cocoa_Window.hxx>
#else
    #undef Bool
    #undef CursorShape
    #undef None
    #undef KeyPress
    #undef KeyRelease
    #undef FocusIn
    #undef FocusOut
    #undef FontChange
    #undef Expose
    #include <Xw_Window.hxx>
#endif


static Handle(Graphic3d_GraphicDriver)& GetGraphicDriver()
{
  static Handle(Graphic3d_GraphicDriver) aGraphicDriver;
  return aGraphicDriver;
}

OccView::OccView(QWidget* parent )
    : QWidget(parent),
    myXmin(0),
    myYmin(0),
    myXmax(0),
    myYmax(0),    
    myCurrentMode(CurAction3d_DynamicRotation),
    myDegenerateModeIsOn(Standard_True),
    myRectBand(NULL),
    deg_joint0(0),
    deg_joint1(0)
{
    // No Background
    setBackgroundRole( QPalette::NoRole );

    // set focus policy to threat QContextMenuEvent from keyboard  
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    // Enable the mouse tracking, by default the mouse tracking is disabled.
    setMouseTracking( true );

    init();
    create_objects();
    update_joint_slider();
}

void OccView::init()
{
    // Create Aspect_DisplayConnection
    Handle(Aspect_DisplayConnection) aDisplayConnection =
            new Aspect_DisplayConnection();

    // Get graphic driver if it exists, otherwise initialise it
    if (GetGraphicDriver().IsNull())
    {
        GetGraphicDriver() = new OpenGl_GraphicDriver(aDisplayConnection);
    }

    // Get window handle. This returns something suitable for all platforms.
    WId window_handle = (WId) winId();

    // Create appropriate window for platform
    #ifdef WNT
        Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle) window_handle);
    #elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
        Handle(Cocoa_Window) wind = new Cocoa_Window((NSView *) window_handle);
    #else
        Handle(Xw_Window) wind = new Xw_Window(aDisplayConnection, (Window) window_handle);
    #endif

    // Create V3dViewer and V3d_View
    //myViewer = new V3d_Viewer(GetGraphicDriver(), Standard_ExtString("viewer3d"));

    myViewer = new V3d_Viewer(GetGraphicDriver());
    myView = myViewer->CreateView();

    myView->SetWindow(wind);
    if (!wind->IsMapped()) wind->Map();

    // Create AISInteractiveContext
    myContext = new AIS_InteractiveContext(myViewer);

    // Set up lights etc
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();

    myView->SetBackgroundColor(Quantity_NOC_BLACK);
    myView->MustBeResized();
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

    myContext->SetDisplayMode(AIS_Shaded, Standard_True);
}

const Handle(AIS_InteractiveContext)& OccView::getContext() const
{
    return myContext;
}

/*!
Get paint engine for the OpenGL viewer. [ virtual public ]
*/
QPaintEngine* OccView::paintEngine() const
{
    return 0;
}

void OccView::paintEvent( QPaintEvent* /*theEvent*/ )
{
    myView->Redraw();
}

void OccView::resizeEvent( QResizeEvent* /*theEvent*/ )
{
    if( !myView.IsNull() )
    {
        myView->MustBeResized();
    }
}

void OccView::fitAll( void )
{
    myView->FitAll();
    myView->ZFitAll();
    myView->Redraw();
}

void OccView::reset( void )
{
    myView->Reset();
}

void OccView::pan( void )
{
    myCurrentMode = CurAction3d_DynamicPanning;
}

void OccView::zoom( void )
{
    myCurrentMode = CurAction3d_DynamicZooming;
}

void OccView::rotate( void )
{
    myCurrentMode = CurAction3d_DynamicRotation;
}

void OccView::mousePressEvent( QMouseEvent* theEvent )
{
    if (theEvent->button() == Qt::LeftButton)
    {
        onLButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
    else if (theEvent->button() == Qt::MiddleButton)
    {
        onMButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
    else if (theEvent->button() == Qt::RightButton)
    {
        onRButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
}

void OccView::mouseReleaseEvent( QMouseEvent* theEvent )
{
    if (theEvent->button() == Qt::LeftButton)
    {
        onLButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
    else if (theEvent->button() == Qt::MiddleButton)
    {
        onMButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
    else if (theEvent->button() == Qt::RightButton)
    {
        onRButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
}

void OccView::mouseMoveEvent( QMouseEvent * theEvent )
{
    onMouseMove(theEvent->buttons(), theEvent->pos());
}

void OccView::wheelEvent( QWheelEvent * theEvent )
{
    onMouseWheel(theEvent->buttons(), theEvent->angleDelta(), theEvent->position());
}

void OccView::onLButtonDown( const int /*theFlags*/, const QPoint thePoint )
{
    // Save the current mouse coordinate in min.
    myXmin = thePoint.x();
    myYmin = thePoint.y();
    myXmax = thePoint.x();
    myYmax = thePoint.y();

}

void OccView::onMButtonDown( const int /*theFlags*/, const QPoint thePoint )
{
    // Save the current mouse coordinate in min.
    myXmin = thePoint.x();
    myYmin = thePoint.y();
    myXmax = thePoint.x();
    myYmax = thePoint.y();

    if (myCurrentMode == CurAction3d_DynamicRotation)
    {
        myView->StartRotation(thePoint.x(), thePoint.y());
    }
}

void OccView::onRButtonDown( const int /*theFlags*/, const QPoint /*thePoint*/ )
{

}

void OccView::onMouseWheel( const int /*theFlags*/, const QPoint theDelta, const QPointF thePoint )
{
    Standard_Integer aFactor = 16;

    Standard_Integer aX = thePoint.x();
    Standard_Integer aY = thePoint.y();

    if (theDelta.y() > 0)
    {
        aX += aFactor;
        aY += aFactor;
    }
    else
    {
        aX -= aFactor;
        aY -= aFactor;
    }

    myView->Zoom(thePoint.x(), thePoint.y(), aX, aY);
}

void OccView::addItemInPopup( QMenu* /*theMenu*/ )
{
}

void OccView::popup( const int /*x*/, const int /*y*/ )
{
}

void OccView::onLButtonUp( const int theFlags, const QPoint thePoint )
{
    // Hide the QRubberBand
    if (myRectBand)
    {
        myRectBand->hide();
    }

    // Ctrl for multi selection.
    if (thePoint.x() == myXmin && thePoint.y() == myYmin)
    {
        if (theFlags & Qt::ControlModifier)
        {
            multiInputEvent(thePoint.x(), thePoint.y());
        }
        else
        {
            inputEvent(thePoint.x(), thePoint.y());
        }
    }

}

void OccView::onMButtonUp( const int /*theFlags*/, const QPoint thePoint )
{
    if (thePoint.x() == myXmin && thePoint.y() == myYmin)
    {
        panByMiddleButton(thePoint);
    }
}

void OccView::onRButtonUp( const int /*theFlags*/, const QPoint thePoint )
{
    popup(thePoint.x(), thePoint.y());
}

void OccView::onMouseMove( const int theFlags, const QPoint thePoint )
{
    // Draw the rubber band.
    if (theFlags & Qt::LeftButton)
    {
        drawRubberBand(myXmin, myYmin, thePoint.x(), thePoint.y());

        dragEvent(thePoint.x(), thePoint.y());
    }

    // Ctrl for multi selection.
    if (theFlags & Qt::ControlModifier)
    {
        multiMoveEvent(thePoint.x(), thePoint.y());
    }
    else
    {
        moveEvent(thePoint.x(), thePoint.y());
    }

    // Middle button.
    if (theFlags & Qt::MiddleButton)
    {
        switch (myCurrentMode)
        {
        case CurAction3d_DynamicRotation:
            myView->Rotation(thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicZooming:
            myView->Zoom(myXmin, myYmin, thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicPanning:
            myView->Pan(thePoint.x() - myXmax, myYmax - thePoint.y());
            myXmax = thePoint.x();
            myYmax = thePoint.y();
            break;

         default:
            break;
        }
    }

}

void OccView::dragEvent( const int x, const int y )
{
    myContext->Select(myXmin, myYmin, x, y, myView, Standard_True);

    emit selectionChanged();
}

void OccView::multiDragEvent( const int x, const int y )
{
    myContext->ShiftSelect(myXmin, myYmin, x, y, myView, Standard_True);

    emit selectionChanged();

}

void OccView::inputEvent( const int x, const int y )
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    myContext->Select(Standard_True);

    emit selectionChanged();
}

void OccView::multiInputEvent( const int x, const int y )
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    myContext->ShiftSelect(Standard_True);

    emit selectionChanged();
}

void OccView::moveEvent( const int x, const int y )
{
    myContext->MoveTo(x, y, myView, Standard_True);
}

void OccView::multiMoveEvent( const int x, const int y )
{
    myContext->MoveTo(x, y, myView, Standard_True);
}

void OccView::drawRubberBand( const int minX, const int minY, const int maxX, const int maxY )
{
    QRect aRect;

    // Set the rectangle correctly.
    (minX < maxX) ? (aRect.setX(minX)) : (aRect.setX(maxX));
    (minY < maxY) ? (aRect.setY(minY)) : (aRect.setY(maxY));

    aRect.setWidth(abs(maxX - minX));
    aRect.setHeight(abs(maxY - minY));

    if (!myRectBand)
    {
        myRectBand = new QRubberBand(QRubberBand::Rectangle, this);

        // setStyle is important, set to windows style will just draw
        // rectangle frame, otherwise will draw a solid rectangle.
        myRectBand->setStyle(QStyleFactory::create("windows"));
    }

    myRectBand->setGeometry(aRect);
    myRectBand->show();
}

void OccView::panByMiddleButton( const QPoint& thePoint )
{
    Standard_Integer aCenterX = 0;
    Standard_Integer aCenterY = 0;

    QSize aSize = size();

    aCenterX = aSize.width() / 2;
    aCenterY = aSize.height() / 2;

    myView->Pan(aCenterX - thePoint.x(), thePoint.y() - aCenterY);
}


void OccView::animation(Handle(AIS_Shape) anAisBox){
    gp_Trsf start_pnt, end_pnt;

    start_pnt.SetValues(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);
    end_pnt.SetValues(1, 0, 0, 100, 0, 1, 0, 100, 0, 0, 1, 100);

    Handle(AIS_Animation) ais_animation = new AIS_Animation("obj1");
    Handle(AIS_AnimationObject) ais_ao = new AIS_AnimationObject("obj1", getContext(), anAisBox, start_pnt, end_pnt);
    ais_ao->SetOwnDuration(10);
    ais_ao->SetStartPts(0);

    ais_animation->Add(ais_ao);

    double duration = ais_animation->Duration();

    ais_animation->StartTimer(0, 1.0, true);

    while (!ais_animation->IsStopped())
    {
        ais_animation->UpdateTimer();

        getContext()->UpdateCurrentViewer();
    }
}

/*
void OccView::open_stepfile(){

    TopoDS_Shape ashape,shape0,shape1,shape2,shape3,shape4,shape5,shape6,shape7;
    TopoDS_Solid solid0,solid1,solid2,solid3,solid4,solid5,solid6,solid7;
    Handle(AIS_Shape) ais_shape0,ais_shape1,ais_shape2,ais_shape3,ais_shape4,ais_shape5,ais_shape6,ais_shape7;

    double deg_base,deg_joint1,deg_joint2,deg_joint3,deg_joint4,deg_joint5,deg_joint6,trans_x,trans_y,trans_z,endeffector_x,endeffector_y,endeffector_z,euler_x,euler_y,euler_z;
    gp_Trsf myTrsf0,myTrsf1,myTrsf2,myTrsf3,myTrsf4,myTrsf5,myTrsf6;

    STEPControl_Reader Reader0, Reader1, Reader2, Reader3, Reader4, Reader5, Reader6;

    IFSelect_ReturnStatus AAA;
    AAA = Reader0.ReadFile(":/robot/kuka_base.step");
    Reader1.ReadFile(":/robot/kuka_joint_1.step");
    Reader2.ReadFile(":/robot/kuka_joint_2.step");
    Reader3.ReadFile(":/robot/kuka_joint_3.step");
    Reader4.ReadFile(":/robot/kuka_joint_4.step");
    Reader5.ReadFile(":/robot/kuka_joint_5.step");
    Reader6.ReadFile(":/robot/kuka_joint_6.step");

    Reader0.TransferRoots();

    std::cout << Reader0.TransferRoots() << " Reader0 roots transferred." << std::endl;
    //std::cout << Reader1.TransferRoots() << " Reader1 roots transferred." << std::endl;
    //std::cout << Reader2.TransferRoots() << " Reader2 roots transferred." << std::endl;
    //std::cout << Reader3.TransferRoots() << " Reader3 roots transferred." << std::endl;
    //std::cout << Reader4.TransferRoots() << " Reader4 roots transferred." << std::endl;
    //std::cout << Reader5.TransferRoots() << " Reader5 roots transferred." << std::endl;
    //std::cout << Reader6.TransferRoots() << " Reader6 roots transferred." << std::endl;

    shape0=Reader0.OneShape();
    shape1=Reader1.OneShape();
    shape2=Reader2.OneShape();
    shape3=Reader3.OneShape();
    shape4=Reader4.OneShape();
    shape5=Reader5.OneShape();
    shape6=Reader6.OneShape();

    ais_shape0=new AIS_Shape(shape0); //floor
    ais_shape1=new AIS_Shape(shape1); //robot base
    ais_shape2=new AIS_Shape(shape2); //robot base
    ais_shape3=new AIS_Shape(shape3); //robot base
    ais_shape4=new AIS_Shape(shape4); //robot base
    ais_shape5=new AIS_Shape(shape5); //robot base
    ais_shape6=new AIS_Shape(shape6); //robot base

    myContext->Display(ais_shape0,Standard_True);
    myContext->Display(ais_shape1,Standard_True);
    myContext->Display(ais_shape2,Standard_True);
    myContext->Display(ais_shape3,Standard_True);
    myContext->Display(ais_shape4,Standard_True);
    myContext->Display(ais_shape5,Standard_True);
    myContext->Display(ais_shape6,Standard_True);

    myView->FitAll();
}
*/

void OccView::create_objects(){
    shape0 = BRepPrimAPI_MakeBox(3.0, 4.0, 5.0).Shape();
    gp_Ax2 a;
    a.SetLocation(gp_Pnt(0.0, 10.0, 0.0));
    shape1 = BRepPrimAPI_MakeBox(a, 3.0, 4.0, 5.0 ).Shape();

    ais_shape0 = new AIS_Shape(shape0);
    ais_shape1 = new AIS_Shape(shape1);

    getContext()->Display(ais_shape0, Standard_True);
    getContext()->Display(ais_shape1, Standard_True);
}



void OccView::update_joint_slider(){
    for(int i = 0; i < 200; i++){

    myTrsf0.SetRotation(gp_Ax1(gp_Pnt(1.5,0,0),gp_Dir(0,1,0)),i * M_PI /180);
    myTrsf1.SetRotation(gp_Ax1(gp_Pnt(0,10.0,0),gp_Dir(0,0,1)),deg_joint1 * M_PI /180);

    ais_shape0->SetLocalTransformation(myTrsf0);
    ais_shape1->SetLocalTransformation(myTrsf0*myTrsf1);

    getContext()->CurrentViewer()->Redraw();
    }

}
