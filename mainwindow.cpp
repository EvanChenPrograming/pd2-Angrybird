#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Enable the event Filter
    qApp->installEventFilter(this);
    event_ctr=0;
    mousePoint.setX(120);
    mousePoint.setY(580);
    temp_x=120;temp_y=580;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *)
{
    // Setting the QGraphicsScene
    scene = new QGraphicsScene(0,0,width()*2,height()*2);
    QPixmap bg;
    bg.load(":/image/backgroung.png");
    bg = bg.scaled(width()*2,height()*2);
    scene->addPixmap(bg);
    ui->graphicsView->setScene(scene);
    ui->graphicsView-> scale(0.5,0.5);
    for(int i=0;i<10;i++){
        scorenumberPic[i].load("://image/number_"+QString::number(i)+".png");
        scorenumberPic[i]=scorenumberPic[i].scaled(scorenumberPic[i].width()*4,scorenumberPic[i].height()*4);
    }
    button = new QPushButton("",this);
    QPixmap icon;
    icon.load(":/image/restart.png");
    icon = icon.scaled(800,800);
    button->setIcon(icon);
    button->setIconSize(QSize(70,70));
    button->setGeometry(10,10,70,70);
    button->setFlat(true);
    button->show();
    connect(button,SIGNAL(clicked(bool)),this,SLOT(restart()));

    button1 = new QPushButton("",this);
    icon.load(":/image/exit.png");
    icon = icon.scaled(800,800);
    button1->setIcon(icon);
    button1->setIconSize(QSize(70,70));
    button1->setGeometry(100,10,70,70);
    button1->setFlat(true);
    button1->show();
    connect(button1,SIGNAL(clicked(bool)),this,SLOT(quitgame()));

    // Create world
    world = new b2World(b2Vec2(0.0f, -9.8f));
    world->SetContactListener(&listener);
    // Setting Size
    GameItem::setGlobalSize(QSizeF(32,18),size());
    // Create ground (You can edit here)
    itemList.push_back(new Land(32,-17,64,0,QPixmap(":/ground.pn").scaled(width()*2,height()/6.0),world,scene));
    //itemList.push_back(new Land(32,1.5,3,35,QPixmap(":/ground.pn").scaled(width(),height()/6.0),world,scene));//r bound
    //itemList.push_back(new Land(0,1.5,3,35,QPixmap(":/ground.pn").scaled(width(),height()/6.0),world,scene));//l bound
    createStage();
    createBird(blue);
    // Timer
    connect(&timer,SIGNAL(timeout()),this,SLOT(tick()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(showScore()));
    timer.start(100/6);
    connect(this,SIGNAL(quitGame()),this,SLOT(QUITSLOT()));

    connect(&timer_z,SIGNAL(timeout()),this,SLOT(zoomIn()));
    connect(&timer_f,SIGNAL(timeout()),this,SLOT(followBird()));
    connect(&timer_waiter,SIGNAL(timeout()),this,SLOT(createBird()));
}


void MainWindow::createBird(Bird_type type){
    if(birdNum<=0){

        return;
    }
    if(type == random){
        srand(time(NULL));
        int ran=rand()%4;
        switch (ran) {
        case 0:
            type=original;
            break;
        case 1:
            type=yellow;
            break;
        case 2:
            type=black;
            break;
        case 3:
            type=blue;
            break;
        default:
            break;
        }
    }

    nowHoldState=release;
    GameItem *birdie;
    switch (type) {
    case original:
        birdie = new Bird_original(3.3f,2.7f,0.5f,&timer,QPixmap(":/bird.png").scaled(height()/15.0,height()/15.0),world,scene);
        nowBird=original;
        break;
    case yellow:
        birdie = new Bird_yellow(3.3f,2.7f,0.5f,&timer,QPixmap(":/image/yellow.png").scaled(height()/15.0,height()/15.0),world,scene);
        birdie->inWaterD=-0.5;
        nowBird=yellow;
        break;
    case black:
        birdie = new Bird_black(3.3f,2.7f,0.5f,&timer,QPixmap(":/image/black.png").scaled(height()/15.0,height()/12.0),world,scene);
        nowBird=black;
        birdie->inWaterD=0;
        break;
    case blue:
        birdie = new Bird_blue(3.3f,2.7f,0.5f,&timer,QPixmap(":/image/blue.png").scaled(height()/15.0,height()/15.0),world,scene,birdList);
        nowBird=blue;
        birdie->inWaterD=-2;
        break;
    default:
        break;
    }
    birdie->getBody().SetGravityScale(0);
    birdList.push_back(birdie);
    birdNum--;
    mousePoint.setX(120);
    mousePoint.setY(580);
    pos_x=120;pos_y=580;
    timer_waiter.stop();
}

void MainWindow::createStage(){
    QPixmap pic(":/image/pillar.png"),pic2;
    pic = pic.scaled(pic.width()*2,pic.height()*3);
    itemList.push_back(new Land(35,-10,1.5,6.5,pic,world,scene));
    itemList.push_back(new Land(60,-10,1.5,6.5,pic,world,scene));
    GameItem *sticks,*pigs;
    pic.load(":/image/stonestick0.png");
    pic = pic.scaled(14*90,pic.height());
    pic2.load(":/image/stonestick2.png");
    pic2 = pic2.scaled(14*90,pic.height());
    sticks=new Obstacle(46,-3,13,0.1,30,1.0,0,1000,&timer,pic,pic2,world,scene);
    sticks->inWaterD=0.1;
    itemList.push_back(sticks);

    //wood
    pic.load(":/image/thinwood0.png");
    pic = pic.scaled(pic.width()*1.2,pic.height());
    pic2.load(":/image/thinwood1.png");
    pic2 = pic2.scaled(pic2.width()*1.2,pic2.height());
    sticks=new Obstacle(44,0,3,0.5,10,1.0,0.1,600,&timer,pic,pic2,world,scene);
    sticks->getBody().SetTransform(b2Vec2(44,0),3.1415/2);
    sticks->inWaterD=-0.6;
    itemList.push_back(sticks);
    sticks=new Obstacle(60,0,3,0.5,10,1.0,0.1,600,&timer,pic,pic2,world,scene);
    sticks->getBody().SetTransform(b2Vec2(60,0),3.1415/2);
    sticks->inWaterD=-0.6;
    itemList.push_back(sticks);
    sticks=new Obstacle(51,1,3,0.5,10,1.0,0.1,600,&timer,pic,pic2,world,scene);
    sticks->getBody().SetTransform(b2Vec2(51,0),3.1415/2);
    sticks->inWaterD=-0.6;
    itemList.push_back(sticks);
    sticks=new Obstacle(35,1,3,0.5,10,1.0,0.1,600,&timer,pic,pic2,world,scene);
    sticks->getBody().SetTransform(b2Vec2(35,0),3.1415/2);
    sticks->inWaterD=-0.6;
    itemList.push_back(sticks);

    //stone
    pic.load(":/image/stonestick0.png");
    pic = pic.scaled(pic.width()*2,pic.height()*2);
    pic2.load(":/image/stonestick2.png");
    pic2 = pic2.scaled(pic2.width()*2,pic2.height()*2);
    sticks=new Obstacle(39.5,3,5,0.4,30,1.0,0,1000,&timer,pic,pic2,world,scene);
    sticks->inWaterD=0.5;
    itemList.push_back(sticks);
    sticks=new Obstacle(55.5,3,5,0.4,30,1.0,0,1000,&timer,pic,pic2,world,scene);
    sticks->inWaterD=0.5;
    itemList.push_back(sticks);

    //ice
    pic.load(":/image/ice0.png");
    pic = pic.scaled(pic.width()*1.2,pic.height());
    pic2.load(":/image/ice2.png");
    pic2 = pic2.scaled(pic2.width()*1.2,pic2.height());
    sticks=new Obstacle(43,7.5,3,0.5,0.8,0.1,0.1,300,&timer,pic,pic2,world,scene);
    sticks->getBody().SetTransform(b2Vec2(43,7.5),3.1415/2);
    sticks->inWaterD=-2;
    itemList.push_back(sticks);
    sticks=new Obstacle(52,7.5,3,0.5,0.8,0.1,0.1,300,&timer,pic,pic2,world,scene);
    sticks->getBody().SetTransform(b2Vec2(52,7.5),3.1415/2);
    sticks->inWaterD=-2;
    itemList.push_back(sticks);
    pic.load(":/image/ice0.png");
    pic = pic.scaled(pic.width()*2.1,pic.height()*2);
    pic2.load(":/image/ice2.png");
    pic2 = pic2.scaled(pic2.width()*2.1,pic2.height()*2);
    sticks=new Obstacle(47,11.4,5,0.5,0.8,0,0.1,300,&timer,pic,pic2,world,scene);
    sticks->inWaterD=-2;
    itemList.push_back(sticks);
    sticks=new Obstacle(16,-3,5,0.5,0.8,0,0.1,300,&timer,pic,pic2,world,scene);
    sticks->inWaterD=-5;
    itemList.push_back(sticks);

    //pigs
    QPixmap *p;
    p=new QPixmap[5];
    for(int i=0;i<5;i++){
        p[i].load(":/image/berd"+QString::number(i)+".png");
    }
    pigs=new Pig(40,-1.7,1,10,1.0,0.5,500,&timer,p,5,world,scene);
    pigs->inWaterD=0.6;
    itemList.push_back(pigs);

    p=new QPixmap[2];
    for(int i=0;i<2;i++){
        p[i].load(":/image/pig"+QString::number(i)+".png");
    }
    pigs=new Pig(56,-1.8,1,10,1.0,0.5,500,&timer,p,2,world,scene);
    pigs->inWaterD=0.6;
    itemList.push_back(pigs);

    p=new QPixmap[3];
    for(int i=0;i<3;i++){
        p[i].load(":/image/helmetPig"+QString::number(i)+".png");
    }
    pigs=new Pig(16,-1.8,0.8,15,1.0,0.5,500,&timer,p,3,world,scene);
    pigs->inWaterD=0.6;
    itemList.push_back(pigs);

    p=new QPixmap[3];
    for(int i=0;i<3;i++){
        p[i].load(":/image/king"+QString::number(i)+".png");
        p[i] = p[i].scaled(p[i].width()*2,p[i].height()*2);
    }
    pigs=new Pig(48,-14,3,10,1.0,0.5,2000,&timer,p,3,world,scene);
    pigs->inWaterD=0.6;
    itemList.push_back(pigs);

    //rock
    p=new QPixmap[3];
    for(int i=0;i<3;i++){
        p[i].load(":/image/rock"+QString::number(i)+".png");
    }
    pigs=new Pig(48,12.5,0.8,30,1.0,0.5,2000,&timer,p,3,world,scene);
    pigs->inWaterD=0.6;
    itemList.push_back(pigs);

    //gold egg
    //pigs = new Bird_pokemon
}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        event_ctr++;
        QKeyEvent *KeyEvent = (QKeyEvent*)event;
        if(KeyEvent->key()==Qt::Key_R && event_ctr%5==1){
            std::cout<<"R"<<std::endl;
            createBird();
        }
        if(KeyEvent->key()==Qt::Key_Q){
            restart();
        }

        if(KeyEvent->key()==Qt::Key_Equal){
            ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
            ui->graphicsView-> scale(scaleFactor, scaleFactor);
        }
        if(KeyEvent->key()==Qt::Key_Minus){
            ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
            ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
        if(KeyEvent->key()==Qt::Key_Down){
            timer_z.start(10);
        }
        if(KeyEvent->key()==Qt::Key_Space){
            timer_z.stop();
            timer_f.stop();
            ui->graphicsView->setTransform(QTransform(0.5, 0, 0, 0.5, 0, 0));
            ui->graphicsView->centerOn(600,300);
        }
    }
    if(event->type() == QEvent::GraphicsSceneMousePress)
    {
        if(nowHoldState==release){
            nowHoldState=hold;
        }
        else if(!birdList.isEmpty()){
            birdList.back()->controled();
        }


    }
    if(event->type() == QEvent::GraphicsSceneMouseMove)
    {
        QGraphicsSceneMouseEvent  *mouseEvent = (QGraphicsSceneMouseEvent *)event;
        mousePoint=mouseEvent->scenePos();
    }
    if(event->type() == QEvent::GraphicsSceneMouseRelease)
    {
        std::cout << "Release !" <<std::endl ;
        if(nowHoldState==hold){
            timer_waiter.start(10000);
            shoot();
        }

    }
    return false;
}

