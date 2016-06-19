#ifndef MYCONTACTLISTENER_H
#define MYCONTACTLISTENER_H

#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <iostream>
#include <gameitem.h>

class MyContactListener : public b2ContactListener
  {
public:
    void PostSolve(b2Contact *contact,const b2ContactImpulse *impulse);
    //void BeginContact(b2Contact* contact);
    int score=0;
  };


#endif // MYCONTACTLISTENER_H
