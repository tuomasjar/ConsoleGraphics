
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <conio.h>
#include "Entity.h"
using namespace std;

const int m_screenWidth = 120;
const int m_screenHeight = 40;
const int m_mapWidth = 30;
const int m_mapHeight = 10;
const float gravity = 9.81;
const float acceleration = 10;
const float maxSpeed = 15;
const float jumpSpeed = 15;
const WCHAR emptyChar = ' ';
const WCHAR fullBlock = (WCHAR)0xfeff2588;
auto tp1 = chrono::system_clock::now();
auto tp2 = chrono::system_clock::now();
Entity Player;

int main()
{
    wstring map;
    map += L"##############################";
    map += L"#............................#";
    map += L"#.........................####";
    map += L"#......................##....#";
    map += L"#.................###....##..#";
    map += L"#...........###..........##..#";
    map += L"#.....###....................#";
    map += L"###..........................#";
    map += L"#........................#####";
    map += L"##############################";


    Player = { 10,17,0,0,false,(WCHAR)0xfeff263A };
    wchar_t* screen = new wchar_t[m_screenWidth * m_screenHeight];
    std::fill_n(screen, m_screenWidth * m_screenHeight, emptyChar);
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER,NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(hConsole, &info);
    info.bVisible = false;
    SetConsoleCursorInfo(hConsole, &info);
    DWORD dwBytesWritten = 0;
    DWORD dwConSize;
    for (int x = 0; x < m_screenWidth; x++)
        for(int y = 0; y < m_screenHeight; y++){
        if (map[(y / 4)*m_mapWidth + (x/4)] == '#') {
            screen[y*m_screenWidth + x] = fullBlock;
        }
    }
    for (;;) {
        tp2 = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();

        screen[(int)Player.Y * m_screenWidth + (int)Player.X] = emptyChar;
        map[(int)(Player.Y / 4) * m_mapWidth + (int)(Player.X / 4)] = '.';
        
        

        if (GetAsyncKeyState(VK_UP) & 0x8000 && !Player.jump) {
            Player.ySpeed = -jumpSpeed;
            Player.jump = true;
        }else if (Player.Y > m_screenHeight) {
            Player.Y = m_screenHeight - 1;
            Player.ySpeed = 0;
            Player.jump = false;
        } else if (screen[(int)(Player.Y - 0.5) * m_screenWidth + (int)(Player.X)] == fullBlock) {
            Player.ySpeed = 0;
        } else if (screen[(int)(Player.Y + 0.5) * m_screenWidth + (int)(Player.X)] == fullBlock) {
            Player.ySpeed = 0;
            Player.jump = false;
        }
        Player.ySpeed += gravity * fElapsedTime;
        Player.Y += Player.ySpeed * fElapsedTime;

        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            if (Player.xSpeed > -maxSpeed) {
                Player.xSpeed -= acceleration * fElapsedTime;
            }
            if (Player.xSpeed < -maxSpeed) {
                Player.xSpeed = -maxSpeed;
            }
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            if (Player.xSpeed < maxSpeed) {
                Player.xSpeed += acceleration* fElapsedTime;
            }
            if (Player.xSpeed > maxSpeed) {
                Player.xSpeed = maxSpeed;
            }
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            if (Player.xSpeed < 0) {
                Player.xSpeed += acceleration * fElapsedTime;
            }
            if (Player.xSpeed > 0) {
                Player.xSpeed -= acceleration * fElapsedTime;
            }
            if (abs(Player.xSpeed) < 0.1) {
                Player.xSpeed = 0;
            }
        }
        Player.X += Player.xSpeed * fElapsedTime;
        if (Player.X < 0) {
            Player.X = 0;
            Player.xSpeed = 0;
        }
        if (Player.X > m_screenWidth) {
            Player.X = m_screenWidth - 1;
            Player.xSpeed = 0;
        }
        if (Player.xSpeed < 0 && screen[(int)Player.Y * m_screenWidth + (int)(Player.X - 1)] == fullBlock) {
            Player.xSpeed = 0;
        } else if (Player.xSpeed > 0 && screen[(int)Player.Y * m_screenWidth + (int)(Player.X + 1)] == fullBlock) {
            Player.xSpeed = 0;
        }
        if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) {
            return 0;
        }
        map[(int)(Player.Y / 4) * m_mapWidth + (int)(Player.X / 4)] = Player.symbol;
        for (int nx = 0; nx < m_mapWidth; nx++)
            for (int ny = 0; ny < m_mapHeight; ny++) {
                screen[(ny+1) * m_screenWidth + nx] = map[ny * m_mapWidth + nx];
            }
        swprintf_s(screen, 60, L"X=%d, Y=%d, xSpeed = %3.2f, ySpeed = %3.2f, FPS=%3.2f ", (int)Player.X, (int)Player.Y, Player.xSpeed, Player.ySpeed, 1.0f / fElapsedTime);
        screen[(short)Player.Y * m_screenWidth + (short)Player.X] = Player.symbol;
        screen[m_screenWidth * m_screenHeight - 1] = '\0';
        
        WriteConsoleOutputCharacter(hConsole, screen, m_screenWidth * m_screenHeight, { 0,0}, &dwBytesWritten);
    }

}

/*
0xfeff263A face
0xfeff03c8 Cactus
0x0xfeff03c8 boat
0xfeff0c06 other boat
0xfeff0da3
0xfeff0df4 grass
0xfeff1099
0xfeff2588 block
*/