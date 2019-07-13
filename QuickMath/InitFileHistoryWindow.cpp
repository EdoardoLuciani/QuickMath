#include "stdafx.h"
#include "QuickMath.h"
#include "external_variables.h"
#include "InitFileHistoryWindow.h"

HWND hwnd_edit_head[3];
HWND* hwnd_edit_cell_ptr;
HWND* hwnd_restore_button_ptr;
int n_rows;
int current_window_len_x = 640; 

LRESULT CALLBACK InitFileHistoryWindowProc(HWND file_history_hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE: 
		hwnd_edit_cell_ptr = new HWND[(int)n_rows * 3];
		hwnd_restore_button_ptr = new HWND[n_rows];
		break;
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);

		if ( (wmId >= ID_DYNAMIC_HISTORY_BUTTONS) && (wmId <= ID_MAX_DYNAMIC_HISTORY_BUTTONS)) {
			SendMessage(hWnd, WM_RESTORE_CALCULATION, (WPARAM)wmId - 60000, (LPARAM)NULL);
			DestroyWindow(file_history_hWnd);
		}
		break;
	}
	case WM_SIZE: {
		current_window_len_x = LOWORD(lParam);
		for (int i = 0; i < 3; i++) {
			SetWindowPos(hwnd_edit_head[i], NULL, 26 + ((current_window_len_x - 92) / 3) * i, 40, (current_window_len_x - 92) / 3, 30, SWP_NOOWNERZORDER);
		}

		for (int i = 0, index = 0; i < n_rows; i++) {
			for (int j = 0; j < 3; j++, index++) {
				SetWindowPos(hwnd_edit_cell_ptr[index], NULL, 26 + ((current_window_len_x - 92) / 3) * j, 70 + 30 * i, (current_window_len_x - 92) / 3, 30, SWP_NOOWNERZORDER);
			}
			SetWindowPos(hwnd_restore_button_ptr[i], NULL, 26 + 15 + ((current_window_len_x - 92) / 3) * 3, 70+30*i, NULL, NULL, SWP_NOOWNERZORDER | SWP_NOSIZE);
		}
		break; 
	}
	case WM_VSCROLL: {
		if (LOWORD(wParam) == SB_THUMBTRACK) {
		for (int i = 0; i < 3; i++) {
			SetWindowPos(hwnd_edit_head[i], NULL, 26 + ((current_window_len_x - 92) / 3) * i, 40 - HIWORD(wParam), NULL, NULL, SWP_NOOWNERZORDER | SWP_NOSIZE);
		}

		for (int i = 0, index = 0; i < n_rows; i++) {
			for (int j = 0; j < 3; j++, index++) {
				SetWindowPos(hwnd_edit_cell_ptr[index], NULL, 26 + ((current_window_len_x - 92) / 3) * j, 70 + 30 * i - HIWORD(wParam), NULL, NULL, SWP_NOOWNERZORDER | SWP_NOSIZE);
			}
			SetWindowPos(hwnd_restore_button_ptr[i], NULL, 26 + 15 + ((current_window_len_x - 92) / 3) * 3, 70+30*i - HIWORD(wParam), NULL, NULL, SWP_NOOWNERZORDER | SWP_NOSIZE);
		}
		SCROLLINFO scrolldata;
		scrolldata.cbSize = sizeof(SCROLLINFO);
		scrolldata.fMask = SIF_POS;
		scrolldata.nPos = HIWORD(wParam);
		SetScrollInfo(file_history_hWnd, SB_VERT, &scrolldata, TRUE);
		}
		break; 
	} 
	case WM_CLOSE:
		DestroyWindow(file_history_hWnd);
		break;
	case WM_MOUSEWHEEL: {
		SCROLLINFO scrolldata = {NULL};
		scrolldata.cbSize = sizeof(SCROLLINFO);
		GetScrollInfo(file_history_hWnd, SB_VERT, &scrolldata);
		scrolldata.cbSize = sizeof(SCROLLINFO);
		scrolldata.fMask = SIF_POS;
		scrolldata.nPos += HIWORD(wParam);
		SetScrollInfo(file_history_hWnd, SB_VERT, &scrolldata,TRUE);
		break;
	}
	case WM_DESTROY:
		delete[] hwnd_edit_cell_ptr;
		delete[] hwnd_restore_button_ptr;
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
	n_rows = operations_history[0].size();

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
			26 + 175 * i,         // x position 
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
	
	HBITMAP restore_button_image = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));

	for (int i = 0, index = 0; i < n_rows; i++) {
		for (int j = 0; j < 3; j++, index++) {
			hwnd_edit_cell_ptr[index] = CreateWindowW(
				L"EDIT",
				NULL,
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY,
				26 + 175 * j,
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
		hwnd_restore_button_ptr[i] = CreateWindowW(
			L"BUTTON",  // Predefined class; Unicode assumed 
			NULL,      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_BITMAP,  // Styles 
			26+175*3+15,         // x position 
			70 + 30 * i,         // y position 
			30,        // Button width
			30,        // Button height
			history_window,     // Parent window
			(HMENU) (ID_DYNAMIC_HISTORY_BUTTONS+i),       // No menu.
			hInst,
			NULL);      // Pointer not needed.
		SendMessage(hwnd_restore_button_ptr[i], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)restore_button_image);
	}

	SCROLLINFO scroll_info;
	scroll_info.cbSize = sizeof(SCROLLINFO);
	scroll_info.fMask = SIF_PAGE | SIF_RANGE;
	scroll_info.nPage = 350;
	scroll_info.nMin = 30;
	scroll_info.nMax = (n_rows)*30;
	SetScrollInfo(history_window, SB_VERT,&scroll_info, TRUE);
	
}