void MainWindow::zoomIn(){
    ui->graphicsView->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->graphicsView-> scale(1.01, 1.01);
    timer_zctr++;
    if(timer_zctr>=60){timer_z.stop();timer_zctr=0;}
}

void MainWindow::followBird(){
    if(birdList.isEmpty()){timer_f.stop();return;}
    QPointF mappedPoint=birdList.back()->getPoint();/*
    if(zoom_in==true)zoomRate+=0.003;
    if(zoom_in==false)zoomRate-=0.01;
    if(zoomRate>=1.5)zoom_in=false;
    if(zoomRate<=0.8){
        zoom_in=true;
        ui->graphicsView->setTransform(QTransform(0.5, 0, 0, 0.5, 0, 0));
        ui->graphicsView->centerOn(600,300);
        timer_f.stop();
    }*/
    ui->graphicsView->setTransform(QTransform(zoomRate, 0, 0, zoomRate, 0, 0));
    ui->graphicsView->centerOn(mappedPoint);
    if(!birdList.back()->getBody().IsAwake()||mappedPoint.x()>3060||mappedPoint.y()>1840){
        if(mappedPoint.x()>3060||mappedPoint.y()>1840)birdList.back()->now_phase=GameItem::destroy;
        timer_f.stop();
        zoom_in=true;
        ui->graphicsView->setTransform(QTransform(0.5, 0, 0, 0.5, 0, 0));
        ui->graphicsView->centerOn(600,300);

    }

}

