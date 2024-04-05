
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <conio.h>
#include "Entity.h"
using namespace std;

const int m_screenWidth = 120;
const int m_screenHeight = 40;
WCHAR emptyChar = ' ';
auto tp1 = chrono::system_clock::now();
auto tp2 = chrono::system_clock::now();
Entity Player;

int main()
{
    Player = { 0,0,(WCHAR)0xfeff2588 };
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

        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            if (Player.Y > 0) {
                Player.Y -= 5.0 * fElapsedTime;
            }
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            if (Player.Y < m_screenHeight - 1) {
                Player.Y += 5.0 * fElapsedTime;
            }
        }
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            if (Player.X > 0) {
                Player.X -= 5.0 * fElapsedTime;
            }
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            if (Player.X < m_screenWidth - 1) {
                Player.X += 5.0 * fElapsedTime;
            }
        }
        if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) {
            return 0;
        }
        short currentX = (short)Player.X;
        short currentY = (short)Player.Y;
        screen[currentY * m_screenWidth + currentX] = Player.symbol;
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
0xfeff2588
*/