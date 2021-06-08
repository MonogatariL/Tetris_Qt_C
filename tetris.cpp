#include "tetris.h"
#include "ui_tetris.h"
#include<QPaintEvent>
#include<QTimer>
#include<QPainter>
#include<QPen>
#include<QDebug>
#include"config.h"
#include<QVector>
#include<QTime>
#include<QPixmap>
#include<QMessageBox>
#include<QAction>
#include"Theme.h"
Tetris::Tetris(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Tetris)
{
    ui->setupUi(this);
    setFixedSize(900,900);
    //resize(600,900);
    timerId=startTimer(speed);
    start=(width()-block_len*21)/2;
    initialization();
    change_theme(theme,0);

    connect(ui->New_Game,&QAction::triggered,this,[=](){killTimer(timerId);update();new_game();});
    connect(ui->Classic,&QAction::triggered,this,[=](){change_theme(theme,0);});
    connect(ui->Monogatari,&QAction::triggered,this,[=](){change_theme(theme,1);});
    connect(ui->Shadow_Block,&QAction::triggered,this,[=](){block_shadow_open=false;});

}
Tetris::~Tetris()
{
    delete ui;
}



void Tetris::new_block()
{
    clean();
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    if(status_next.first>=7)
    {
        status_next.first = qrand() % 7;    //产生7以内的随机数
        status_next.second= qrand() % 4;
    }
    status.first = status_next.first;
    status.second= status_next.second;
    status_next.first = qrand() % 7;    //产生7以内的随机数
    status_next.second= qrand() % 4;
    Any=true;
    get_new(block,status);
    update();
}
void Tetris::move_left()
{
    QVector<QPair<int,int>>res;
    for(int j=0;j<block[0].size();j++)
    {
        for(int i=0;i<block.size();i++)
        {
            if(block[i][j].second)
            {
                if(j-1>=0&&((block[i][j-1].first==false)||(block[i][j-1].second==true)))
                    res.push_back(QPair<int,int>(i,j));
                else
                    return;
            }
        }
    }
    for(int i=0;i<res.size();i++)
    {
        int x=res[i].first;
        int y=res[i].second;
        block[x][y].first=false;block[x][y].second=false;
        block[x][y-1].first=true;block[x][y-1].second=true;
    }
    update();
}
void Tetris::move_right()
{
    QVector<QPair<int,int>>res;
    for(int j=block[0].size()-1;j>=0;j--)
    {
        for(int i=0;i<block.size();i++)
        {
            if(block[i][j].second)
            {
                if(j+1<block[i].size()&&((block[i][j+1].first==false)||(block[i][j+1].second==true)))
                    res.push_back(QPair<int,int>(i,j));
                else
                    return;
            }
        }
    }
    for(int i=0;i<res.size();i++)
    {
        int x=res[i].first;
        int y=res[i].second;
        block[x][y].first=false;block[x][y].second=false;
        block[x][y+1].first=true;block[x][y+1].second=true;
    }
     update();
}
void Tetris::move_down()
{
    QVector<QPair<int,int>>res;
    for(int i=block.size()-1;i>=0;i--)
    {
        for(int j=block[i].size()-1;j>=0;j--)
        {
            if(block[i][j].second)
            {
                if(i+1<block.size()&&((block[i+1][j].first==false)||(block[i+1][j].second==true)))
                {
                    res.push_back(QPair<int,int>(i,j));
                }
                else
                {
                    block[i][j].second=false;
                    block_pic[i][j].first=status.first;
                }
            }
        }
    }
    if(res.size()<4)
    {
        for(int i=0;i<res.size();i++)
        {
            int x=res[i].first;
            int y=res[i].second;
            block[x][y].second=false;
            block_pic[x][y].first=status.first;
        }
        Any=false;
        return;
    }
    else
    {
        for(int i=0;i<res.size();i++)
        {
            int x=res[i].first;
            int y=res[i].second;
            block[x][y].first=false;block[x][y].second=false;
            block[x+1][y].first=true;block[x+1][y].second=true;
        }

    }
    update();
}
void Tetris::clean()
{
    for(int i=block.size()-1;i>=0;i--)
    {
        int sum=0;
        for(int j=0;j<block[i].size();j++)
        {
            if(block[i][j].first)
                sum++;
            else
                break;
        }
        if(sum==10)
        {
            score+=100;
            for(int x=i-1;x>=0;x--)
            {
                for(int y=0;y<block[x].size();y++)
                {
                    if(block[x][y].first)
                    {
                        block[x][y].first=false;
                        block[x+1][y].first=true;
                    }
                    else
                    {
                        block[x+1][y].first=false;
                    }
                    block_pic[x+1][y]=block_pic[x][y];
                }
            }
            i++;
        }
    }

    update();
}
void Tetris::get_new(QVector<QVector<QPair<bool, bool> > > &block,QPair<int,int>&status)
{
    int x=status.first;
    int y=status.second;
    switch (x)
    {
    case 0://O
    {
        if(block[0][5].first==false){block[0][5].first=true;block[0][5].second=true;}else{game_over();return;}
        if(block[0][4].first==false){block[0][4].first=true;block[0][4].second=true;}else{game_over();return;}
        if(block[1][5].first==false){block[1][5].first=true;block[1][5].second=true;}else{game_over();return;}
        if(block[1][4].first==false){block[1][4].first=true;block[1][4].second=true;}else{game_over();return;}
    }break;
    case 1://I
    {
        switch (y%2)
        {
        case 0:
        {
            if(block[1][5].first==false){block[1][5].first=true;block[1][5].second=true;}else{game_over();return;}
            if(block[1][4].first==false){block[1][4].first=true;block[1][4].second=true;}else{game_over();return;}
            if(block[1][3].first==false){block[1][3].first=true;block[1][3].second=true;}else{game_over();return;}
            if(block[1][6].first==false){block[1][6].first=true;block[1][6].second=true;}else{game_over();return;}
        }break;
        case 1:
        {
            if(block[0][5].first==false){block[0][5].first=true;block[0][5].second=true;}else{game_over();return;}
            if(block[1][5].first==false){block[1][5].first=true;block[1][5].second=true;}else{game_over();return;}
            if(block[2][5].first==false){block[2][5].first=true;block[2][5].second=true;}else{game_over();return;}
            if(block[3][5].first==false){block[3][5].first=true;block[3][5].second=true;}else{game_over();return;}
        }break;
        }
    }break;
    case 2://W
    {
        switch (y)
        {
        case 0:
        {
            if(block[0][5].first==false){block[0][5].first=true;block[0][5].second=true;}else{game_over();return;}
            if(block[1][5].first==false){block[1][5].first=true;block[1][5].second=true;}else{game_over();return;}
            if(block[1][6].first==false){block[1][6].first=true;block[1][6].second=true;}else{game_over();return;}
            if(block[1][4].first==false){block[1][4].first=true;block[1][4].second=true;}else{game_over();return;}
        }break;
        case 1:
        {
            if(block[0][4].first==false){block[0][4].first=true;block[0][4].second=true;}else{game_over();return;}
            if(block[1][4].first==false){block[1][4].first=true;block[1][4].second=true;}else{game_over();return;}
            if(block[2][4].first==false){block[2][4].first=true;block[2][4].second=true;}else{game_over();return;}
            if(block[1][5].first==false){block[1][5].first=true;block[1][5].second=true;}else{game_over();return;}
        }break;
        case 2:
        {
            if(block[0][4].first==false){block[0][4].first=true;block[0][4].second=true;}else{game_over();return;}
            if(block[0][5].first==false){block[0][5].first=true;block[0][5].second=true;}else{game_over();return;}
            if(block[0][3].first==false){block[0][3].first=true;block[0][3].second=true;}else{game_over();return;}
            if(block[1][4].first==false){block[1][4].first=true;block[1][4].second=true;}else{game_over();return;}
        }break;
        case 3:
        {
            if(block[0][5].first==false){block[0][5].first=true;block[0][5].second=true;}else{game_over();return;}
            if(block[1][5].first==false){block[1][5].first=true;block[1][5].second=true;}else{game_over();return;}
            if(block[2][5].first==false){block[2][5].first=true;block[2][5].second=true;}else{game_over();return;}
            if(block[1][4].first==false){block[1][4].first=true;block[1][4].second=true;}else{game_over();return;}
        }break;
        }
    }break;
    case 3://S
    {
        switch (y%2)
        {
        case 0:
        {
            if(block[0][5].first==false){block[0][5].first=true;block[0][5].second=true;}else{game_over();return;}
            if(block[0][4].first==false){block[0][4].first=true;block[0][4].second=true;}else{game_over();return;}
            if(block[1][4].first==false){block[1][4].first=true;block[1][4].second=true;}else{game_over();return;}
            if(block[1][3].first==false){block[1][3].first=true;block[1][3].second=true;}else{game_over();return;}
        }break;
        case 1:
        {
            if(block[1][4].first==false){block[1][4].first=true;block[1][4].second=true;}else{game_over();return;}
            if(block[0][4].first==false){block[0][4].first=true;block[0][4].second=true;}else{game_over();return;}
            if(block[1][5].first==false){block[1][5].first=true;block[1][5].second=true;}else{game_over();return;}
            if(block[2][5].first==false){block[2][5].first=true;block[2][5].second=true;}else{game_over();return;}
        }break;
        default:break;
        }
    }break;
    case 4://Z
    {
        switch (y%2)
        {
        case 0:
        {
            if(block[0][3].first==false){block[0][3].first=true;block[0][3].second=true;}else{game_over();return;}
            if(block[0][4].first==false){block[0][4].first=true;block[0][4].second=true;}else{game_over();return;}
            if(block[1][4].first==false){block[1][4].first=true;block[1][4].second=true;}else{game_over();return;}
            if(block[1][5].first==false){block[1][5].first=true;block[1][5].second=true;}else{game_over();return;}
        }break;
        case 1:
        {
            if(block[1][4].first==false){block[1][4].first=true;block[1][4].second=true;}else{game_over();return;}
            if(block[0][5].first==false){block[0][5].first=true;block[0][5].second=true;}else{game_over();return;}
            if(block[1][5].first==false){block[1][5].first=true;block[1][5].second=true;}else{game_over();return;}
            if(block[2][4].first==false){block[2][4].first=true;block[2][4].second=true;}else{game_over();return;}
        }break;
        default:break;
        }
    }break;
    case 5://J
    {
        switch (y)
        {
        case 0:
        {
            if(block[0][5].first==false){block[0][5].first=true;block[0][5].second=true;}else{game_over();return;}
            if(block[1][4].first==false){block[1][4].first=true;block[1][4].second=true;}else{game_over();return;}
            if(block[2][4].first==false){block[2][4].first=true;block[2][4].second=true;}else{game_over();return;}
            if(block[0][4].first==false){block[0][4].first=true;block[0][4].second=true;}else{game_over();return;}
        }break;
        case 1:
        {
            if(block[0][4].first==false){block[0][4].first=true;block[0][4].second=true;}else{game_over();return;}
            if(block[0][3].first==false){block[0][3].first=true;block[0][3].second=true;}else{game_over();return;}
            if(block[0][5].first==false){block[0][5].first=true;block[0][5].second=true;}else{game_over();return;}
            if(block[1][5].first==false){block[1][5].first=true;block[1][5].second=true;}else{game_over();return;}
        }break;
        case 2:
        {
            if(block[0][5].first==false){block[0][5].first=true;block[0][5].second=true;}else{game_over();return;}
            if(block[1][5].first==false){block[1][5].first=true;block[1][5].second=true;}else{game_over();return;}
            if(block[2][5].first==false){block[2][5].first=true;block[2][5].second=true;}else{game_over();return;}
            if(block[2][4].first==false){block[2][4].first=true;block[2][4].second=true;}else{game_over();return;}
        }break;
        case 3:
        {
            if(block[0][3].first==false){block[0][3].first=true;block[0][3].second=true;}else{game_over();return;}
            if(block[1][3].first==false){block[1][3].first=true;block[1][3].second=true;}else{game_over();return;}
            if(block[1][4].first==false){block[1][4].first=true;block[1][4].second=true;}else{game_over();return;}
            if(block[1][5].first==false){block[1][5].first=true;block[1][5].second=true;}else{game_over();return;}
        }break;
        default:break;
        }
    }break;
    case 6://L
    {
        switch (y)
        {
        case 0:
        {
            if(block[0][5].first==false){block[0][5].first=true;block[0][5].second=true;}else{game_over();return;}
            if(block[0][4].first==false){block[0][4].first=true;block[0][4].second=true;}else{game_over();return;}
            if(block[1][5].first==false){block[1][5].first=true;block[1][5].second=true;}else{game_over();return;}
            if(block[2][5].first==false){block[2][5].first=true;block[2][5].second=true;}else{game_over();return;}
        }break;
        case 1:
        {
            if(block[0][5].first==false){block[0][5].first=true;block[0][5].second=true;}else{game_over();return;}
            if(block[1][3].first==false){block[1][3].first=true;block[1][3].second=true;}else{game_over();return;}
            if(block[1][4].first==false){block[1][4].first=true;block[1][4].second=true;}else{game_over();return;}
            if(block[1][5].first==false){block[1][5].first=true;block[1][5].second=true;}else{game_over();return;}
        }break;
        case 2:
        {
            if(block[2][5].first==false){block[2][5].first=true;block[2][5].second=true;}else{game_over();return;}
            if(block[1][4].first==false){block[1][4].first=true;block[1][4].second=true;}else{game_over();return;}
            if(block[2][4].first==false){block[2][4].first=true;block[2][4].second=true;}else{game_over();return;}
            if(block[0][4].first==false){block[0][4].first=true;block[0][4].second=true;}else{game_over();return;}
        }break;
        case 3:
        {
            if(block[0][3].first==false){block[0][3].first=true;block[0][3].second=true;}else{game_over();return;}
            if(block[0][4].first==false){block[0][4].first=true;block[0][4].second=true;}else{game_over();return;}
            if(block[0][5].first==false){block[0][5].first=true;block[0][5].second=true;}else{game_over();return;}
            if(block[1][3].first==false){block[1][3].first=true;block[1][3].second=true;}else{game_over();return;}
        }break;
        default:break;
        }
    }break;
    default:break;
    }
}
void Tetris::block_change()
{
    int x=status.first;
    if(x==0)return;
    switch (x)
    {
    case 1:I_change();break;
    case 2:W_change();break;
    case 3:S_change();break;
    case 4:Z_change();break;
    case 5:J_change();break;
    case 6:L_change();break;
    default:return;
    }
}
void Tetris::I_change()
{
    int y=status.second;
    switch (y%2)
    {
    case 0:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(block[i][j].second)
        {
            int x=i;int y=j+2;
            if(x-1>0&&x+2<block.size()&&!block[x-1][y].first&&!block[x+2][y].first&&!block[x+1][y].first)
            {
                mode_change(i,j);mode_change(i,j+1);mode_change(i,j+3);
                mode_change(x-1,y);mode_change(x+2,y);mode_change(x+1,y);
                update();
                status.second=1;
                return;
            }
            y--;
            if(x-1>0&&x+2<block.size()&&!block[x-1][y].first&&!block[x+2][y].first&&!block[x+1][y].first)
            {
                mode_change(i,j);mode_change(i,j+2);mode_change(i,j+3);
                mode_change(x-1,y);mode_change(x+2,y);mode_change(x+1,y);
                update();
                status.second=1;
                return;
            }
            return;
        }
    }break;
    case 1:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(block[i][j].second)
        {
            int x=i+1;int y=j;
            if(y-2>=0&&y+1<block[i].size()&&!block[x][y-1].first&&!block[x][y-2].first&&!block[x][y+1].first)
            {

                mode_change(i,j);mode_change(i+2,j);mode_change(i+3,j);
                mode_change(x,y+1);mode_change(x,y-1);mode_change(x,y-2);
                update();
                status.second=0;
                return;
            }
            else if(y-1>=0&&y+2<block[i].size()&&!block[x][y-1].first&&!block[x][y+1].first&&!block[x][y+2].first)
            {

                mode_change(i,j);mode_change(i+2,j);mode_change(i+3,j);
                mode_change(x,y+1);mode_change(x,y-1);mode_change(x,y+2);
                update();
                status.second=0;
                return;
            }
            else if(y-3>=0&&!block[x][y-1].first&&!block[x][y-3].first&&!block[x][y-2].first)
            {
                mode_change(i,j);mode_change(i+2,j);mode_change(i+3,j);
                mode_change(x,y-1);mode_change(x,y-3);mode_change(x,y-2);
                update();
                status.second=0;
                return;
            }
            else if(y+3<block[i].size()&&!block[x][y+1].first&&!block[x][y+3].first&&!block[x][y+2].first)
            {
                mode_change(i,j);mode_change(i+2,j);mode_change(i+3,j);
                mode_change(x,y+1);mode_change(x,y+3);mode_change(x,y+2);
                update();
                status.second=0;
                return;
            }
            x++;
            if(y-2>=0&&y+1<block[i].size()&&!block[x][y-1].first&&!block[x][y-2].first&&!block[x][y+1].first)
            {
                mode_change(i,j);mode_change(i+1,j);mode_change(i+3,j);
                mode_change(x,y+1);mode_change(x,y-1);mode_change(x,y-2);
                update();
                status.second=0;
                return;
            }
            else if(y-1>=0&&y+2<block[i].size()&&!block[x][y-1].first&&!block[x][y+1].first&&!block[x][y+2].first)
            {
                mode_change(i,j);mode_change(i+1,j);mode_change(i+3,j);
                mode_change(x,y+1);mode_change(x,y-1);mode_change(x,y+2);
                update();
                status.second=0;
                return;
            }
            else if(y-3>=0&&!block[x][y-1].first&&!block[x][y-3].first&&!block[x][y-2].first)
            {
                mode_change(i,j);mode_change(i+1,j);mode_change(i+3,j);
                mode_change(x,y-1);mode_change(x,y-3);mode_change(x,y-2);
                update();
                status.second=0;
                return;
            }
            else if(y+3<block[i].size()&&!block[x][y+1].first&&!block[x][y+3].first&&!block[x][y+2].first)
            {
                mode_change(i,j);mode_change(i+1,j);mode_change(i+3,j);
                mode_change(x,y+1);mode_change(x,y+3);mode_change(x,y+2);
                update();
                status.second=0;
                return;
            }
            else return;
        }
    }break;
    }
}
void Tetris::W_change()
{
    int y=status.second;
    switch (y)
    {
    case 0:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(i+2<block.size()&&!block[i+2][j].first)
        {
            mode_change(i+1,j-1);mode_change(i+2,j);
            update();
            status.second=1;
            return;
        }
    }break;
    case 1:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(i+1<block.size()&&j-1>=0&&!block[i+1][j-1].first)
        {
            mode_change(i,j);mode_change(i+1,j-1);
            update();
            status.second=2;
            return;
        }
        else if(!block[i][j+1].first&&!block[i+2][j+1].first)
        {
            mode_change(i,j);mode_change(i,j+1);
            mode_change(i+2,j);mode_change(i+2,j+1);
            update();
            status.second=3;
            return;
        }
    }break;
    case 2:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(i-1>=0&&j+1<block[i].size()&&!block[i-1][j+1].first)
        {
            mode_change(i,j+2);mode_change(i-1,j+1);
            update();
            status.second=3;
            return;
        }
    }break;
    case 3:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(i+1<block.size()&&j+1<block[i].size()&&!block[i+1][j+1].first)
        {
            mode_change(i+2,j);mode_change(i+1,j+1);
            update();
            status.second=0;
            return;
        }
        else if(!block[i][j-1].first&&!block[i+2][j-1].first)
        {
            mode_change(i,j);mode_change(i,j-1);
            mode_change(i+2,j);mode_change(i+2,j-1);
            update();
            status.second=1;
            return;
        }
    }break;
    default:return;
    }
}
void Tetris::S_change()
{
    int y=status.second;
    switch (y%2)
    {
    case 0:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(i-1>=0&&!block[i-1][j-1].first&&!block[i][j-1].first)
        {
            mode_change(i,j+1);mode_change(i-1,j-1);
            mode_change(i+1,j-1);mode_change(i,j-1);
            update();
            status.second=1;
            return;
        }
    }break;
    case 1:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(j+2<block[i].size()&&!block[i+1][j+2].first&&!block[i+2][j].first)
        {
            mode_change(i,j);mode_change(i+1,j+2);
            mode_change(i+1,j);mode_change(i+2,j);
            update();
            status.second=0;
            return;
        }
        else if(j-1>=0&&!block[i+2][j-1].first&&!block[i+2][j].first)
        {
            mode_change(i,j);mode_change(i+2,j-1);
            mode_change(i+2,j+1);mode_change(i+2,j);
            update();
            status.second=0;
            return;
        }
    }break;
    default:return;
    }
}
void Tetris::Z_change()
{
    int y=status.second;
    switch (y%2)
    {
    case 0:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(i-1>=0&&!block[i+1][j].first&&!block[i-1][j+1].first)
        {
            mode_change(i+1,j+1);mode_change(i+1,j+2);
            mode_change(i+1,j);mode_change(i-1,j+1);
            update();
            status.second=1;
            return;
        }
    }break;
    case 1:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(j+1<block[i].size()&&!block[i+2][j+1].first&&!block[i+2][j].first)
        {
            mode_change(i,j);mode_change(i+2,j+1);
            mode_change(i+2,j-1);mode_change(i+2,j);
            update();
            status.second=0;
            return;
        }
        else if(j-2>=0&&!block[i+1][j-2].first&&!block[i+2][j].first)
        {
            mode_change(i,j);mode_change(i+1,j);
            mode_change(i+1,j-2);mode_change(i+2,j);
            update();
            status.second=0;
            return;
        }
    }break;
    default:return;
    }
}
void Tetris::J_change()
{
    int y=status.second;
    switch (y)
    {
    case 0:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(j-1>=0&&!block[i+1][j-1].first&&!block[i+1][j+1].first&&!block[i+2][j+1].first)
        {
            mode_change(i,j);mode_change(i,j+1);mode_change(i+2,j);
            mode_change(i+1,j-1);mode_change(i+1,j+1);mode_change(i+2,j+1);
            update();
            status.second=1;
            return;
        }
        else if(j-1>=0&&!block[i+1][j+1].first&&!block[i+1][j+2].first&&!block[i+2][j+2].first)
        {
            mode_change(i,j);mode_change(i,j+1);mode_change(i+2,j);
            mode_change(i+1,j+1);mode_change(i+1,j+2);mode_change(i+2,j+2);
            update();
            status.second=1;
            return;
        }
        else if(!block[i+1][j+1].first&&!block[i+2][j+1].first)
        {
            mode_change(i,j);mode_change(i+1,j);
            mode_change(i+1,j+1);mode_change(i+2,j+1);
            update();
            status.second=2;
            return;
        }
        else if(j-1>=0&&!block[i][j-1].first&&!block[i+1][j+1].first&&!block[i+1][j-1].first)
        {
            mode_change(i,j);mode_change(i,j+1);mode_change(i+2,j);
            mode_change(i,j-1);mode_change(i+1,j-1);mode_change(i+1,j+1);
            update();
            status.second=3;
            return;
        }
    }break;
    case 1:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(i-1>=0&&!block[i-1][j+1].first&&!block[i+1][j+1].first&&!block[i+1][j].first)
        {
            mode_change(i,j);mode_change(i,j+2);mode_change(i+1,j+2);
            mode_change(i+1,j);mode_change(i+1,j+1);mode_change(i-1,j+1);
            update();
            status.second=2;
            return;
        }
        else if(!block[i+1][j].first&&!block[i+1][j+1].first)
        {
            mode_change(i,j+1);mode_change(i,j+2);
            mode_change(i+1,j);mode_change(i+1,j+1);
            update();
            status.second=3;
            return;
        }
    }break;
    case 2:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(j+1<block[i].size()&&!block[i+1][j+1].first&&!block[i+1][j+1].first&&!block[i][j-1].first)
        {
            mode_change(i,j);mode_change(i+2,j);mode_change(i+2,j-1);
            mode_change(i+1,j-1);mode_change(i,j-1);mode_change(i+1,j+1);
            update();
            status.second=3;
            return;
        }
        else if(j-2>=0&&!block[i+1][j-2].first&&!block[i+2][j-2].first)
        {
            mode_change(i,j);mode_change(i+1,j);
            mode_change(i+1,j-2);mode_change(i+2,j-2);
            update();
            status.second=3;
            return;
        }
        else if(!block[i][j-1].first&&!block[i+1][j-1].first)
        {
            mode_change(i+1,j);mode_change(i+2,j);
            mode_change(i,j-1);mode_change(i+1,j-1);
            update();
            status.second=0;
            return;
        }

    }break;
    case 3:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(i+2<=block.size()&&!block[i+2][j+1].first&&!block[i][j+1].first&&!block[i][j+2].first)
        {
            mode_change(i,j);mode_change(i+1,j);mode_change(i+1,j+2);
            mode_change(i,j+1);mode_change(i,j+2);mode_change(i+2,j+1);
            update();
            status.second=0;
            return;
        }
        else if(i-1>=0&&!block[i][j+1].first&&!block[i-1][j+1].first&&!block[i-1][j+2].first)
        {
            mode_change(i,j);mode_change(i+1,j);mode_change(i+1,j+2);
            mode_change(i,j+1);mode_change(i-1,j+1);mode_change(i-1,j+2);
            update();
            status.second=0;
            return;
        }
        else if(!block[i][j+1].first&&!block[i][j+2].first)
        {
            mode_change(i+1,j);mode_change(i+1,j+1);
            mode_change(i,j+1);mode_change(i,j+2);
            update();
            status.second=0;
            return;
        }break;
    }
    default:return;
    }
}
void Tetris::L_change()
{
    int y=status.second;
    switch (y)
    {
    case 0:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(j+2<block[i].size()&&!block[i+1][j].first&&!block[i+1][j+2].first&&!block[i][j+2].first)
        {
            mode_change(i,j);mode_change(i,j+1);mode_change(i+2,j+1);
            mode_change(i+1,j);mode_change(i+1,j+2);mode_change(i,j+2);
            update();
            status.second=1;
            return;
        }
        else if(j-1>=0&&!block[i+1][j-1].first&&!block[i+1][j].first)
        {
            mode_change(i,j);mode_change(i+2,j+1);
            mode_change(i+1,j-1);mode_change(i+1,j);
            update();
            status.second=1;
            return;
        }
        else if(!block[i+2][j].first&&!block[i+1][j].first)
        {
            mode_change(i,j+1);mode_change(i+1,j+1);
            mode_change(i+2,j);mode_change(i+1,j);
            update();
            status.second=2;
            return;
        }
        else if(j+2<block[i].size()&&!block[i+1][j].first&&!block[i+2][j].first&&!block[i+1][j+2].first)
        {
            mode_change(i,j);mode_change(i,j+1);mode_change(i+2,j+1);
            mode_change(i+1,j);mode_change(i+2,j);mode_change(i+1,j+2);
            update();
            status.second=3;
            return;
        }
    }break;
    case 1:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(i+2<block.size()&&!block[i][j-1].first&&!block[i+2][j-1].first&&!block[i+2][j].first)
        {
            mode_change(i,j);mode_change(i+1,j);mode_change(i+1,j-2);
            mode_change(i,j-1);mode_change(i+2,j-1);mode_change(i+2,j);
            update();
            status.second=2;
            return;
        }
        else if(!block[i][j-2].first&&!block[i][j-1].first)
        {
            mode_change(i+1,j);mode_change(i+1,j-1);
            mode_change(i,j-1);mode_change(i,j-2);
            update();
            status.second=3;
            return;
        }
    }break;
    case 2:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(j-1>=0&&!block[i+1][j-1].first&&!block[i+2][j-1].first&&!block[i+1][j+1].first)
        {
            mode_change(i,j);mode_change(i+2,j);mode_change(i+2,j+1);
            mode_change(i+1,j-1);mode_change(i+2,j-1);mode_change(i+1,j+1);
            update();
            status.second=3;
            return;
        }
        else if(j+2<block[i].size()&&!block[i+1][j+1].first&&!block[i+1][j+2].first)
        {
            mode_change(i,j);mode_change(i+2,j+1);
            mode_change(i+1,j+1);mode_change(i+1,j+2);
            update();
            status.second=3;
            return;
        }
        else if(!block[i][j+1].first&&!block[i+1][j+1].first)
        {
            mode_change(i+2,j);mode_change(i+1,j);
            mode_change(i,j+1);mode_change(i+1,j+1);
            update();
            status.second=0;
            return;
        }
        else if(j-1>=0&&block[i][j+1].first&&!block[i+1][j+1].first&&!block[i+1][j-1].first)
        {
            mode_change(i,j);mode_change(i+2,j);mode_change(i+2,j+1);
            mode_change(i,j+1);mode_change(i+1,j+1);mode_change(i+1,j-1);
            update();
            status.second=1;
            return;
        }

    }break;
    case 3:
    {
        QPair<int,int>first=find_first_true();
        int i=first.first;
        int j=first.second;
        if(i-1>=0&&!block[i-1][j].first&&!block[i-1][j+1].first&&!block[i+1][j+1].first)
        {
            mode_change(i,j);mode_change(i,j+2);mode_change(i+1,j);
            mode_change(i-1,j);mode_change(i-1,j+1);mode_change(i+1,j+1);
            update();
            status.second=0;
            return;
        }
        else if(!block[i+1][j+1].first&&!block[i+1][j+2].first)
        {
            mode_change(i,j);mode_change(i,j+1);
            mode_change(i+1,j+2);mode_change(i+1,j+1);
            update();
            status.second=1;
            return;
        }
    }break;
    default:return;
    }
}