void MainWindow::shoot(){
    nowHoldState=shooted;
    if(!birdList.isEmpty()||nowBird!=killed){
        std::cout << "x:"<<birdList.back()->getBody().GetPosition().x<<"       y: "<<birdList.back()->getBody().GetPosition().y<< std::endl ;
        birdList.back()->getBody().SetGravityScale(1.0);
        birdList.back()->getBody().SetLinearVelocity(b2Vec2(-(mousePoint.x()-130)/5,(mousePoint.y()-580)/5));
        timer_f.start(100/6);
    }
}

void MainWindow::mapBird(){
    //birdList.back()->setPoint(100,100);
    if(birdList.isEmpty() || nowHoldState==release)return;
    //QPointF pos=birdList.back()->getPoint();

    //pos_x=pos.x()>=120?pos.x()-120:120-pos.x();
    //pos_y=pos.y()>=580?pos.y()-580:580-pos.y();
    pos_x=mousePoint.x()>120?mousePoint.x()-120:mousePoint.x()+120;
    pos_y=mousePoint.y()>580?mousePoint.y()-580:mousePoint.y()+580;
    if((pos_x*pos_x+pos_y*pos_y)>25000){
        birdList.back()->setPoint(temp_x,temp_y);
    }
    else{
        temp_x=mousePoint.x();
        temp_y=mousePoint.y();
        birdList.back()->setPoint(temp_x,temp_y);
    }

    //birdList.back()->setPoint(mousePoint.x(),mousePoint.y());
}

