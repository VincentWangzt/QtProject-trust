#include "pg_twoplayers.h"
#include "ui_pg_twoplayers.h"
#include<QTimer>
#include<QDebug>
#include<QPainter>
#include<QPropertyAnimation>
#include<QAbstractAnimation>
#include<QSequentialAnimationGroup>
#include<QParallelAnimationGroup>
#include<QTimer>
pg_twoplayers::pg_twoplayers(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::pg_twoplayers)
{
    ui->setupUi(this);
    ui->machine_00->hide();
    ui->machine_01->hide();
    ui->machine_10->hide();
    ui->machine_11->hide();
    ui->text2->hide();
    hiding_all();
}
void pg_twoplayers::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    //文字
    // QRect rec(169,10,399,208);
    // painter.drawPixmap(rec,QPixmap(":/image/what to do.png"));
}
//opponent的移动&组合
void pg_twoplayers::right_opponent(QLabel* opponent1){
    //右人
    opponent_f =new QPropertyAnimation(opponent1,"geometry");
    opponent_f->setStartValue(QRect(575,250,120,160));
    opponent_f->setKeyValueAt(0.25,QRect(552.5,240,120,160));
    opponent_f->setKeyValueAt(0.5,QRect(530,250,120,160));
    opponent_f->setKeyValueAt(0.75,QRect(507.5,240,120,160));
    opponent_f->setEndValue(QRect(485,250,120,160));
    opponent_f->setDuration(500);


    /*********后退************/
    //右人
    opponent_b =new QPropertyAnimation(opponent1,"geometry");
    opponent_b->setStartValue(QRect(485,250,120,160));
    opponent_b->setKeyValueAt(0.5,QRect(485,250,120,160));
    opponent_b->setKeyValueAt(0.625,QRect(507.5,240,120,160));
    opponent_b->setKeyValueAt(0.75,QRect(530,260,110,160));
    opponent_b->setKeyValueAt(0.875,QRect(552.5,240,120,160));
    opponent_b->setEndValue(QRect(575,250,120,160));
    opponent_b->setDuration(1000);

    //并行
    forwardGroup= new QParallelAnimationGroup(this);
    forwardGroup->addAnimation(user_f);
    forwardGroup->addAnimation(opponent_f);
    forwardGroup->addAnimation(coin_animation);
    forwardGroup->addAnimation(coin_animation1);
    //并行
    backwardGroup= new QParallelAnimationGroup(this);
    backwardGroup->addAnimation(user_b);
    backwardGroup->addAnimation(opponent_b);
    // backwardGroup->addAnimation(coin_animationup);
    // backwardGroup->addAnimation(coin_animation1up);
    backwardGroup->addAnimation(coin_animationb);
    backwardGroup->addAnimation(coin_animation1b);
    //整动画
    Group =new QSequentialAnimationGroup(this);
    Group->addAnimation(forwardGroup);
   Group->addPause(1000);
    Group->addAnimation(backwardGroup);
    Group->start();


    //连接变化
    connect(coin_animation,&QPropertyAnimation::finished,this,[=](){
        flag=1;
        ui->machine->hide();
        ui->left->hide();
        opponent1->hide();
        reaction(opponent1);//,match(Player_Pair players)
       // opponent->hide();
    });
    //人物恢复
    connect(Group,&QSequentialAnimationGroup::currentAnimationChanged,this,[=](){
        if(flag==1){
            qDebug()<<"change";
            flag=0;
        }
        else if(flag==0){
            hiding_all();
            ui->left->setGeometry(QRect(140,260,120,160));
            ui->left->show();
            opponent1->setGeometry(QRect(485,250,120,160));
            opponent1->show();
        }
    });

    connect(coin_animationb,&QPropertyAnimation::finished,this,[=](){
        ui->machine->show();
        ui->machine_00->hide();
        ui->machine_01->hide();
        ui->machine_10->hide();
        ui->machine_11->hide();

    });
}

