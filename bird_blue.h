#ifndef BIRD_BLUE_H
#define BIRD_BLUE_H

#include <gameitem.h>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>

#define BIRD_DENSITY 10.0f
#define BIRD_FRICTION 0.2f
#define BIRD_RESTITUTION 0.5f

class Bird_blue : public GameItem
{
public:
    Bird_blue(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,QList<GameItem *> birdList);
    Bird_blue(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
    void setLinearVelocity(b2Vec2 velocity);
    void hit(float impulse);
    void controled();

    float tx;
    float ty;
    float tradius;
    QTimer *ttimer;
    QPixmap tpixmap;
    b2World *tworld;
    QGraphicsScene *tscene;
    QList<GameItem *> tbirdList;
};


#endif // BIRD_BLUE_H