QPair<int, int> Tetris::find_first_true()
{
    for(int i=0;i<block.size();i++)
    {
        for(int j=0;j<block[i].size();j++)
        {
            if(block[i][j].second)
                return QPair<int,int>(i,j);
        }
    }
    return QPair<int,int>(0,0);
}

void Tetris::together()
{
    for(int i=0;i<block.size();i++)
    {
        for(int j=0;j<block[i].size();j++)
        {
            block_pic[i][j].second=block[i][j].first;
            if(block[i][j].second)
            {
                block_pic[i][j].first=status.first;
            }
        }
    }
}
void Tetris::mode_change(int x, int y)
{
    block[x][y].first=!block[x][y].first;
    block[x][y].second=!block[x][y].second;
}
void Tetris::game_over()
{
    killTimer(timerId);
    update();
    QString str=QString("Score:%1\nGrade:%2 ").arg(score).arg(grade);
    int ret=QMessageBox::question(this,"游戏结束",str,QMessageBox::Ok,QMessageBox::No);
    switch(ret)
    {
        case QMessageBox::Ok:new_game();break;
        case QMessageBox::No:close();break;
    }
}

void Tetris::paint_block()
{
    QPainter painter(this);
    for(int i=0;i<block_pic.size();i++)
    {
        for(int j=0;j<block_pic[i].size();j++)
        {
            if(block_pic[i][j].second)
            {
                   painter.drawPixmap(start+j*block_len,(i-1)*block_len+23,block_len,block_len,theme[block_pic[i][j].first]);
            }
        }
    }
}
void Tetris::paint_next_block()
{
    for(int i=0;i<block_next.size();i++)
    {
        for(int j=0;j<block_next[i].size();j++)
        {
            block_next[i][j].first=false;
            block_next[i][j].second=false;
        }
    }
    get_new(block_next,status_next);
    QPainter painter(this);
    for(int i=0;i<block_next.size();i++)
    {
        for(int j=0;j<block_next[i].size();j++)
        {
            block_next_pic[i][j].second=block_next[i][j].first;
            if(block_next[i][j].second)
            {
                block_next_pic[i][j].first=status_next.first;
            }
        }
    }
    int key=start+block_len*7+20;
    QBrush brush;brush.setStyle(Qt::Dense3Pattern);painter.setBrush(brush);
    for(int i=0;i<block_next_pic.size();i++)
    {
        for(int j=0;j<block_next_pic[i].size();j++)
        {
            if(block_next_pic[i][j].second)
            {      if(status_next.first==1&&status_next.second%2==1)
                   painter.drawPixmap(key+j*block_len-25,(i+1)*block_len+23-block_len,block_len,block_len,theme[block_next_pic[i][j].first]);
                   else
                   painter.drawPixmap(key+j*block_len,(i+1)*block_len+23,block_len,block_len,theme[block_next_pic[i][j].first]);
            }
        }
    }

}
void Tetris::paint_block_shadow()
{
    QPainter painter(this);
    QVector<QVector<QPair<bool,bool>>>block_shadow=block;
    QVector<QVector<QPair<int,bool>>>block_pic_shadow=block_pic;
    while(1)
    {
            QVector<QPair<int,int>>res;
        for(int i=block_shadow.size()-1;i>=0;i--)
        {
            for(int j=block_shadow[i].size()-1;j>=0;j--)
            {
                if(block_shadow[i][j].second)
                {
                    if(i+1<block_shadow.size()&&((block_shadow[i+1][j].first==false)||(block_shadow[i+1][j].second==true)))
                    {
                        res.push_back(QPair<int,int>(i,j));
                    }
                    else
                    {
                        block_shadow[i][j].second=false;
                        block_pic_shadow[i][j].first=status.first;
                    }
                }
            }
        }
        if(res.size()<4)
        {
            for(int i=0;i<res.size();i++)
            {
                int x=res[i].first;
                int y=res[i].second;
                block_shadow[x][y].second=false;
                block_pic_shadow[x][y].first=status.first;
            }
            break;
        }
        else
        {
            for(int i=0;i<res.size();i++)
            {
                int x=res[i].first;
                int y=res[i].second;
                block_shadow[x][y].first=false;block_shadow[x][y].second=false;
                block_shadow[x+1][y].first=true;block_shadow[x+1][y].second=true;
            }

        }
    }
    painter.setOpacity(0.3);
    for(int i=0;i<block.size();i++)
    {
        for(int j=0;j<block[i].size();j++)
        {
            if(block_shadow[i][j].first)
            {
                painter.drawPixmap(start+j*block_len,(i-1)*block_len+23,block_len,block_len,theme[block_pic_shadow[i][j].first]);
            }
        }
    }
    update();

}
void Tetris::paint_background()
{
    QPainter painter(this);
    QPen pen;pen.setWidth(3);painter.setPen(pen);
    //外边框中线
    painter.drawLine(start,block_len*20+10+23,start+block_len*10,block_len*20+10+23);
    //外边框左线
    painter.drawLine(start-10,0,start-10,block_len*20+23);
    //外边框右线
    painter.drawLine(start+block_len*10+10,0,start+block_len*10+10,block_len*20+23);
    pen.setWidth(1);painter.setPen(pen);
    int x=start-block_len;
    int y=-block_len;
    for(int i=0;i<=20;i++)
    {
        x+=block_len;
        y+=block_len;
        if(i<=10)
        painter.drawLine(x,0,x,block_len*20+23);
        painter.drawLine(start,y+23,start+block_len*10,y+23);
    }
    int key=start+block_len*10+20;
//    int ex_x=key-block_len;
//    int ex_y=-block_len;
//    for(int i=0;i<5;i++)
//    {
//        ex_x+=block_len;
//        ex_y+=block_len;
//        painter.drawLine(ex_x,0,ex_x,block_len*4+23);
//        painter.drawLine(key,ex_y+23,key+block_len*4,ex_y+23);
//    }
    painter.drawPixmap(key,0,4*block_len,4*block_len,theme[8]);
    ui->label->setText(QString("SCORE:%1").arg(score));
    ui->label_2->setText(QString("GRADE:%2").arg(grade));
}

