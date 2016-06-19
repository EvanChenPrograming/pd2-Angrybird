#include "obstacle.h"

Obstacle::Obstacle(float x, float y, float w, float h,float density,float friction,float restitution,float force, QTimer *timer, QPixmap pixmap,QPixmap pix2, b2World *world, QGraphicsScene *scene):GameItem(world)
{
    // Set pixmap
    g_pixmap.setPixmap(pixmap);
    g_pixmap.setTransformOriginPoint(g_pixmap.boundingRect().width()/2,g_pixmap.boundingRect().height()/2);
    g_size = QSize(w,h);

    // Create Body
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.position.Set(x,y);
    bodydef.userData = this;
    g_body = world->CreateBody(&bodydef);
    b2PolygonShape bodyBox;
    bodyBox.SetAsBox(w,h);
    b2FixtureDef fixturedef;
    fixturedef.shape = &bodyBox;
    fixturedef.density = density;
    fixturedef.friction = friction;
    fixturedef.restitution = restitution;
    g_body->SetAngularDamping(3);
    g_body->CreateFixture(&fixturedef);

    // Bound timer
    connect(timer, SIGNAL(timeout()), this,SLOT(paint()));

    scene->addItem(&g_pixmap);
    f=force;
    pix=pix2;
}

    void Obstacle::hit(float impulse){
        if(impulse>f)now_phase=destroy;
        else if(impulse>f/2){
            if(now_phase==broke)
                now_phase=destroy;
            else{
                now_phase=broke;
                g_pixmap.setPixmap(pix);
            }
        }
    }
