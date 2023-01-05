// MineSweeperLayout.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <tchar.h>  // This is for unicode.
#include <windows.h> // This is for objects used in Windows
#include <tlhelp32.h> // This is for memory access to other processes

#include "Minesweeper.h"

int main()
{
    CMinesweeper Minesweeper;
    Minesweeper.Debug = true; // Set to 'true' for debug comments
    Minesweeper.Run(); // Make sure minesweeper is running before execution
    system("pause");

    return EXIT_SUCCESS;
}

