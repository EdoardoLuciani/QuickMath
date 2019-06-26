#include "stdafx.h"
#include "QuickMath.h"
#include "InitHelpShortcutDialog.h"

HINSTANCE hInstDlgLoc;

LRESULT CALLBACK subDialogProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg) {
	case WM_INITDIALOG: {
		HWND hwndDlgTextBox = CreateWindowW(
			L"EDIT",  // Predefined class; Unicode assumed 
			NULL,      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_READONLY | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,  // Styles 
			10,         // x position 
			10,         // y position 
			450,        // Button width
			215,        // Button height
			hwndDlg,     // Parent window
			(HMENU)ID_DIALOG_EDIT1,       // No menu.
			hInstDlgLoc,
			NULL);      // Pointer not needed.

		HFONT hFontDlgtextBox = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, TEXT("Segoe UI"));

		SendMessage(hwndDlgTextBox, WM_SETFONT, (WPARAM)hFontDlgtextBox, TRUE);

		WCHAR *shortcuts_explanation = new WCHAR[100];
		LoadStringW(hInstDlgLoc, IDS_SHORTCUTLIST, shortcuts_explanation, 100);
		SendMessage(hwndDlgTextBox, WM_SETTEXT, NULL, (LPARAM)shortcuts_explanation);
		delete[] shortcuts_explanation;
		return TRUE;
	} break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hwndDlg, wParam);
			return TRUE;
		} break;
	case WM_CLOSE:
		EndDialog(hwndDlg, wParam);
		return TRUE;
	}
	return 0;
}

void InitHelpShortcutDialog(HWND parent_hWnd, HINSTANCE hInstance) {
	hInstDlgLoc = hInstance;
	CreateDialog(NULL, MAKEINTRESOURCE(IDD_SHORTCUTHELPDIALOG), parent_hWnd, subDialogProc);
}