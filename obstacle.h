#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <gameitem.h>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>

#define PIG_DENSITY 10.0f
#define PIG_FRICTION 0.2f
#define PIG_RESTITUTION 0.5f

class Obstacle : public GameItem
{
public:
    Obstacle(float x, float y, float w, float h,float density,float friction,float restitution,float force, QTimer *timer, QPixmap pixmap,QPixmap pix2, b2World *world, QGraphicsScene *scene);
    void hit(float impulse);

private:
    float f;
    QPixmap pix;

};

#endif // OBSTACLE_H