//根据result变化
void pg_twoplayers::reaction(QLabel* opponent){//,MatchResult& result
        ui->machine->hide();

        // if(result.action[0]==0&&result.action[1]==0){
        ui->machine_00->show();
        ui->left_hate->setGeometry(QRect(140,260,120,160));
        ui->left_hate->show();
        ui->right_hate->setGeometry(QRect(485,250,120,160));
        ui->right_hate->show();
        // }
        // else if(result.action[0]==1&&result.action[1]==0){
        //     ui->machine_10->show();
        //     ui->left->hide();
        // ui->left_excited->setGeometry(QRect(140,260,120,160));
        // ui->left_excited->show();
        // ui->right_sad->setGeometry(QRect(485,250,120,160));
        // ui->right_sad->show();
        // }
        // else if(result.action[0]==0&&result.action[1]==1){
        //     ui->machine_01->show();
        //     ui->left->hide();
        // ui->left_sad->setGeometry(QRect(140,260,120,160));
        // ui->left_sad->show();
        // ui->right_excited->setGeometry(QRect(485,250,120,160));
        // ui->right_excited->show();
        // }
        // else if(result.action[0]==1&&result.action[1]==1){
        //      ui->machine_11->show();
        //      ui->left->hide();
        //      ui->left_happy->setGeometry(QRect(140,260,120,160));
        //      ui->left_happy->show();
        //      ui->right_happy->setGeometry(QRect(485,250,120,160));
        //      ui->right_happy->show();
        // }
        show_hat(opponent);

}
//表情变化统一ui，用帽子区分人物
void pg_twoplayers::show_hat(QLabel* opponent){
    if(opponent==ui->right_copycat) {
        ui->copycat_h->show();}
   else if(opponent==ui->right_cooperator) {
        ui->cooperator_h->show();}
    else if(opponent==ui->right_cheater) {
        ui->cheater_h->show();}
    else if(opponent==ui->right_grudger) {
        ui->grudger_h->show();}
    else if(opponent==ui->right_detective) {
        ui->detective_h->show();}

}

//左人移动
void pg_twoplayers::left_opponent(){
    /****前进*****/
    //左人
    user_f =new QPropertyAnimation(ui->left,"geometry");
    user_f->setStartValue(QRect(50,260,120,160));
    user_f->setKeyValueAt(0.25,QRect(72.5,250,120,160));
    user_f->setKeyValueAt(0.5,QRect(95,260,120,160));
    user_f->setKeyValueAt(0.75,QRect(117.5,250,120,160));
    user_f->setEndValue(QRect(140,260,120,160));
    user_f->setDuration(500);
    /*****后退*********/
    //左人
    user_b =new QPropertyAnimation(ui->left,"geometry");
    user_b->setStartValue(QRect(140,260,120,160));
    user_b->setKeyValueAt(0.5,QRect(140,260,120,160));
    user_b->setKeyValueAt(0.625,QRect(117.5,250,120,160));
    user_b->setKeyValueAt(0.75,QRect(95,260,120,160));
    user_b->setKeyValueAt(0.875,QRect(72.5,250,120,160));
    user_b->setEndValue(QRect(50,260,120,160));
    user_b->setDuration(1000);
}

//金币移动same
void pg_twoplayers::same_part(){
    //左硬币
    coin_animation =new QPropertyAnimation(ui->coin_left,"geometry");
    coin_animation->setStartValue(QRect(130,300,50,70));
    coin_animation->setKeyValueAt(0.125,QRect(152.5,290,50,70));
    coin_animation->setKeyValueAt(0.25,QRect(175,300,50,70));
    coin_animation->setKeyValueAt(0.375,QRect(197.5,290,50,70));
    coin_animation->setKeyValueAt(0.5,QRect(220,300,50,70));
    coin_animation->setEndValue(QRect(250,350,50,70));
    coin_animation->setDuration(1000);
    //右硬币
    coin_animation1 =new QPropertyAnimation(ui->coin_right,"geometry");
    coin_animation1->setStartValue(QRect(565,300,50,70));
    coin_animation1->setKeyValueAt(0.125,QRect(542.5,290,50,70));
    coin_animation1->setKeyValueAt(0.25,QRect(520,300,50,70));
    coin_animation1->setKeyValueAt(0.375,QRect(499.5,290,50,70));
    coin_animation1->setKeyValueAt(0.5,QRect(474.5,300,50,70));
    coin_animation1->setEndValue(QRect(452.5,350,50,70));
    coin_animation1->setDuration(1000);
    /*****后退*********/

    //左硬币
    coin_animationb =new QPropertyAnimation(ui->coin_left,"geometry");
    coin_animationb->setStartValue(QRect(250,350,50,70));
    coin_animationb->setKeyValueAt(0.875,QRect(152.5,290,50,70));
    coin_animationb->setKeyValueAt(0.75,QRect(175,300,50,70));
    coin_animationb->setKeyValueAt(0.625,QRect(197.5,290,50,70));
    coin_animationb->setKeyValueAt(0.5,QRect(220,300,50,70));
    coin_animationb->setEndValue(QRect(130,300,50,70));
    coin_animationb->setDuration(1000);
    //右硬币
    coin_animation1b =new QPropertyAnimation(ui->coin_right,"geometry");
    coin_animation1b->setStartValue(QRect(452.5,350,50,70));
    coin_animation1b->setKeyValueAt(0.875,QRect(542.5,290,50,70));
    coin_animation1b->setKeyValueAt(0.75,QRect(520,300,50,70));
    coin_animation1b->setKeyValueAt(0.625,QRect(499.5,290,50,70));
    coin_animation1b->setKeyValueAt(0.5,QRect(474.5,300,50,70));
    coin_animation1b->setEndValue(QRect(565,300,50,70));
    coin_animation1b->setDuration(1000);



}

