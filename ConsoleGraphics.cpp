
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <conio.h>
#include "Entity.h"
using namespace std;

const int m_screenWidth = 120;
const int m_screenHeight = 40;
const int m_mapWidth = 12;
const int m_mapHeight = 10;
const float gravity = 9.81;
const float acceleration = 5;
const float maxSpeed = 15;
WCHAR emptyChar = ' ';
auto tp1 = chrono::system_clock::now();
auto tp2 = chrono::system_clock::now();
Entity Player;

int main()
{
    wstring map;
    map += L"############";
    map += L"#..........#";
    map += L"#.......####";
    map += L"#..........#";
    map += L"#......##..#";
    map += L"#......##..#";
    map += L"#..........#";
    map += L"###........#";
    map += L"#......#####";
    map += L"############";


    Player = { 1,17,0,0,false,(WCHAR)0xfeff263A };
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
    for (;;) {
        tp2 = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();

        screen[(short)Player.Y * m_screenWidth + (short)Player.X] = emptyChar;

        /*if (GetAsyncKeyState(VK_UP) & 0x8000) {
            if (Player.Y > 0) {
                Player.Y -= 5.0 * fElapsedTime;
            }
        }*/
        
        if (GetAsyncKeyState(VK_UP) & 0x8000 && !Player.jump) {
            Player.ySpeed = -10;
            Player.jump = true;
        }

        Player.ySpeed += gravity * fElapsedTime;
        Player.Y += Player.ySpeed * fElapsedTime;
        
        if (Player.Y > m_screenHeight /* || map[(int)(Player.Y / 4) * m_mapWidth + (int)Player.X] == '#'*/) {
            Player.Y -= 1;
            Player.ySpeed = 0;
            Player.jump = false;
        }
        
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
        Player.X += Player.xSpeed * fElapsedTime;
        if (Player.X < 0) {
            Player.X = 0;
            Player.xSpeed = 0;
        }
        if (Player.X > m_screenWidth) {
            Player.X = m_screenWidth - 1;
            Player.xSpeed = 0;
        }
        if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) {
            return 0;
        }

        for (int nx = 0; nx < m_mapWidth; nx++)
            for (int ny = 0; ny < m_mapHeight; ny++) {
                screen[(ny+1) * m_screenWidth + nx] = map[ny * m_mapWidth + nx];
            }
        swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, FPS=%3.2f ", Player.xSpeed, Player.ySpeed, 1.0f / fElapsedTime);
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