/*
*    Copyright (c) 2018 Shing Liu All Rights Reserved.
*
*           File : occQt.h
*         Author : Shing Liu(eryar@163.com)
*           Date : 2018-01-08 20:00
*        Version : OpenCASCADE7.2.0 & Qt5.7.1
*
*    Description : OpenCASCADE in Qt.
*/

#ifndef OCCQT_H
#define OCCQT_H

#include "ui_occQt.h"
#include "myanimation.h"

#include <AIS_InteractiveContext.hxx>

#include <AIS_Shape.hxx>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QTimer>
#include <QDebug>
#include <QHBoxLayout>

class OccView;

//! Qt main window which include OpenCASCADE for its central widget.
class occQt : public QMainWindow
{
    Q_OBJECT

public:
    //! constructor/destructor.
    occQt(QWidget *parent = nullptr);
    ~occQt();

protected:
    //! create all the actions.
    void createActions();

    //! create all the menus.
    void createMenus();

    //! create the toolbar.
    void createToolBars();

    //! create the MainWidget.
    void createMainWidget();

private slots:
    //! show about box.
    void about();

private:
    QLabel* lable_q1;
    QLabel* lable_q2;
    QLabel* lable_q3;
    QLabel* lable_q4;
    QLabel* lable_q5;
    QLabel* lable_q6;
    QLabel* lable_time;
    QLineEdit* lineEdit_q1;
    QLineEdit* lineEdit_q2;
    QLineEdit* lineEdit_q3;
    QLineEdit* lineEdit_q4;
    QLineEdit* lineEdit_q5;
    QLineEdit* lineEdit_q6;
    QLineEdit* lineEdit_time;
    QPushButton* ButtonStartAinmation;


    Ui::occQtClass ui;

    // wrapped the widget for occ.
    OccView* myOccView;

    std::array<gp_Trsf,6> MyTrsf;

    //QThread* t1;
};

#endif // OCCQT_H
