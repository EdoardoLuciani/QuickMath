#include "stdafx.h"
#include "QuickMath.h"
#include "InitHelpShortcutDialog.h"

HWND hwndDlgTextBox;

LRESULT CALLBACK subDialogProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg) {
	case WM_INITDIALOG: {
		hwndDlgTextBox = CreateWindowW(
			L"EDIT",  // Predefined class; Unicode assumed 
			NULL,      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_LOWERCASE | ES_READONLY,  // Styles 
			50,         // x position 
			50,         // y position 
			300,        // Button width
			50,        // Button height
			hwndDlg,     // Parent window
			(HMENU) ID_DIALOG_EDIT1,       // No menu.
			GetModuleHandle(NULL),
			NULL);      // Pointer not needed.

		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hwndDlg, wParam);
			return TRUE;
		}
	case WM_CLOSE:
		EndDialog(hwndDlg, wParam);
		return TRUE;
	}
	return FALSE;
}

void InitHelpShortcutDialog(HWND parent_hWnd, HINSTANCE hInstance) {
	CreateDialog(NULL, MAKEINTRESOURCE(IDD_SHORTCUTHELPDIALOG), parent_hWnd, subDialogProc);
}