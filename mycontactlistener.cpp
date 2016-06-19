#include "mycontactlistener.h"

/*
void MyContactListener::BeginContact(b2Contact* contact) {

    std::cout<<"start contact"<<std::endl;
     //check if fixture A was a ball
    void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
    if ( bodyUserData )
        static_cast<GameItem *>( bodyUserData )->hit(0);

    //check if fixture B was a ball
    bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
    if ( bodyUserData )
        static_cast<GameItem *>( bodyUserData )->hit(0);

}*/

void MyContactListener::PostSolve(b2Contact *contact,const b2ContactImpulse *impulse){
    //std::cout<<"pass impulse:"<<impulse->normalImpulses[0]<<std::endl;
    void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
    if ( bodyUserData ){
        static_cast<GameItem *>( bodyUserData )->hit(impulse->normalImpulses[0]);
        score+=(int)impulse->normalImpulses[0]/100;
    }

    bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
    if ( bodyUserData ){
        static_cast<GameItem *>( bodyUserData )->hit(impulse->normalImpulses[0]);
        score+=(int)impulse->normalImpulses[0]/100;
    }
}
