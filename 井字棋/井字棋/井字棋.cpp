
#include <iostream>
#include <Windows.h>
#include <graphics.h>
#include <cstdlib> // 包含了 srand 和 rand 函数所需的头文件
#include <ctime>   // 包含了 time 函数所需的头文件
#include <string>
#include <conio.h>
#include <tchar.h>
using namespace std;

#define BOARD_SIZE 3
#define EMPTY ' '
#define PLAYER 'O'
#define OPPONENT 'X'

char bf[8] = "000:000";

int bifen[2] = {0,0};   // 比分

double win = 0;


//进位
void jinwei(char* c,int x)
{
    if (c[x] == ':')
    {
        c[x - 1] ++;
        c[x] = '0';
        if (c[x - 1] == ':')
        {
            c[x - 2]++;
            c[x-1] = '0';
        }
    }
}
//画叉
void fork1(int h, int l,int width=106)
{
    setlinestyle(PS_ENDCAP_ROUND, 10);
    line(h+20,l-20,1.4*width+h-20,l-1.4*width+20);
    line(h+20,l-1.4*width+20,h+1.4*width-20,l-20);
}

//画圆
void circle1(int h,int l,int r,int width=10)
{
    setlinestyle(PS_ENDCAP_ROUND,width);
    circle(h+75,l-75,r-20);
}

//函数：检查游戏是否胜出
char checkWin(char board[BOARD_SIZE][BOARD_SIZE])
{
    //检查横向
    if (board[0][0] == board[0][1] && board[0][1] == board[0][2] && board[0][2] != ' ')
        return board[0][0];
    if (board[1][0] == board[1][1] && board[1][1] == board[1][2] && board[1][2] != ' ')
        return board[1][0];
    if (board[2][0] == board[2][1] && board[2][1] == board[2][2] && board[2][2] != ' ')
        return board[2][0];
    //检查纵向
    if (board[0][0] == board[1][0] && board[1][0] == board[2][0] && board[2][0] != ' ')
        return board[0][0];
    if (board[0][1] == board[1][1] && board[1][1] == board[2][1] && board[2][1] != ' ')
        return board[0][1];
    if (board[0][2] == board[1][2] && board[1][2] == board[2][2] && board[2][2] != ' ')
        return board[0][2];
    //检查对角线
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[2][2] != ' ')
        return board[1][1];
    if (board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[0][2] != ' ')
        return board[1][1];
    //检查游戏是否平局
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == ' ')
                return ' ';
        }
    }
    //游戏平局
    return 'N';
}
//检查是否将要失败
int istofail(char board[3][3], int row, int col, char x) {
    int count = 0;

    // Check row
    int rowCount = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[row][i] == x) {
            /*printf("%c",x);*/
            rowCount++;
        }
    }
    if (rowCount == 2) {
        /*printf("%d行\n", row);*/
        count++;
    }
    //else
    //{
    //    count = 0;
    //}

    // Check column
    int colCount = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][col] == x) {
            colCount++;
        }
    }
    if (colCount == 2) {
        /*printf("%d列\n",col);*/
        count++;
    }

    // Check diagonal
    if (row == col) {
        int diagCount = 0;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i][i] == x) {
                diagCount++;
            }
        }
        if (diagCount == 2) {
            /*printf("斜杠\n");*/
            count++;
        }
    }

    // Check anti-diagonal
    if (row + col == BOARD_SIZE - 1) {
        int antiDiagCount = 0;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i][BOARD_SIZE - 1 - i] == x) {
                antiDiagCount++;
            }
        }
        if (antiDiagCount == 2) {
            /*printf("反斜杠\n");*/
            count++;
        }
    }

    return count;
}


