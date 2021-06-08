#ifndef TETRIS_H
#define TETRIS_H
#include<winsock2.h>
#include <QMainWindow>
#include<QVector>
QT_BEGIN_NAMESPACE
namespace Ui { class Tetris; }
QT_END_NAMESPACE

class Tetris : public QMainWindow
{
    Q_OBJECT

public:
    Tetris(QWidget *parent = nullptr);
    ~Tetris();

private:
    Ui::Tetris *ui;
    //成员对象
public:
    int start;
    long long  score=0;
    int timerId;
    QVector<QVector<QPair<bool,bool>>>block;//1:是否有方块 2:该方块是否正操作的
    QVector<QVector<QPair<int,bool>>>block_pic;//1:皮肤编号 2:该皮肤是否显示
    QVector<QVector<QPair<bool,bool>>>block_next;
    QVector<QVector<QPair<int,bool>>>block_next_pic;
    QVector<QPixmap>theme;
    bool Any=false;
    bool block_shadow_open=true;
    QPair<int,int>status;
    QPair<int,int>status_next=QPair<int,int>(7,9);
    int speed=1000;
    int grade=1;
public:
    //方块移动
    void new_block();
    void move_left();
    void move_right();
    void move_down();
    void clean();
    void get_new(QVector<QVector<QPair<bool,bool>>>&block,QPair<int,int>&);
    void block_change();
    //方块变化
    void I_change();
    void W_change();
    void S_change();
    void Z_change();
    void J_change();
    void L_change();
    //简约操作代码、其他代码
    QPair<int,int> find_first_true();
    void together();
    void mode_change(int x,int y);
    void game_over();
    void paint_block();
    void paint_next_block();
    void paint_block_shadow();
    void paint_background();
    void initialization();
    void new_game();
protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
};
#endif // TETRIS_H
