
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <conio.h>
using namespace std;

const int m_screenWidth = 120;
const int m_screenHeight = 40;
float cursorX = 0.0;
float cursorY = 0.0;
WCHAR cursor = '#';
WCHAR emptyChar = ' ';

int main()
{
    wchar_t* screen = new wchar_t[m_screenWidth * m_screenHeight];
    std::fill_n(screen, m_screenWidth * m_screenHeight, emptyChar);
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER,NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;
    DWORD dwConSize;
    bool up = false,down = false,left = false,right = false;
    for (;;) {
        screen[(short)cursorY * m_screenWidth + (short)cursorX] = emptyChar;

        if (GetAsyncKeyState(VK_UP) & 0x8000 && !up) {
            if (cursorY > 0) {
                cursorY-= 1;
            }
            up = true;
        } 
        if (GetAsyncKeyState(VK_UP) == 0 && up) {
            up = false; 
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000 && !down) {
            if (cursorY < m_screenHeight - 1) {
                cursorY+= 1;
            }
            down = true;
        } 
        if (GetAsyncKeyState(VK_DOWN) == 0 && down) {
            down = false; 
        }
        if (GetAsyncKeyState(VK_LEFT) & 0x8000 && !left) {
            if (cursorX > 0) {
                cursorX-= 1;
            }
            left = true;
        } 
        if (GetAsyncKeyState(VK_LEFT) == 0 && left) {
            left = false;
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && !right) {
            if (cursorX < m_screenWidth - 1) {
                cursorX+= 1;
            }
            right = true;
        } 
        if (GetAsyncKeyState(VK_RIGHT) == 0 && right) {
            right = false; 
        }
        if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) {
            return 0;
        }
        short currentX = (short)cursorX;
        short currentY = (short)cursorY;
        screen[currentY * m_screenWidth + currentX] = cursor;
        screen[m_screenWidth * m_screenHeight - 1] = '\0';
        
        WriteConsoleOutputCharacter(hConsole, screen, m_screenWidth * m_screenHeight, { 0,0}, &dwBytesWritten);
    }

}