void MainWindow::moniterItem(){
    foreach (GameItem *i, itemList) {
        if(i->now_phase==GameItem::destroy){
            itemList.removeOne(i);
            delete i;
        }
        if(i->getBody().GetPosition().y<-3.75){
            if(i->inWater==false)
            {
                i->inWater=true;
                //float D=i->getBody().GetMass()/10*3;
                if(i->inWaterD<0){
                    i->getBody().SetGravityScale(i->inWaterD);
                    i->getBody().SetLinearDamping(6);
                }
                else i->getBody().SetLinearDamping(i->inWaterD);
            }
        }
        else if(i->inWater==true){
            i->inWater=false;
            i->getBody().SetLinearDamping(0);
            i->getBody().SetGravityScale(1);
        }
    }
    foreach (GameItem *i, birdList) {
        if(i->getPoint().x()>3060||i->getPoint().y()>1840)
            i->now_phase=GameItem::destroy;
        if(i->now_phase==GameItem::destroy){
            birdList.removeOne(i);
            delete i;
            temp_x=120;temp_y=580;
            nowBird=killed;
        }
        if(i->getBody().GetPosition().y<-3.75){
            if(i->inWater==false)
            {
                i->inWater=true;
                if(i->inWaterD<0){
                    i->getBody().SetGravityScale(i->inWaterD);
                    i->getBody().SetLinearDamping(6);
                }
                else i->getBody().SetLinearDamping(i->inWaterD);
            }
        }
        else if(i->inWater==true){
            i->inWater=false;
            i->getBody().SetLinearDamping(0);
            i->getBody().SetGravityScale(1);
        }
    }
    if(nowHoldState==hold)mapBird();
}

void MainWindow::showScore(){
    if(listener.score==score_temp/100)return;
    score_temp=listener.score*100;
    if(score_temp>=10000000)return;
    foreach (QGraphicsPixmapItem *i, scorenumbers) {
        scene->removeItem(i);
    }
    scorenumbers.clear();
    int x=1680;
    QString com;
    com=QString::number(score_temp);
    for(int i=com.size()-1;i>=0;i--){
        QGraphicsPixmapItem *N;
        N = new QGraphicsPixmapItem;
        int c=com.at(i).digitValue();
        N->setPixmap(scorenumberPic[c]);
        N->setPos(x,50);
        x-=100;
        scene->addItem(N);
        scorenumbers.push_front(N);
    }
}

void MainWindow::restart(){
    timer.stop();
    timer_z.stop();
    timer_f.stop();
    foreach (GameItem *i, birdList) {
        delete i;
    }
    foreach (GameItem *i, itemList) {
        delete i;
    }
    birdList.clear();
    itemList.clear();
    delete world;

    world = new b2World(b2Vec2(0.0f, -9.8f));
    world->SetContactListener(&listener);
    GameItem::setGlobalSize(QSizeF(32,18),size());
    itemList.push_back(new Land(32,-17,64,0,QPixmap("null"),world,scene));
    birdNum=4;
    createStage();
    createBird(original);
    listener.score=0;
    timer.start(100/6);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    // Close event
    emit quitGame();
}

void MainWindow::tick()
{
    moniterItem();
    world->Step(1.0/60.0,6,2);
    scene->update();
}

void MainWindow::quitgame(){
    exit(0);
    emit quitGame();
}

void MainWindow::QUITSLOT()
{
    // For debug
    exit(1);
    std::cout << "Quit Game Signal receive !" << std::endl ;
}
