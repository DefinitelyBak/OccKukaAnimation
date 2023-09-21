#ifndef OCCQT_H
#define OCCQT_H

#include "ui_occQt.h"

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

    //
    void PushButton();

signals:
    void InitAnimation(std::array<double, 8>);

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

    //
};

#endif // OCCQT_H
