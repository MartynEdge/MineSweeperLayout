#pragma once

#include "Minesweeper.h"
#include <iostream>

using namespace std;

// Macro for checking Debug status for custom print commands
#define _printDebugM if (Debug)

void CMinesweeper::_printDebug(const wstring message) {
	if (Debug) wcout << message; 
}

void CMinesweeper::_printDebug(const wchar_t* message) {
	if (Debug) _tprintf(message);
}

void CMinesweeper::_print(const wstring message) {
	wcout << message;
}

void CMinesweeper::_print(const wchar_t* message) {
	_tprintf(message);
}

DWORD CMinesweeper::_GetProcessID() {

	HANDLE processSnapshot;
	PROCESSENTRY32 processEntry;

	// Create a snapshot of all active processes
	processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (processSnapshot == INVALID_HANDLE_VALUE) {
		 _printDebug(L"[x] Error: Unable to create process snapshot."s);
		return false;
	}

	// Set the size of the structure
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve the first process
	if (!Process32First(processSnapshot, &processEntry)) {
		_printDebug(L"[x] Error: Unable to retrieve first process snapshot."s);
		CloseHandle(processSnapshot);
		return false;
	}

	// Check each process for "winmine.exe"
	BOOL foundWinmine = false;
	do{
		_printDebugM _tprintf(_T("[Process ID: 0x%08X] Process name: %s \n"),processEntry.th32ProcessID, processEntry.szExeFile);
		if (processEntry.szExeFile == L"winmine.exe"s) {
			// Found process for winmine.exe 
			foundWinmine = true;
			break;
		}
	} while (Process32Next(processSnapshot, &processEntry));

	CloseHandle(processSnapshot);

	// If failed -> return NULL
	if (!foundWinmine) {
		_printDebug(L"[i] Could not find process of winmine.exe\n"s);
		return NULL;
	}

	// If successful -> return process ID
	_printDebugM _tprintf(_T("[i] Found process of winmine.exe to be 0x%08X\n"),processEntry.th32ProcessID);
	return processEntry.th32ProcessID;
}

void CMinesweeper::_PrintMatrix() {
	int i = 0;
	BYTE v;
	bool withinMatrix = true;
	bool finishedMatrix = false;
	int markerCount = 0;

	_tprintf(_T("\nMinesweeper layout"));
	_tprintf(_T("\n------------------\n"));

	do {
		v = *(matrixBuffer+ i);
		if (v == 0x8F) {
			_tprintf(_T("[M]")); // Mine square
			markerCount = 0;
		}
		else if (matrixBuffer[i] == 0x10) {
			markerCount++;
			if (i % 32 == 0) {
				withinMatrix = true;
				_tprintf(_T("\n")); // Marker indicates start of row
			}
			else {
				withinMatrix = false; // Marker indicates end of row
			}
		}
		else {
			markerCount = 0;
			if (withinMatrix) _tprintf(_T("[_]")); // Empty square
		}

		i++;
		if (i > 1) finishedMatrix = (markerCount>=3); // Exit when an arbitrary number of markers found (must be at least 3)
	} while (!finishedMatrix);

	_tprintf(_T("\n"));
}

BOOL CMinesweeper::_GetMatrix() {

	// Allocate memory on the heap. This is released in the class deconstructor.
	matrixBuffer = new BYTE[matrixLength];
	SIZE_T bytesRead;
	memset(matrixBuffer, 0x0, matrixLength);

	_printDebugM _tprintf(_T("[i] Attempting to read memory at address %I64x of PID %d\n"), matrixAddress, processID);

	// Attempt to read memory from the winmine.exe process
	if (!Toolhelp32ReadProcessMemory(processID, (LPCVOID)matrixAddress, matrixBuffer, matrixLength, &bytesRead)) {
		_printDebug(L"[x] Cannot read winmine.exe memory.\n"s);
		return false;
	}
	
	_printDebugM _tprintf(_T("[i] Read bytes of memory: %d\n"), (int)bytesRead);
	
	return true;
}

void CMinesweeper::Run() {
	
	// Get the process ID
	processID=_GetProcessID();
	if (!processID) {
		_print(L"[x] Unable to continue without a valid process ID. Exiting.\n");
		return;
	}

	// Get the mine layout
	if (!_GetMatrix()) {
		_print(L"[x] Unable to continue as could not read winmine.exe memory. Exiting.\n");
		return;
	}

	// Print the mine layout
	_PrintMatrix();

}