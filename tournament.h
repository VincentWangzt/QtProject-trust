#pragma once
#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QSignalMapper>
#include <QMutex>
#include <QThread>
#include <QRandomGenerator>
#include <QPushButton>
#include "judge.h"
#include "pg_allplayers.h"

class Tournament;

class Tournament_Worker : public QObject{
    Q_OBJECT
public:
    int PlayerNum;
    bool start_flag;//是否处于开始状态
    bool continue_flag;//程序是否需要继续运行
    int step_flag;//程序计算到哪一个阶段了
        //0表示尚未开始，1表示LetThemIn,2表示主运算，3表示LetThemOut
    // 3 steps
    QSharedPointer<QMutex> flag_mutex;
    QSharedPointer<QMutex> mutex;
    //QSharedPointer<QMutex> update_mutex;
    QList< QSharedPointer<Player> > player_pool;//tournament参赛者
    QSharedPointer<Judge> judge;

    //PlyerTypeNum
    int type_num=4;
    QVector<int> PlayerTypeNum;//各个类别选手的数量

    //ValMatrix
    QVector<QVector<QVector<int>>> ValMatrix;//mat(i,j,k)表示在双方的(i,j)选择下选择k的得分(0-left 1-right)

    //rule:
    int num_games;//每场锦标赛的轮数
    int Elim_num;//每轮的淘汰人数
    int Probability;//犯错概率，单位%

    //widget
    Tournament* tournament;

    QList<QSharedPointer<Player>> Winner_list,Elim_list;
    explicit Tournament_Worker(int _PlayerNum,Tournament* par);
    void one_vs_one(int id1,int id2);//1-1比赛
    void LetThemIn();
    void Competition();
    void KickThemOut();
    void Button_OnPush(int index);
    void Tournament_Round();
    void reset();
    void Work_OnStep(int step);
    void Set_step(int step);
    void Set_flag(int step,bool flag_conti=true,bool flag_start=true);//flag指continue_flag
    bool Get_flag();//获取continue_flag
    int Get_step();
signals:
    void Update_signal();
    void Start_signal();
};

class Sandbox_ui;

class Tournament : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int highlight_index READ get_highlight_index WRITE set_highlight_index NOTIFY highlight_index_changed)
public:
    explicit Tournament(pg_allplayers* mui,QWidget *parent = nullptr);
    ~Tournament();
    QSharedPointer<QMutex> mutex;
    //QSharedPointer<QMutex> update_mutex;
    Tournament_Worker *Worker;
    static int PlayerNum;
    //选手类别数量部分
    static int type_number;//类别的数量
    static QVector<int> Init_PlayerTypeNum;//我随便设的，看你们ui怎么设置比较方便
    QVector<int> PlayerTypeNum_cache;//各个类别选手的数量
    QVector<QSlider*> Player_slider;//选手类别的滑动条
    QVector<slider*> Player_slider_ui;
    QSharedPointer<QSignalMapper> PlayerTypeNum_signal;
    QVector<int> Order_change;//辅助量，用于PlayerNum_Change函数

    QVector<Connection_Line*> connections;
    void clear_connections();
    int highlight_index;
    Trash_Can trash_can;

    int get_highlight_index();
    void set_highlight_index(int h_i);

    void paintEvent(QPaintEvent* event);

    //value-matrix部分
    QSharedPointer<Judge> judge;
    static int box_pair[8];
    static int Init_ValMatrix[2][2][2];
    QVector<QVector<QVector<int>>> ValMatrix_cache;//mat(i,j,k)表示在双方的(i,j)选择下选择k的得分
    QVector<QVector<QVector<QSpinBox*>>> ValMatrix_spinbox;
    QSharedPointer<QSignalMapper> ValMatrix_signal;

    //rule部分
    static int Init_num_games,Init_Elim_num,Init_Probility;
    int num_games_cache;//每场锦标赛的轮数
    int Elim_num_cache;//每轮的淘汰人数
    int Probility_cache;//犯错概率，单位%
    QSlider* NumGame_slider;
    QSpinBox* NumGame_spinbox;
    QSlider* ElimNum_slider;
    QSpinBox *ElimNum_spinbox_1,*ElimNum_spinbox_2;
    QSlider* Prob_slider;
    QSpinBox* Prob_spinbox;
    QSharedPointer<QSignalMapper> Rule_signal;

    //控制部分
    QPushButton* Start_button;
    QPushButton* Step_button;
    QPushButton* Reset_button;
    QSharedPointer<QSignalMapper> Control_signal;

    QList< QSharedPointer<Player> > player_pool;//tournament参赛者,保留下来了,在工作线程没用到这个

    //新线程：Tournament_Worker在这个线程中运行
    //QSharedPointer<QThread> Worker_Thread;

    void reset();//重设值函数
    void one_vs_one(int id1,int id2);//1-1比赛
    void one_vs_all(int id);//1-all比赛
    void display_log();//似乎是debug用的？

signals:
    void highlight_index_changed(int);

public slots:
    //注：所有的change函数都只负责对cache更新，当耗时运算完成后会emit更新信号完成更新
    void PlayerNum_Change(int index);//选手类别人数变更槽函数
    void ValueMatrix_Change(unsigned int index);//价值矩阵的更改(index为一个3位2进制数代表(i,j,k))
    void Rule_Change(int index);//rule部分改变的函数

    void Update();//更新，把cache中的数据读入到Worker中



signals:
    void Connect_signal();//链接PlayerTypeNum到Control_signal
    //void Update_signal();//用于发出更新指令，调用Update函数
};

#endif // TOURNAMENT_H
