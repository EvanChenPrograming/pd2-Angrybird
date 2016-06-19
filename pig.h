#ifndef PIG_H
#define PIG_H

#include <gameitem.h>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>

#define PIG_DENSITY 10.0f
#define PIG_FRICTION 0.2f
#define PIG_RESTITUTION 0.5f

class Pig : public GameItem
{
public:
    Pig(float x, float y, float radius,float density,float friction,float restitution,float force, QTimer *timer, QPixmap *pixmap,int pics,b2World *world, QGraphicsScene *scene);
    void hit(float impulse);

private:
    float f;
    QPixmap *pix;
    int Pctr,ctr=0;
};

#endif // PIG_H
