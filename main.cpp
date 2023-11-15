#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include <chrono>
using namespace std;
#define ll long long
#define GOLDEN_RATIO 0x9e3779b9;
const int TIMEOUT = 200;
const int minPlusTime = 5000;
int score = 0;

const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

int main();

void setColor(int color) {
    SetConsoleTextAttribute(console, color);
}

void gotoxy(int x, int y) {
    SetConsoleCursorPosition(console, {(SHORT)x, (SHORT)y});
}

void setCursor(bool visible, int size=20) {
    CONSOLE_CURSOR_INFO lp;
    lp.bVisible = visible;
    lp.dwSize = (DWORD)size;
    SetConsoleCursorInfo(console, &lp);
}

struct coor {
    int x, y;
};


deque<coor> dq;

int waited_getchar_util(int timeout) {
    auto st = chrono::steady_clock::now();
    auto cur = chrono::steady_clock::now();
    while(!kbhit()) {
        cur = chrono::steady_clock::now();
        int dif = chrono::duration_cast<chrono::milliseconds>(cur-st).count();
        if(dif>timeout) {
            return -1;
        }
    }
    int c = getch();
    while(c != 72 && c != 75 && c != 77 && c != 80) c = getch();
    cur = chrono::steady_clock::now();
    int dif = chrono::duration_cast<chrono::milliseconds>(cur-st).count();
    while(dif<=timeout) {
        cur = chrono::steady_clock::now();
        dif = chrono::duration_cast<chrono::milliseconds>(cur-st).count(); 
    }
    return c;
}

void game(int mode) {
    srand(time(NULL));
    system("cls");
    gotoxy(0, 0);
    dq = deque<coor>(); 
    coor di = {0, -1};
    char snake = '|';
    int n = mode*8;
    int m = n*2;
    bool snake_body[n+10][m+10];
    for(int i=0; i<=n+1; i++) {
        for(int j=0; j<=m+1; j++) {
            if(i==0||i==n+1||j==0||j==m+1) {
                setColor(12);
                cout << "#";
            } else {
                setColor(10);
                cout << ".";
            }
            snake_body[i][j]=false;
        }
        cout << "\n";
    }
    for(int i=0; i<4; i++) {
        int x = (m>>1)+1, y = (n>>1)+1+i;
        dq.push_back({x, y});
        gotoxy(x, y);
        snake_body[y][x] = true;
        setColor(11);
        cout << "|";
    }
    bool gameOver = false;
    char c;
    auto plus_st = chrono::steady_clock::now();
    time_t timer_st, timer_now;
    time(&timer_st);
    coor getPlusPosition = {-1, -1};
    while(!gameOver) {
        time(&timer_now);
        gotoxy(m+10, 0);
        cout << "- Status -";
        gotoxy(m+10, 1);
        cout << "Score: " << score;
        gotoxy(m+10, 2);
        int total = timer_now-timer_st;
        int minute = 0, second = 0;
        if(total / 60 > 0) {
            minute += total/60;
        }
        second = total % 60;
        string show_minute = to_string(minute);
        string show_second = to_string(second);
        if(show_minute.length() == 1) {
            show_minute+=show_minute[0];
            show_minute[0] = '0';
        }
        if(show_second.length() == 1) {
            show_second+=show_second[0];
            show_second[0] = '0';
        }
        cout << "Play time: " << show_minute << ":" << show_second;
        auto plus_now = chrono::steady_clock::now();
        auto plus_dif = chrono::duration_cast<chrono::milliseconds>(plus_now-plus_st).count();
        if(plus_dif>minPlusTime+(rand()%10000)&&getPlusPosition.x == -1) {
            int rand_x = 1+(rand()%m);
            int rand_y = 1+(rand()%n);
            while(snake_body[rand_y][rand_x] == true) {
                rand_x = 1+(rand()%m);
                rand_y = 1+(rand()%n);
            }
            getPlusPosition = {rand_x, rand_y};
            gotoxy(rand_x, rand_y);
            setColor(14);
            cout << "+";
        }
        switch(int(c)) {
            case 72:
            di = {0, -1};
            break;
            case 80:
            di = {0, 1};
            break;
            case 75:
            di = {-1, 0};
            break;
            case 77:
            di = {1, 0};
            break;
        }
        if(di.x==0&&abs(di.y)==1) {
            snake = '|';
        } else snake = '-';
        coor head = dq.front();
        if(head.x+di.x<1||head.x+di.x>m) {
            if(di.x == 1) {
                head.x = 0;
            } else head.x = m+1;
        }
        if(head.y+di.y<1||head.y+di.y>n) {
            if(di.y == 1) {
                head.y = 0;
            } else head.y = n+1;
        }
        if(snake_body[head.y+di.y][head.x+di.x] == true) {
            gotoxy(head.x+di.x, head.y+di.y);
            setColor(207);
            cout << "X";
            gameOver = true;
            gotoxy(m+10, 4);
            setColor(12);
            cout << "GAME OVER!";
            Sleep(2000);
            while(!kbhit()) {
                gotoxy(m+10, 5);
                setColor(15);
                cout << "Press any key to return to main menu...";
            }
            main();
            return;
        }
        if(head.x+di.x==getPlusPosition.x&&head.y+di.y==getPlusPosition.y) {
            getPlusPosition = {-1, -1};
            int addScore = 1+(rand()%5);
            score += addScore;
            for(int i=0; i<addScore; i++) dq.push_back({dq.back().x-di.x, dq.back().y-di.y});
            plus_st = chrono::steady_clock::now();
        }
        dq.push_front({head.x+di.x, head.y+di.y});
        gotoxy(head.x+di.x, head.y+di.y);
        snake_body[head.y+di.y][head.x+di.x] = true;
        setColor(11);
        cout << snake;
        gotoxy(dq.back().x, dq.back().y);
        snake_body[dq.back().y][dq.back().x] = false;
        dq.pop_back();
        setColor(10);
        cout << ".";
        int getKey = waited_getchar_util(TIMEOUT);
        if(getKey != -1) {
            if((int(c) == 72 && getKey == 80) ||
               (int(c) == 80 && getKey == 72) ||
               (int(c) == 75 && getKey == 77) ||
               (int(c) == 77 && getKey == 75)) {
                    c = c;
               } else c = char(getKey);
        }
    }
}

string game_mode[4] = {"Easy", "Medium", "Hard", "Exit"};

int main() {
    system("cls");
    srand(time(NULL));
    setCursor(false);
    setColor(11);
    cout << "---------------------------------------\n";
    cout << "|       Snake game by phone64bit      |\n";
    cout << "---------------------------------------\n";
    Sleep(500);
    setColor(10);
    cout << "Select Game mode\n";
    int select = 0;
    while(true) {
        for(int i=0; i<4; i++) {
            gotoxy(0, 4+i);
            if(i==select) setColor(12); else setColor(15);
            cout<<i+1<<". "<<game_mode[i];
        }
        char c = getch();
        while(c != 72 && c != 80 && c != 13) {
            c = getch();
        }
        if(c == 72 && select >= 1) select--;
        if(c == 80 && select <= 2) select++;
        if(c==13) {
            if(select < 3) {
                game(select+1);
                return 0;
            } else exit(0);
        }
    }
    return 0;
}