//判断是否是平局
bool  isTieScenarioForAnyPlayer(char board[BOARD_SIZE][BOARD_SIZE], char z)
{
    if (checkWin(board) == PLAYER || checkWin(board) == OPPONENT)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[i][j] == ' ')
                {
                    board[i][j] = z;
                    if (z == PLAYER)
                    {
                        if (isTieScenarioForAnyPlayer(board, OPPONENT) == false)
                        {
                            board[i][j] = ' ';
                            return false;
                        }
                    }
                    else
                    {
                        if (isTieScenarioForAnyPlayer(board, PLAYER) == false)
                        {
                            board[i][j] = ' ';
                            return false;
                        }
                    }
                    board[i][j] = ' ';
                }
            }
        }
    }
    return true;
}
//计算胜率
void winning_rate(char board[BOARD_SIZE][BOARD_SIZE], char z, int bushu)
{
    if (checkWin(board) == PLAYER)
    {
        win += bushu + 1;
    }
    else if (checkWin(board) == 'N')
    {
        win += (bushu + 1) / 2;
    }
    else if (checkWin(board) == OPPONENT)
    {
        win -= (bushu+1);
    }
    else if (bushu <= 0)
    {
        return;
    }
    else
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[i][j] == ' ')
                {
                    board[i][j] = z;
                    if (z == PLAYER)
                    {
                        winning_rate(board, OPPONENT, bushu-1 );
                    }
                    else
                    {
                        winning_rate(board, PLAYER, bushu -1);
                    }
                    board[i][j] = ' ';
                }
            }
        }
    }
}
//计算下一步最优解
void get_next_move1(char board[BOARD_SIZE][BOARD_SIZE], int number,int wz[2])
{
    double max = -100;
    //double min = 0;
    //int w[2] = {-1,-1};

    if (number > 3)
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[i][j] == ' ')
                {
                    board[i][j] = PLAYER;
                    win = 0;
                    //loser = 0;
                    //tie = 0;
                    if (number < 3)
                    {
                        winning_rate(board, OPPONENT, 6);
                    }
                    else
                    {
                        winning_rate(board, OPPONENT, 4);
                    }

                    int w1= istofail(board, i, j, OPPONENT)*4;   
                    w1 += istofail(board, i, j, PLAYER);
                    /*printf("%d,%d,%lf,%d,%lf\n", i, j, win,w1,w1+win);*/
                    //w[1] = -1;
                    //w[0] = -1;

                    double winrate = win+w1 ;
                    if (max < winrate)
                    {
                       // min = tierate;
                        max = winrate;
                        wz[0] = i;
                        wz[1] = j;
                    }
                    board[i][j] = ' ';
                }
            }
        }
    }

    if (wz[0]!=-1)
    {
        board[wz[0]][wz[1]] = PLAYER;
    }
    else {

        srand(static_cast<unsigned int>(time(nullptr)));
        //没有就随机下一个位置
        while (true) {
            int row = rand() % BOARD_SIZE;
            int col = rand() % BOARD_SIZE;
            if (board[row][col] == ' ') {
                int x = row;
                int y = col;
                wz[0] = x;
                wz[1] = y;
                board[x][y] = PLAYER;
                break;
            }
        }
    }
    //printf("\n");
}

//比分初始化
void bifenchongzhi(int bifen[2],char* bf)//比分重置
{
    memset(bifen, 0, sizeof(bifen));
    for (int i = 0; i < 7; i++)
    {
        if(i!=3)
            bf[i] = '0';
    }
    //printf("比分已重置\n");
}

