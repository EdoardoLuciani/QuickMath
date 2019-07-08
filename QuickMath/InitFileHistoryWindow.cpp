#include "stdafx.h"
#include "QuickMath.h"
#include "external_variables.h"
#include "InitFileHistoryWindow.h"

HWND hwnd_edit_head[3];
HWND* hwnd_edit_cell_ptr;
int n_rows;

LRESULT CALLBACK InitFileHistoryWindowProc(HWND file_history_hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_COMMAND: {

		} break;
	case WM_SIZE:
		for (int i = 0; i < 3; i++) {
			SetWindowPos(hwnd_edit_head[i], NULL, 26 + ((LOWORD(lParam) - 26 * 2) / 3) * i, 40, (LOWORD(lParam) - 26 * 2) / 3, 30, SWP_NOOWNERZORDER);
		}

		for (int i = 0, index = 0; i < n_rows; i++) {
			for (int j = 0; j < 3; j++, index++) {
				SetWindowPos(hwnd_edit_cell_ptr[index], NULL, 26 + ((LOWORD(lParam) - 26 * 2) / 3) * j, 70 + 30 * i, (LOWORD(lParam) - 26 * 2) / 3, 30, SWP_NOOWNERZORDER);
			}
		}
		break;
	case WM_CLOSE:
		DestroyWindow(file_history_hWnd);
		break;
	case WM_DESTROY:
		DestroyWindow(file_history_hWnd);
		break;
	default:
		return DefWindowProc(file_history_hWnd, message, wParam, lParam);
	}
	return 0;
}

void InitFileHistoryWindow(std::array<std::vector<std::wstring>, 3> &operations_history) {

	WCHAR szWindowClass[100];            // the main window class name
	wcscpy(szWindowClass, L"HISTORY");

	WNDCLASSEX windowclassforwindow;
	ZeroMemory(&windowclassforwindow, sizeof(WNDCLASSEX));
	windowclassforwindow.cbClsExtra = NULL;
	windowclassforwindow.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow.cbWndExtra = NULL;
	windowclassforwindow.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowclassforwindow.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclassforwindow.hIcon = NULL;
	windowclassforwindow.hIconSm = NULL;
	windowclassforwindow.hInstance = hInst;
	windowclassforwindow.lpfnWndProc = (WNDPROC)InitFileHistoryWindowProc;
	windowclassforwindow.lpszClassName = szWindowClass;
	windowclassforwindow.lpszMenuName = NULL;
	windowclassforwindow.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassExW(&windowclassforwindow);

	RECT rMyRect;
	GetClientRect(hWnd, (LPRECT)& rMyRect);
	ClientToScreen(hWnd, (LPPOINT)& rMyRect.left);

	HWND history_window = CreateWindowEx(NULL,
		szWindowClass,
		L"History",
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		rMyRect.left,
		rMyRect.top,
		640,
		480,
		NULL,
		NULL,
		hInst,
		NULL);
	ShowWindow(history_window, SW_SHOW);

	WCHAR edit_heat_titles[3][30];
	LoadStringW(hInst, IDS_FILEHISTORYHEAD1, edit_heat_titles[0], 30);
	LoadStringW(hInst, IDS_FILEHISTORYHEAD2, edit_heat_titles[1], 30);
	LoadStringW(hInst, IDS_FILEHISTORYHEAD3, edit_heat_titles[2], 30);

	HFONT hFontEdit = CreateFont(25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Segoe UI"));

	for (int i = 0; i < 3; i++) {
		hwnd_edit_head[i] = CreateWindowW(
			L"EDIT",  // Predefined class; Unicode assumed 
			NULL,      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY,  // Styles 
			26 + 190 * i,         // x position 
			40,         // y position 
			190,        // Button width
			30,        // Button height
			history_window,     // Parent window
			NULL,       // No menu.
			hInst,
			NULL);      // Pointer not needed.
		SendMessage(hwnd_edit_head[i], WM_SETTEXT, 0, (LPARAM)edit_heat_titles[i]);
		SendMessage(hwnd_edit_head[i], WM_SETFONT, (WPARAM)hFontEdit, TRUE);
	}

	n_rows = operations_history[0].size();
	hwnd_edit_cell_ptr = new HWND[n_rows * 3];

	for (int i = 0, index = 0; i < n_rows; i++) {
		for (int j = 0; j < 3; j++, index++) {
			hwnd_edit_cell_ptr[index] = CreateWindowW(
				L"EDIT",
				NULL,
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY,
				26 + 190 * j,
				70+30*i,
				190,
				30,
				history_window,
				NULL,
				hInst,
				NULL);
			SendMessage(hwnd_edit_cell_ptr[index], WM_SETTEXT, 0, (LPARAM)operations_history[j][i].c_str());
			SendMessage(hwnd_edit_cell_ptr[index], WM_SETFONT, (WPARAM)hFontEdit, TRUE);
			SendMessage(hwnd_edit_cell_ptr[index], SB_SETBKCOLOR,0,RGB(126,13,43));
		}
	}
	
}