void Tetris::initialization()
{
    for(int i=0;i<21;i++)
    {
        block.push_back(QVector<QPair<bool,bool>>());
        for(int j=0;j<10;j++)
            block[i].push_back(QPair<bool,bool>(false,false));
    }
    for(int i=0;i<21;i++)
    {
        block_pic.push_back(QVector<QPair<int,bool>>());
        for(int j=0;j<10;j++)
        {
            block_pic[i].push_back(QPair<int,bool>(0,false));
        }
    }
    for(int i=0;i<5;i++)
    {
        block_next.push_back(QVector<QPair<bool,bool>>());
        for(int j=0;j<10;j++)
            block_next[i].push_back(QPair<bool,bool>(false,false));
    }
    for(int i=0;i<5;i++)
    {
        block_next_pic.push_back(QVector<QPair<int,bool>>());
        for(int j=0;j<10;j++)
        {
            block_next_pic[i].push_back(QPair<int,bool>(0,false));
        }
    }
    theme.resize(9);
    new_block();
}
void Tetris::new_game()
{
    for(int i=0;i<block.size();i++)
    {
        for(int j=0;j<block[i].size();j++)
        {
            block[i][j].first=false;
            block[i][j].second=false;
            block_pic[i][j].second=false;
        }
    }
    for(int i=0;i<block_next.size();i++)
    {
        for(int j=0;j<block_next.size();j++)
        {
            block_next[i][j].first=false;
            block_next[i][j].second=false;
            block_next_pic[i][j].second=false;
        }
    }
    score=0;
    speed=1000;
    grade=1;
    timerId=startTimer(speed);
    update();
    //new_block();
    return;
}
void Tetris::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<event->x()<<" "<<event->y();

}
void Tetris::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QBrush brush;brush.setStyle(Qt::Dense3Pattern);painter.setBrush(brush);
    painter.drawPixmap(0,0,width(),height(),theme[7]);
    together();
    paint_background();
    paint_block();
    paint_next_block();
    if(block_shadow_open)
    paint_block_shadow();
}
void Tetris::keyPressEvent(QKeyEvent *event)
{
    if(Any==false)return;
    qDebug()<<event->key();
    switch (event->key())
    {
    case 16777234:move_left();break;
    case 16777236:move_right();break;
    case 16777237:move_down();break;
    case 90:block_change();break;
    case 88:while(Any)move_down();new_block();break;
    case 16777216:
    {
        //
        //ui->label_3->hide();
    }break;
    case 16777264:change_theme(theme,0);break;
    case 16777265:change_theme(theme,1);break;
    case 16777217:block_shadow_open=!block_shadow_open;break;
    default:return;
    }

}
void Tetris::timerEvent(QTimerEvent *event)
{
    if(score>=grade*score_up
             )
    {
        killTimer(timerId);
        speed-=speed/10;
        grade++;
        timerId=startTimer(speed);
    }
    if(Any==false)new_block();
    move_down();
}