//棋盘界面
int boardgui(char board[3][3],bool people)
{
    int player = MessageBox( nullptr, _T("我要先手"), _T("选择先后手"), MB_YESNO);
    if (player == IDYES)
    {
        player = 2;
    }
    else
    {
        player = 1;
    }

    initgraph(450, 660);
    
    // 获取窗口句柄
    HWND hwnd = GetHWnd();

    // 获取屏幕尺寸
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 计算窗口左上角坐标使其位于屏幕中央
    int windowWidth = 450;  // 窗口宽度
    int windowHeight = 660; // 窗口高度
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

    // 重新调整窗口位置和大小，以适应新的样式
    SetWindowPos(hwnd, NULL, x, y, windowWidth+16, windowHeight+39, SWP_FRAMECHANGED);

    HICON hIcon = (HICON)LoadImage(NULL, L"图标.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    if (hIcon != NULL) {
        // 获取系统最大图标大小
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);

        // 调整图标大小
        hIcon = (HICON)LoadImage(NULL, L"图标.ico", IMAGE_ICON, cxIcon, cyIcon, LR_LOADFROMFILE);

        // 设置窗口图标
        SendMessage(GetHWnd(), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(GetHWnd(), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    }

    
    ExMessage m;		// 定义消息变量

    //背景无边框矩形
    setfillcolor(RGB(200, 220, 240));
    solidrectangle(0, 0, 450, 210);
    //背景无填充矩形
    setlinestyle(PS_SOLID, 3);
    setlinecolor(RGB(230, 240, 250));
    rectangle(0, 0, 450, 210);

    //标题
    settextcolor(BLACK);
    setbkcolor(RGB(200, 220, 240));
    settextstyle(30, 0, _T("宋体"));
    outtextxy(150, 20, _T("井字棋游戏"));
    //比分
    setlinestyle(PS_SOLID, 3);
    setlinecolor(RGB(50, 55, 60));
    rectangle(20, 110, 430 + 1, 170 + 1);
    setfillcolor(WHITE);
    solidrectangle(20, 110, 430, 170);

    setbkcolor(WHITE);
    settextstyle(25, 0, _T("宋体"));
    if (people)
    {
        outtextxy(100, 115, _T("玩家O"));
        outtextxy(290, 115, _T("玩家X"));
    }
    else
    {
        outtextxy(100, 115, _T("机器"));
        outtextxy(290, 115, _T("玩家"));
    }


    //菜单按钮
    settextstyle(20, 0, _T("宋体"));
    setbkcolor(RGB(150, 160, 170));
    setlinestyle(PS_SOLID, 3);
    setlinecolor(RGB(50, 55, 60));
    rectangle(20, 180, 215 + 2, 200 + 2);
    setfillcolor(RGB(150, 160, 170));
    solidrectangle(20, 180, 215, 200);
    outtextxy(90, 180, _T("菜单(M)"));

    //菜单按钮
    setlinestyle(PS_SOLID, 3);
    setlinecolor(RGB(50, 55, 60));
    rectangle(225, 180, 430 + 2, 200 + 2);
    setfillcolor(RGB(150, 160, 170));
    solidrectangle(225, 180, 430, 200);
    outtextxy(300, 180, _T("退出(O)"));

    while (true)
    {

        for (int i = 0; i < 3; i++) 
        {
            for (int j = 0; j < 3; j++)
            {
                board[i][j]=' ';
            }
        }
        int player1 = player;
        int wz[2] = { -1,-1 };
        int number = 1;

        int x = -1;
        int y = -1;
        //棋盘清空
        setbkcolor(WHITE);
        clearrectangle(0, 210, 450, 660);

        //比分
        settextstyle(25, 0, _T("宋体"));
        setbkcolor(WHITE);
        for (int i = 0; i < 7; i++)
        {
            outtextxy(170 + 15 * i, 145, bf[i]);
        }
        //棋盘线
        setlinestyle(PS_SOLID| PS_ENDCAP_ROUND,10);
        setlinecolor(RGB(150,160,170));
        line(10,360,440,360);
        line(10,510,440,510);
        line(150,220,150,650);
        line(300,220,300,650);

        while (true)
        {
            if (player1 == 2)
            {
                //人人对弈
                setlinestyle(PS_SOLID| PS_ENDCAP_FLAT | PS_JOIN_BEVEL,3);
                setlinecolor(RGB(50,55,60));
                line(250,140+2,400,140+2);
                while (true)
                {
                    m = getmessage(EX_MOUSE|EX_CHAR);
                    x = -1;
                    y = -1;
                    switch (m.message)
                    {
                        case WM_LBUTTONUP:
                            if (m.y > 210 && m.y <= 660 && m.x >= 0 && m.y <= 660)
                            {
                                if (m.y < 355)
                                {
                                    x = 0;
                                }
                                else if (m.y < 500)
                                {
                                    x = 1;
                                }
                                else
                                {
                                    x = 2;
                                }
                                y = m.x / 145;
                            }
                            //菜单
                            if (m.x >= 20 && m.x <= 215 && m.y >= 180 && m.y <= 200)
                            {
                                closegraph();
                                memset(board, EMPTY, sizeof(board));
                                return 1;
                            }
                            //退出
                            if (m.x >= 225 && m.x <= 430 && m.y >= 180 && m.y <= 200)
                            {
                                closegraph();
                                bifenchongzhi(bifen, bf);
                                return 0;
                            }
                            break;
                        case WM_CHAR:  // 键盘字符输入事件
                            if (toupper(m.ch) == 'M')  // 不区分大小写比较
                            {
                                // 关闭图形窗口
                                closegraph();
                                memset(board, EMPTY, sizeof(board));
                                return 1;
                            }
                            if (toupper(m.ch) == 'O')  // 不区分大小写比较
                            {
                                closegraph();
                                bifenchongzhi(bifen, bf);
                                return 0;
                            }
                            break;
                    }
                    // 判断位置是否已被占用
                    if (x > -1 && y > -1)
                    {
                        if (board[x][y] == ' ')
                        {
                            board[x][y] = 'X';
                            setlinecolor(RGB(150, 160, 170));
                            fork1(y * 150 , x * 150+ 360);
                            ++number;
                            Sleep(50);
                            break;
                        }
                    }
                }
                setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_BEVEL, 3);
                setlinecolor(RGB(255, 255, 255));
                line(250, 140 + 2, 400, 140 + 2);

                // 判断是否胜出
                char winner = checkWin(board);
                if (winner == 'X')
                {
                    ++bifen[player1 - 1];
                    // 判断分数是否超出表示范围
                    if (bifen[player1 - 1] == 1000)
                    {
                        //Sleep(50);
                        //people == true ? MessageBox(GetHWnd(), _T("玩家X满分"), _T("游戏结束"), MB_OK) : MessageBox(GetHWnd(), _T("玩家满分"), _T("游戏结束"), MB_OK);
                        //memset(board, EMPTY, sizeof(board));
                        break;
                    }
                    bf[6]++;
                    jinwei(bf, 6);
                    Sleep(50);
                    memset(board, EMPTY, sizeof(board));
                    people == true ? MessageBox(GetHWnd(), _T("玩家X胜利"), _T("游戏结束"), MB_OK) : MessageBox(GetHWnd(), _T("玩家胜利"), _T("游戏结束"), MB_OK);
                    break;
                }
                // 判断是否平局
                if (winner == 'N' ||  isTieScenarioForAnyPlayer(board, PLAYER))
                {
                    Sleep(50);
                    MessageBox(GetHWnd(), _T("平局"), _T("游戏结束"), MB_OK);
                    memset(board, EMPTY, sizeof(board));
                    break;
                }
            }
            else
            {
                //人机对弈
                setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_BEVEL, 3);
                setlinecolor(RGB(50, 55, 60));
                line(50, 140 + 2, 200, 140 + 2);
                memset(wz, -1, sizeof(wz));
                if (!people)
                {
                    // 电脑下棋
                    get_next_move1(board, number, wz);
                    Sleep(50);
                    setlinecolor(RGB(150, 160, 170));
                    circle1(wz[1] * 150, wz[0] * 150 + 360, 72);
                    ++number;
                }
                else
                {
                    while (true)
                    {
                        m = getmessage(EX_MOUSE|EX_CHAR);
                        x = -1;
                        y = -1;
                        switch (m.message)
                        {
                            case WM_LBUTTONUP:
                                if (m.y > 210 && m.y <= 660 && m.x >= 0 && m.y <= 660)
                                {
                                    if (m.y < 355)
                                    {
                                        x = 0;
                                    }
                                    else if (m.y < 500)
                                    {
                                        x = 1;
                                    }
                                    else
                                    {
                                        x = 2;
                                    }
                                    y = m.x / 145;
                                }
                                //菜单
                                if (m.x >= 20 && m.x <= 215 && m.y >= 180 && m.y <= 200)
                                {
                                    closegraph();
                                    memset(board, EMPTY, sizeof(board));
                                    return 1;
                                }
                                //退出
                                if (m.x >= 225 && m.x <= 430 && m.y >= 180 && m.y <= 200)
                                {
                                    closegraph();
                                    bifenchongzhi(bifen, bf);
                                    return 0;
                                }
                                break;
                            case WM_CHAR:  // 键盘字符输入事件
                                if (toupper(m.ch) == 'M')  // 不区分大小写比较
                                {
                                    // 关闭图形窗口
                                    closegraph();
                                    memset(board, EMPTY, sizeof(board));
                                    return 1;
                                }
                                if (toupper(m.ch) == 'O')  // 不区分大小写比较
                                {
                                    closegraph();
                                    bifenchongzhi(bifen, bf);
                                    return 0;
                                }
                                break;
                        }
                        // 判断位置是否已被占用
                        if (board[x][y] == ' ' && x > -1 && y > -1)
                        {
                            board[x][y] = 'O';
                            setlinecolor(RGB(150, 160, 170));
                            circle1(y * 150, x * 150 + 360 , 72);
                            Sleep(50);
                            break;
                        }
                    }
                }
                setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_BEVEL, 3);
                setlinecolor(RGB(255, 255, 255));
                line(50, 140 + 2, 200, 140 + 2);

                // 判断是否胜出
                char winner = checkWin(board);
                if (winner == 'O')
                {
                    ++bifen[player1 - 1];
                    // 判断分数是否超出表示范围
                    if (bifen[player1-1]==1000)
                    {
                        //Sleep(50);
                        //people == true ? MessageBox(GetHWnd(), _T("玩家O满分"), _T("游戏结束"), MB_OK) : MessageBox(GetHWnd(), _T("机器满分"), _T("游戏结束"), MB_OK);
                        //memset(board, EMPTY, sizeof(board));
                        break;
                    }
                    if (player1 == 1)
                    {
                        bf[2]++;
                        jinwei(bf, 2);
                        Sleep(50);
                        memset(board, EMPTY, sizeof(board));
                        people == true ? MessageBox(GetHWnd(), _T("玩家O胜利"), _T("游戏结束"), MB_OK) : MessageBox(GetHWnd(), _T("机器胜利"), _T("游戏结束"), MB_OK);
                    }
                    break;
                }
                // 判断是否平局
                if (winner == 'N' ||  isTieScenarioForAnyPlayer(board, OPPONENT))
                {
                    Sleep(50);
                    MessageBox(GetHWnd(), _T("平局"), _T("游戏结束"), MB_OK);
                    memset(board, EMPTY, sizeof(board));
                    break;
                }

            }
            //交换下棋者
            player1 = player1 == 2 ? 1 : 2;
        }
        // 判断分数是否超出表示范围
        if (bifen[player1 - 1] == 1000)
        {
            Sleep(50);
            if (player1 == 2)
            {
                people == true ? MessageBox(GetHWnd(), _T("玩家X满分"), _T("游戏结束"), MB_OK) : MessageBox(GetHWnd(), _T("玩家满分"), _T("游戏结束"), MB_OK);
            }
            else
            {
                people == true ? MessageBox(GetHWnd(), _T("玩家O满分"), _T("游戏结束"), MB_OK) : MessageBox(GetHWnd(), _T("机器满分"), _T("游戏结束"), MB_OK);
            }
            memset(board, EMPTY, sizeof(board));
            bifenchongzhi(bifen, bf);
            closegraph();
            return 1;
        }
    }
    closegraph();
    return 0;
}
// 主界面函数
int gui()
{
    initgraph(450, 280);

    // 获取窗口句柄
    HWND hwnd = GetHWnd();

    // 获取屏幕尺寸
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 计算窗口左上角坐标使其位于屏幕中央
    int windowWidth = 450;  // 窗口宽度
    int windowHeight = 280; // 窗口高度
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

    // 重新调整窗口位置和大小，以适应新的样式
    SetWindowPos(hwnd, NULL, x, y, windowWidth + 16, windowHeight + 39, SWP_FRAMECHANGED);

    HICON hIcon = (HICON)LoadImage(NULL, L"图标.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    if (hIcon == NULL)
    {
        MessageBox(NULL, L"无法加载图标文件！", L"错误", MB_OK | MB_ICONERROR);
    }
    else
    {
        // 设置窗口图标
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    }

    setbkcolor(WHITE);
    cleardevice();
    setfillcolor(RGB(200, 220, 240));
    solidrectangle(0, 0, 450, 280);

    settextcolor(BLACK);
    setbkcolor(RGB(200, 220, 240));
    settextstyle(30, 0, _T("宋体"));
    outtextxy(150, 20, _T("井字棋游戏"));

    setbkcolor(RGB(150, 160, 170));
    setfillcolor(RGB(150, 160, 170));
    setcolor(BLACK);
    setlinecolor(RGB(50, 55, 60));

    settextstyle(20, 0, _T("宋体"));
    setlinestyle(PS_SOLID, 3);

    rectangle(50, 100, 400 + 2, 130 + 2);
    solidrectangle(50, 100, 400, 130);
    outtextxy(200, 105, _T("退出(O)"));  // 显示快捷键

    rectangle(50, 160, 400 + 2, 190 + 2);
    solidrectangle(50, 160, 400, 190);
    outtextxy(180, 165, _T("人人对弈(R)"));  // 显示快捷键

    rectangle(50, 220, 400 + 2, 250 + 2);
    solidrectangle(50, 220, 400, 250);
    outtextxy(180, 225, _T("人机对弈(J)"));  // 显示快捷键

    ExMessage m;  // 定义消息变量
    while (true)
    {
        // 获取一条鼠标或按键消息
        m = getmessage(EX_MOUSE | EX_CHAR);
        switch (m.message)
        {
        case WM_LBUTTONUP:
            if (m.x >= 50 && m.x <= 400 && m.y >= 220 && m.y <= 250)
            {
                // 关闭图形窗口
                closegraph();
                return 2;
            }
            else if (m.x >= 50 && m.x <= 400 && m.y >= 100 && m.y <= 130)
            {
                closegraph();
                return 0;
            }
            else if (m.x >= 50 && m.x <= 400 && m.y >= 160 && m.y <= 190)
            {
                closegraph();
                return 1;
            }
            break;
        case WM_CHAR:  // 键盘字符输入事件
            if (toupper(m.ch) == 'J')  // 不区分大小写比较
            {
                // 关闭图形窗口
                closegraph();
                return 2;
            }
            else if (toupper(m.ch) == 'O')  // 不区分大小写比较
            {
                closegraph();
                return 0;
            }
            else if (toupper(m.ch) == 'R')  // 不区分大小写比较
            {
                closegraph();
                return 1;
            }
            break;  // 别忘了在每个 case 结束后添加 break ，除非您故意希望继续执行下一个 case
        }
    }
    return 0;
}
/*
int main()
{
    char board[BOARD_SIZE][BOARD_SIZE];        //定义一个3x3的棋盘
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] = ' ';
        }
    }

    int e = 0;
    int e1 = 0;

    // 初始化图形模式
    initgraph(400, 300);

    // 获取窗口句柄
    HWND hwnd = GetHWnd();

    // 去除窗口标题栏，但保留边框
    SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_CAPTION);

    // 获取屏幕尺寸
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 计算窗口左上角坐标使其位于屏幕中央
    int windowWidth = 400;  // 窗口宽度
    int windowHeight = 300; // 窗口高度
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

    // 重新调整窗口位置和大小，以适应新的样式
    SetWindowPos(hwnd, NULL, x, y, windowWidth, windowHeight, SWP_FRAMECHANGED);

    IMAGE HC(400,300);
    SetWorkingImage(&HC);

    srand(static_cast<unsigned int>(time(nullptr)));
    char j[5] = { '0', '0', '0', '%' };

    int fx1 = 200;
    int fy1 = 150;
    int cx = 100;
    int cy = 150;

    int sf = 5;
    int sc = 5;

    int tx1width = 40;
    int i = 0;
    int tx2width = 40;
    setbkmode(TRANSPARENT);
    for (; i <= 200; i++)
    {
        SetWorkingImage(&HC);

        setfillcolor(RGB(240, 240, 240));
        solidrectangle(0, 0, 400, 300);

        //setfillcolor(RGB(180, 180, 180));
        //POINT pts1[] = { {5, 295}, {395, 295}, {400, 300} ,{0,300} };
        //solidpolygon(pts1,4);
        //POINT pts2[] = { {395, 5}, {400-1, 0}, {400, 300} ,{395,295} };
        //solidpolygon(pts2, 4);

        //setfillcolor(RGB(230, 230, 230));
        //solidrectangle(5,5,395,295);

        //setlinestyle(PS_SOLID , 1);
        //setlinecolor(BLACK);
        //rectangle(5, 5, 395, 295);

        //rectangle(0, 0, 400-1, 300-1);

        setlinecolor(RGB(100, 120, 140));
        fork1(fx1, fy1, tx1width);
        circle1(cx, cy, tx2width, 7);

        fx1 += sf;
        fy1 += sf-2;

        cx -= sc;
        cy += sf-2;
        if (fx1 > 350)
        {
            fx1 = 200;
            fy1 = 150;
            //sf = rand() % 4;
            tx1width = 40;
        }
        if (cx < -20)
        {
            //sc = rand() % 4;
            cx = 100;
            cy =150;
            tx2width = 40;
        }
        tx1width += 5;
        tx2width += 5;
        //标题
        //setbkcolor(RGB(200, 220, 240));
        settextcolor(BLACK);
        settextstyle(30, 0, _T("宋体"));
        outtextxy(130, 20, _T("井字棋游戏"));

        //进度条
        setlinestyle(PS_SOLID, 3);
        setlinecolor(RGB(50, 50, 50));
        setfillcolor(RGB(100, 120, 140));

        rectangle(45, 246, 374, 264);
        solidrectangle(47, 250 - 2, 50 + i / 2 * 3 + 22, 260 + 2);

        settextstyle(20, 0, _T("宋体"));
        outtextxy(45, 220, _T("进入游戏中....."));

        for (int p = 0; p < 4; p++)
        {
            outtextxy(p*10 + 340, 270,(wchar_t)j[p]);
        }

        // 设置绘图目标为绘图窗口
        SetWorkingImage();
        // 将 img 对象显示在绘图窗口中
        putimage(0, 0, &HC);

        j[2] += i % 2 == 0 ? 1 : 0;
        for (int z = 0; z < 3; z++)
        {
            if (j[z] == '9' + 1)
            {
                j[z] = '0';
                j[z - 1]++;
            }
        }

        int d = rand() % 20;
        if (d == 7)
        {
            Sleep(rand() % 100+200);
        }
    }
    Sleep(500);
    closegraph();

ks:;
    e = gui();

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] = ' ';
        }
    }
    if (e != e1)
    {
        bifenchongzhi(bifen, bf);
        e1 = e;
    }

    if (e == 1&& boardgui(board,true))
    {
        Sleep(100);
        goto ks;
    }
    if (e == 2&& boardgui(board,false)) {
        Sleep(100);
        goto ks;
    }

    return 0;
}*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        closegraph();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    char board[BOARD_SIZE][BOARD_SIZE];        //定义一个 3x3 的棋盘
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] =' ';
        }
    }

    int e = 0;
    int e1 = 0;

    // 初始化图形模式
    initgraph(400, 300);

    // 获取窗口句柄
    HWND hwnd = GetHWnd();

    // 去除窗口标题栏，但保留边框
    SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_CAPTION);

    // 获取屏幕尺寸
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 计算窗口左上角坐标使其位于屏幕中央
    int windowWidth = 400;  // 窗口宽度
    int windowHeight = 300; // 窗口高度
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

    // 重新调整窗口位置和大小，以适应新的样式
    SetWindowPos(hwnd, NULL, x, y, windowWidth, windowHeight, SWP_FRAMECHANGED);

    IMAGE HC(400, 300);
    SetWorkingImage(&HC);

    srand(static_cast<unsigned int>(time(nullptr)));
    char j[5] = { '0', '0', '0', '%' };

    int fx1 = 200;
    int fy1 = 150;
    int cx = 100;
    int cy = 150;

    int sf = 5;
    int sc = 5;

    int tx1width = 40;
    int i = 0;
    int tx2width = 40;
    setbkmode(TRANSPARENT);
    for (; i <= 200; i++)
    {
        SetWorkingImage(&HC);

        setfillcolor(RGB(240, 240, 240));
        solidrectangle(0, 0, 400, 300);

        //setfillcolor(RGB(180, 180, 180));
        //POINT pts1[] = { {5, 295}, {395, 295}, {400, 300},{0,300} };
        //solidpolygon(pts1,4);
        //POINT pts2[] = { {395, 5}, {400-1, 0}, {400, 300},{395,295} };
        //solidpolygon(pts2, 4);

        //setfillcolor(RGB(230, 230, 230));
        //solidrectangle(5,5,395,295);

        //setlinestyle(PS_SOLID, 1);
        //setlinecolor(BLACK);
        //rectangle(5, 5, 395, 295);

        //rectangle(0, 0, 400-1, 300-1);

        setlinecolor(RGB(100, 120, 140));
        fork1(fx1, fy1, tx1width);
        circle1(cx, cy, tx2width, 7);

        fx1 += sf;
        fy1 += sf - 2;

        cx -= sc;
        cy += sf - 2;
        if (fx1 > 350)
        {
            fx1 = 200;
            fy1 = 150;
            //sf = rand() % 4;
            tx1width = 40;
        }
        if (cx < -20)
        {
            //sc = rand() % 4;
            cx = 100;
            cy = 150;
            tx2width = 40;
        }
        tx1width += 5;
        tx2width += 5;
        //标题
        //setbkcolor(RGB(200, 220, 240));
        settextcolor(BLACK);
        settextstyle(30, 0, _T("宋体"));
        outtextxy(130, 20, _T("井字棋游戏"));

        //进度条
        setlinestyle(PS_SOLID, 3);
        setlinecolor(RGB(50, 50, 50));
        setfillcolor(RGB(100, 120, 140));

        rectangle(45, 246, 374, 264);
        solidrectangle(47, 250 - 2, 50 + i / 2 * 3 + 22, 260 + 2);

        settextstyle(20, 0, _T("宋体"));
        outtextxy(45, 220, _T("进入游戏中....."));

        for (int p = 0; p < 4; p++)
        {
            outtextxy(p * 10 + 340, 270, (wchar_t)j[p]);
        }

        // 设置绘图目标为绘图窗口
        SetWorkingImage();
        // 将 img 对象显示在绘图窗口中
        putimage(0, 0, &HC);

        j[2] += i % 2 == 0 ? 1 : 0;
        for (int z = 0; z < 3; z++)
        {
            if (j[z] == '9' + 1)
            {
                j[z] = '0';
                j[z - 1]++;
            }
        }

        int d = rand() % 20;
        if (d == 7)
        {
            Sleep(rand() % 100 + 200);
        }
    }
    Sleep(500);
    closegraph();

   ks: e = gui();

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] = ' ';
        }
    }
    if (e != e1)
    {
        bifenchongzhi(bifen, bf);
        e1 = e;
    }

    if (e == 1 && boardgui(board, true))
    {
        Sleep(100);
        goto ks;  // 通常不建议使用 goto 语句，这里可以考虑使用循环来替代
    }
    if (e == 2 && boardgui(board, false))
    {
        Sleep(100);
        goto ks;  // 通常不建议使用 goto 语句，这里可以考虑使用循环来替代
    }

    return 0;
}