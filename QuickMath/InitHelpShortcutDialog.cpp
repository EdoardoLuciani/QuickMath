#include "stdafx.h"
#include "QuickMath.h"
#include "InitHelpShortcutDialog.h"

TCHAR lpszLatin[] = L"Lorem ipsum dolor sit amet, consectetur "
L"adipisicing elit, sed do eiusmod tempor "
L"incididunt ut labore et dolore magna "
L"aliqua. Ut enim ad minim veniam, quis "
L"nostrud exercitation ullamco laboris nisi "
L"ut aliquip ex ea commodo consequat. Duis "
L"aute irure dolor in reprehenderit in "
L"voluptate velit esse cillum dolore eu "
L"fugiat nulla pariatur. Excepteur sint "
L"occaecat cupidatat non proident, sunt "
L"in culpa qui officia deserunt mollit "
L"anim id est laborum.greuihgeihughiugeriuhgeriuhgehuigeuihegrhuiegrhiugeregiuhrgeiuhregruihegriuhegruhigeruihgeiurhieughrsaufokhguoishgiuerhgiueruighergiuehgeisopfhjsdpfgiodasfghiujpdf"
L"fwehjfewfwehjfwehjfwehkjfweeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeioufeuiwefiuphfweuihpfewhupifewphufiweqwfehupiwfehupiwefhuipfewuhefwuhpfewuhpfweuhpiwefhuopfewhupefwuhpewfuhpiwefhupwefi"
L"fhfwhjfwfwhjeefwhjefwhjfewhljwefhjlfewhjefwewhffhewjfwehjfhlfhlsdhvhvcxbvcxbndhjfdhjfsopiuewiuorweopirusdpoifjvposidfjrewpoigfuerpioguerpioguergpoierugpioegueropigudspokjvdckghjefdpg";


LRESULT CALLBACK subDialogProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg) {
	case WM_INITDIALOG: {
		HWND hwndDlgTextBox = CreateWindowW(
			L"EDIT",  // Predefined class; Unicode assumed 
			NULL,      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,  // Styles 
			5,         // x position 
			5,         // y position 
			455,        // Button width
			220,        // Button height
			hwndDlg,     // Parent window
			(HMENU)ID_DIALOG_EDIT1,       // No menu.
			GetModuleHandle(NULL),
			NULL);      // Pointer not needed.
		SendMessage(hwndDlgTextBox, WM_SETTEXT, NULL, (LPARAM)lpszLatin);

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
	CreateDialog(NULL, MAKEINTRESOURCE(IDD_SHORTCUTHELPDIALOG), parent_hWnd, subDialogProc);
}