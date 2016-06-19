#include "pig.h"

Pig::Pig(float x, float y, float radius,float density,float friction,float restitution,float force, QTimer *timer, QPixmap *pixmap,int pics, b2World *world, QGraphicsScene *scene):GameItem(world)
{
    // Set pixmap
    pix = new QPixmap[pics];
    for(int i=0;i<pics;++i){
        pix[i]=pixmap[i];
    }
    g_pixmap.setPixmap(pixmap[0]);
    g_pixmap.setTransformOriginPoint(g_pixmap.boundingRect().width()/2,g_pixmap.boundingRect().height()/2);
    g_size = QSize(radius*2,radius*2);

    // Create Body
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.position.Set(x,y);
    bodydef.userData = this;
    g_body = world->CreateBody(&bodydef);
    b2CircleShape bodyshape;
    bodyshape.m_radius = radius;
    b2FixtureDef fixturedef;
    fixturedef.shape = &bodyshape;
    fixturedef.density = density;
    fixturedef.friction = friction;
    fixturedef.restitution = restitution;
    g_body->SetAngularDamping(3);
    g_body->CreateFixture(&fixturedef);

    // Bound timer
    connect(timer, SIGNAL(timeout()), this,SLOT(paint()));

    scene->addItem(&g_pixmap);
    f=force;
    Pctr=pics;
}

    void Pig::hit(float impulse){
        if(impulse>f)now_phase=destroy;
        else if(impulse>f/3){
            if(now_phase==broke)
                now_phase=destroy;
            else if(ctr<Pctr){
                ctr++;
                if(ctr==Pctr-1)now_phase=broke;
                g_pixmap.setPixmap(pix[ctr]);
            }
        }
    }
