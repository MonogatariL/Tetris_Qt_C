#ifndef THEME_H
#define THEME_H
#include<QPixmap>
void change_theme(QVector<QPixmap>&theme,int x)
{
    switch (x)
    {
    case 0:
    {
        theme[0].load(":/new/prefix1/res/blue.png");
        theme[1].load(":/new/prefix1/res/dark_blue.png");
        theme[2].load(":/new/prefix1/res/green.png");
        theme[3].load(":/new/prefix1/res/orange.png");
        theme[4].load(":/new/prefix1/res/purple.png");
        theme[5].load(":/new/prefix1/res/red.png");
        theme[6].load(":/new/prefix1/res/yellow.png");
        //theme[7].load(":/new/prefix1/res/background.png");
        theme[7].load("");
        theme[8].load("");
    }break;
    case 1:
    {
        theme[0].load(":/new/prefix2/monogatari/Hachikyuji_Mayoi.png");
        theme[1].load(":/new/prefix2/monogatari/Kissshot_Acerolaorion_Heartunder Blade.png");
        theme[2].load(":/new/prefix2/monogatari/Sengoku_Nadeko.png");
        theme[3].load(":/new/prefix2/monogatari/Araragi_Tsukihi.png");
        theme[4].load(":/new/prefix2/monogatari/Araragi_Karen.png");
        theme[5].load(":/new/prefix2/monogatari/Senjougahara_Hitagi.png");
        theme[6].load(":/new/prefix2/monogatari/Hanekawa_Tsubasa.png");
        theme[7].load(":/new/prefix2/monogatari/Mogatari.png");
        theme[8].load(":/new/prefix2/monogatari/Araragi_Koyomi.png");
    }
    default:return;
    }
}
#endif // THEME_H
