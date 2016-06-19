#ifndef BIRD_YELLOW_H
#define BIRD_YELLOW_H


#include <gameitem.h>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>

#define BIRD_DENSITY 10.0f
#define BIRD_FRICTION 0.2f
#define BIRD_RESTITUTION 0.3f

class Bird_yellow : public GameItem
{
public:
    Bird_yellow(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
    void setLinearVelocity(b2Vec2 velocity);
    void hit(float impulse);
    void controled();
};


#endif // BIRD_YELLOW_H