void pg_twoplayers::first_opponent(){
    ui->right_copycat->show();
    same_part();
    left_opponent();
    right_opponent(ui->right_copycat);
    //qDebug()<<"in";
}

void pg_twoplayers::second_opponent(){
    ui->right_cheater->show();
    ui->right_copycat->hide();
    same_part();
    left_opponent();
    right_opponent(ui->right_cheater);
    //qDebug()<<"in";
}

void pg_twoplayers::third_opponent(){
    ui->right_cooperator->show();
    ui->right_cheater->hide();
    same_part();
    left_opponent();
    right_opponent(ui->right_cooperator);
    //qDebug()<<"in";
}

void pg_twoplayers::fourth_opponent(){
    ui->right_grudger->show();
    ui->right_cooperator->hide();
    same_part();
    left_opponent();
    right_opponent(ui->right_grudger);
    //qDebug()<<"in";
}

void pg_twoplayers::fifth_opponent(){
    ui->right_detective->show();
    ui->right_grudger->hide();
    same_part();
    left_opponent();
    right_opponent(ui->right_detective);
    //qDebug()<<"in";
}

pg_twoplayers::~pg_twoplayers()
{
    delete ui;
}


void pg_twoplayers::on_cheatButton_clicked()
{
    if(round<3){
        ui->text1->hide();
        ui->text2->show();
        first_opponent();
        //Group->start();
        qDebug()<<round;
    }
    else if(round>=3&&round<5){
          second_opponent();
        //Group_2->start();
    }
    else if(round>=5&&round<7){
        third_opponent();
    }
    else if(round>=7&&round<9){
        fourth_opponent();
    }
    else if(round>=9&&round<11){
        fifth_opponent();
    }
    round++;
    //animation->start();
   // animation_2->start();
   // coin_animation->start();
}


void pg_twoplayers::on_cooperateButton_clicked()
{
    if(round<3){
        first_opponent();
        //Group->start();
        ui->text1->hide();
        ui->text2->show();
        qDebug()<<round;
    }
    else if(round>=3&&round<5){
        second_opponent();
        //Group_2->start();
    }
    else if(round>=5&&round<7){
        third_opponent();
    }
    else if(round>=7&&round<9){
        fourth_opponent();
    }
    else if(round>=9&&round<11){
        fifth_opponent();
    }
    round++;
}

void pg_twoplayers::hiding_all(){
    ui->left_sad->hide();
    ui->left_hate->hide();
    ui->left_excited->hide();
    ui->left_happy->hide();

    ui->right_sad->hide();
    ui->right_hate->hide();
    ui->right_excited->hide();
    ui->right_happy->hide();

    ui->right_cheater->hide();
    ui->right_cooperator->hide();
    ui->right_grudger->hide();
    ui->right_detective->hide();

    ui->copycat_h->hide();
    ui->cheater_h->hide();
    ui->cooperator_h->hide();
    ui->grudger_h->hide();
    ui->detective_h->hide();
}
