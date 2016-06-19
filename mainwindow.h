#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QString>
#include <QGraphicsSceneMouseEvent>
#include <QPushButton>
#include <QList>
#include <QDesktopWidget>
#include <QTimer>
#include <Box2D/Box2D.h>
#include <QMouseEvent>
#include <iostream>
#include <ctime>
#include <cstdlib>

#include <gameitem.h>
#include <land.h>
#include <bird_original.h>
#include <bird_black.h>
#include <bird_blue.h>
#include <bird_yellow.h>
#include <bird_pokemon.h>
#include <pig.h>
#include <obstacle.h>
#include "mycontactlistener.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showEvent(QShowEvent *);
    bool eventFilter(QObject *,QEvent *event);
    void closeEvent(QCloseEvent *);
    enum Bird_type{start,killed,random,original,black,yellow,blue,pokemon,ogre};
signals:
    // Signal for closing the game
    void quitGame();

private slots:
    void tick();
    void zoomIn();
    void followBird();
    void moniterItem();
    void mapBird();
    void createBird(Bird_type type=random);
    void showScore();
    void restart();
    void quitgame();
    // For debug slot
    void QUITSLOT();

private:
    QPushButton *button,*button1;
    MyContactListener listener;
    Bird_type nowBird=start;
    enum holdState{hold,release,shooted};
    holdState nowHoldState=release;
    QPointF mousePoint;
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    b2World *world;
    QList<GameItem *> itemList,birdList,pigList;
    QTimer timer,timer_z,timer_f,timer_mouse,timer_animate,timer_waiter;
    int event_ctr,timer_zctr=0;
    int temp_x,temp_y;
    int pos_x=120,pos_y=580,score_temp,birdNum=4;
    QPixmap scorenumberPic[10];
    QList<QGraphicsPixmapItem *> scorenumbers;

    double scaleFactor = 1.15;
    double zoomRate=1;
    bool zoom_in=true;

    void createStage();
    void shoot();
    void result();
};

#endif // MAINWINDOW_H
