#pragma once

#include <iostream>
#include <tchar.h>  // This is for unicode.
#include <windows.h> // This is for objects used in Windows
#include <tlhelp32.h> // This is for memory access to other processes

using namespace std;

class CMinesweeper {
private:
    void _printDebug(const wchar_t* message);
    void _printDebug(const wstring message);
    void _printFormat(const char* arg, ...);
    void _print(const wstring message);
    void _print(const wchar_t* message);
    DWORD _GetProcessID();
    BOOL _GetMatrix();
    void _PrintMatrix();
    
public:

    void Run();
    BOOL Debug = true;
    DWORD processID = NULL;
    LONGLONG matrixAddress = 0x01005360; // Starting address of mine matrix
    int matrixLength = 0x56a0 - 0x5350; // 848 bytes maximum length of mine matrix
    BYTE* matrixBuffer = NULL;

    CMinesweeper() {}
    ~CMinesweeper() {
        delete[] matrixBuffer; // Free the memory allocation
    }
};

