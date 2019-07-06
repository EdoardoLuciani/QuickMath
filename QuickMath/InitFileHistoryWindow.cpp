#include "stdafx.h"
#include "InitFileHistoryWindow.h"

LRESULT CALLBACK InitFileHistoryWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_COMMAND: {

		} break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void InitFileHistoryWindow(HINSTANCE hInst, std::array<std::vector<std::wstring>, 3> &operations_history) {

	WCHAR szWindowClass[100];            // the main window class name

	wcscpy(szWindowClass, L"HISTORY");

	WNDCLASSEX windowclassforwindow;
	ZeroMemory(&windowclassforwindow, sizeof(WNDCLASSEX));
	windowclassforwindow.cbClsExtra = NULL;
	windowclassforwindow.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow.cbWndExtra = NULL;
	windowclassforwindow.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclassforwindow.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclassforwindow.hIcon = NULL;
	windowclassforwindow.hIconSm = NULL;
	windowclassforwindow.hInstance = hInst;
	windowclassforwindow.lpfnWndProc = (WNDPROC)InitFileHistoryWindowProc;
	windowclassforwindow.lpszClassName = szWindowClass;
	windowclassforwindow.lpszMenuName = NULL;
	windowclassforwindow.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassExW(&windowclassforwindow);
	
	HWND handleforwindow2 = CreateWindowEx(NULL,
		szWindowClass,
		L"Child Window",
		WS_OVERLAPPEDWINDOW,
		200,
		150,
		640,
		480,
		NULL,
		NULL,
		hInst,
		NULL);

	ShowWindow(handleforwindow2, SW_SHOW);

	int info = GetLastError();
	//printf("%d", info);
}

