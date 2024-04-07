
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <conio.h>
#include "Entity.h"
using namespace std;

const int m_screenWidth = 120;
const int m_screenHeight = 40;
const int m_mapWidth = 60;
const int m_mapHeight = 10;
const int map_visible = 30;
const float gravity = 9.81;
const float acceleration = 10;
const float maxSpeed = 15;
const float jumpSpeed = 15;
const WCHAR emptyChar = ' ';
const WCHAR fullBlock = (WCHAR)0xfeff2588;
auto tp1 = chrono::system_clock::now();
auto tp2 = chrono::system_clock::now();
Entity Player;
bool hit = false;
int offset = 0;

int main()
{
    wstring map;
    map += L"############################################################";
    map += L"#                                                          #";
    map += L"#                         #####                         ####";
    map += L"#                      ##    ##                      ##    #";
    map += L"#                 ###    ##                     ###    ##  #";
    map += L"#           ###          ##               ###          ##  #";
    map += L"#     ###                           ###                    #";
    map += L"###                          ####                          #";
    map += L"#                        ######                        #####";
    map += L"############################################################";


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
    int testVariable = 0;
    bool s_pressed = false;
    for (;;) {
        // Time elapsed
        tp2 = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();
        // Clear previous position
        screen[(int)Player.Y * m_screenWidth + (int)(Player.X-offset)] = emptyChar;
        // See if we can and will jump
        if (GetAsyncKeyState(VK_UP) & 0x8000 && !Player.jump) {
            Player.ySpeed = -jumpSpeed;
            Player.jump = true;
        }else if (Player.Y > m_screenHeight) {
            Player.Y = m_screenHeight - 1;
            Player.ySpeed = 0;
            Player.jump = false;
        } else if (screen[(int)(Player.Y - 0.5) * m_screenWidth + (int)(Player.X - offset)] == fullBlock && !hit) {
            Player.ySpeed = 0;
            hit = true;
        } else if (screen[(int)(Player.Y + 0.5) * m_screenWidth + (int)(Player.X - offset)] == fullBlock) {
            Player.ySpeed = 0;
            Player.jump = false;
            hit = false;
        }
        // Move player y-wise
        Player.ySpeed += gravity * fElapsedTime;
        Player.Y += Player.ySpeed * fElapsedTime;
        // Check for controls
        // Pressing left
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            if (Player.xSpeed > -maxSpeed) {
                Player.xSpeed -= acceleration * fElapsedTime;
            }
            if (Player.xSpeed < -maxSpeed) {
                Player.xSpeed = -maxSpeed;
            }
        }
        // Pressing right
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            if (Player.xSpeed < maxSpeed) {
                Player.xSpeed += acceleration* fElapsedTime;
            }
            if (Player.xSpeed > maxSpeed) {
                Player.xSpeed = maxSpeed;
            }
        }
        // Breaking down
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
        // Move player x-wise
        Player.X += Player.xSpeed * fElapsedTime;
        if (Player.X < 0) {
            Player.X = 0;
            Player.xSpeed = -Player.xSpeed;
        }
        // Check for out of bounds
        if (Player.X > m_screenWidth + offset) {
            Player.X = m_screenWidth + offset - 1;
            Player.xSpeed = -Player.xSpeed;
        }
        // Check for collision
        if (Player.xSpeed < 0 && screen[(int)Player.Y * m_screenWidth + (int)(Player.X - 0.5 - offset)] == fullBlock) {
            Player.xSpeed = -Player.xSpeed;
        } else if (Player.xSpeed > 0 && screen[(int)Player.Y * m_screenWidth + (int)(Player.X + 0.5 - offset)] == fullBlock) {
            Player.xSpeed = -Player.xSpeed;
        }
        // Quit key
        if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) {
            return 0;
        }
        // Calculate offset
        if (Player.X > m_screenWidth / 2) {
                offset = (int)(Player.X - (m_screenWidth / 2));
                if (offset > m_screenWidth) {
                    offset = m_screenWidth;
                }
        }
        // Draw screen
        for (int x = 0; x < m_screenWidth; x++)
            for (int y = 0; y < m_screenHeight; y++) {
                if (map[(y / 4) * m_mapWidth + ((x + offset) / 4)] == '#') {
                    screen[y * m_screenWidth + x] = fullBlock;
                } else {
                    screen[y * m_screenWidth + x] = emptyChar;
                }
            }
        // Add statistics
        swprintf_s(screen, 75, L"Offset=%d, X=%d, Y=%d, xSpeed = %3.2f, ySpeed = %3.2f, FPS=%3.2f", offset, (int)Player.X, (int)Player.Y, Player.xSpeed, Player.ySpeed, 1.0f / fElapsedTime);
        // Print player location.
        screen[(short)Player.Y * m_screenWidth + (short)(Player.X - offset)] = Player.symbol;
        // last position to string terminator
        screen[m_screenWidth * m_screenHeight - 1] = '\0';
        // Draw screen
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