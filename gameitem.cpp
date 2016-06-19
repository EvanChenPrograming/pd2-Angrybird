#include "gameitem.h"


GameItem::GameItem(b2World *world):
    g_body(NULL),g_world(world)
{
}

GameItem::~GameItem()
{
    g_world->DestroyBody(g_body);
}

QSizeF GameItem::g_windowsize = QSizeF();
QSizeF GameItem::g_worldsize = QSizeF();

void GameItem::setGlobalSize(QSizeF worldsize, QSizeF windowsize)
{
    g_worldsize = worldsize;
    g_windowsize = windowsize;
}

void GameItem::paint()
{
    b2Vec2 pos = g_body->GetPosition();
    //std::cout << g_body->GetAngle() << std::endl;
    QPointF mappedPoint;
    mappedPoint.setX(((pos.x-g_size.width()/2) * g_windowsize.width())/g_worldsize.width()-g_pixmap.boundingRect().width()/4);
    mappedPoint.setY((1.0f - (pos.y+g_size.height()/2)/g_worldsize.height()) * g_windowsize.height()-g_pixmap.boundingRect().height()/4);
    g_pixmap.setPos(mappedPoint);
    g_pixmap.resetTransform();
    g_pixmap.setRotation(-(g_body->GetAngle()*180/3.14159));
}

b2Body &GameItem::getBody(){
    return *g_body;
};

QPointF GameItem::getPoint(){
    b2Vec2 pos = g_body->GetPosition();
    //std::cout << g_body->GetAngle() << std::endl;
    QPointF mappedPoint;
    mappedPoint.setX(((pos.x-g_size.width()/2) * g_windowsize.width())/g_worldsize.width());
    mappedPoint.setY((1.0f - (pos.y+g_size.height()/2)/g_worldsize.height()) * g_windowsize.height());
    return mappedPoint;
}

void GameItem::setPoint(float x,float y){
    float Box_x,Box_y;
    Box_x=x*g_worldsize.width()/g_windowsize.width()+g_size.width();
    Box_y=(1.0f-y/g_windowsize.height())*g_worldsize.height()-g_size.height()/2;
    g_body->SetTransform(b2Vec2(Box_x,Box_y),g_body->GetAngle());
}


void GameItem::hit(float impulse){
    std::cout<<"in gameitem"<<std::endl;
}
void GameItem::controled(){
    ;
}
