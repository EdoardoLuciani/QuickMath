// WindowsCalculator.cpp : Defines the entry point for the application.

//todo: add undo function
//todo: add history
//todo: add graphical mode

#include "stdafx.h"
#include "QuickMath.h"
#include "external_variables.h"
#include "external_functions.hpp"
#include "InitHelpShortcutDialog.h"
#include "InitFileHistoryWindow.h"

constexpr int BUTTON_WIDTH = 80;
constexpr int BUTTON_HEIGHT = 49;
constexpr int OBJ_STARTING_X = 10;
constexpr int BTT_STARTING_Y = 500;
constexpr int BTT_SPACING_Y = 10;
constexpr int N_BTT = 36;

WCHAR button_chars[N_BTT][6] = { L"0",L".",L"*10^", L"PLOT",L"EXE",
							 L"1",L"2",L"3", L"+",L"Ans",
							 L"4",L"5",L"6", L"-",L"pi",
							 L"7",L"8",L"9", L"*",L"e",
							 L"^",L"log10",L"logn",L"/",L"x",
							 L"sqrt",L"cos",L"sin",L"tan",L"=",
							 L"UNDO",L"(",L")",L"RESET",L"DEL" };

HWND hwndButton[N_BTT] = { NULL };
HWND hwndTextBox = NULL;
HWND hwndTextBox2 = NULL;
HWND hwndTextBox3 = NULL;

WNDPROC EditProc;
WNDPROC EditProc3;

HWND last_focus;

std::array<std::vector<std::wstring>,3> operations_history;

BOOL                InitInstance();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	hInst = hInstance;
    // Perform application initialization:
    if (!InitInstance() ) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_QUICKMATH));

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        if (!TranslateAccelerator(hWnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK subEditProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	switch (msg) {
	case WM_CHAR:
		switch (wParam) {
		case VK_TAB:
			return 0;
		case VK_RETURN:
			return 0;
		}
	case WM_KILLFOCUS:
		last_focus = hwndTextBox;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_TAB:
			SetFocus(hwndTextBox3);
			return 0;
		case VK_RETURN:
			SendMessage(hWnd, WM_COMMAND, ID_BUTTON5, NULL);
			return 0;
		}
	default:
		return CallWindowProc(EditProc, wnd, msg, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK subEditProc3(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
		switch (wParam) {
		case VK_TAB:
			return 0;
		case VK_RETURN:
			return 0;
		}
	case WM_KILLFOCUS:
		last_focus = hwndTextBox3;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_TAB:
			SetFocus(hwndTextBox);
			return 0;
		case VK_RETURN:
			SendMessage(hWnd, WM_COMMAND, ID_BUTTON5, NULL);
			return 0;
		}
	default:
		return CallWindowProc(EditProc3, wnd, msg, wParam, lParam);
	}
	return 0;
}

BOOL InitInstance() {

   HFONT hFontEdit = CreateFont(25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
	   OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
	   DEFAULT_PITCH | FF_DONTCARE, TEXT("Consolas"));

   HFONT hFontEdit2 = CreateFont(22, 0, 0, 0, 300, FALSE, FALSE, FALSE, ANSI_CHARSET,
	   OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
	   DEFAULT_PITCH | FF_DONTCARE, TEXT("Consolas"));

   HFONT hFontButton = CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
	   OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
	   DEFAULT_PITCH | FF_DONTCARE, TEXT("Segoe UI"));

   WCHAR szTitle[100];              
   WCHAR szWindowClass[100];
   LoadStringW(hInst, IDS_APP_TITLE, szTitle, 100);
   LoadStringW(hInst, IDC_QUICKMATH, szWindowClass, 100);

   WNDCLASSEXW wcex;
   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.style = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc = WndProc;
   wcex.cbClsExtra = 0;
   wcex.cbWndExtra = 0;
   wcex.hInstance = hInst;
   wcex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_QUICKMATH));
   wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
   wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_QUICKMATH);
   wcex.lpszClassName = szWindowClass;
   wcex.hIconSm = LoadIcon(hInst, MAKEINTRESOURCE(IDI_QUICKMATH));
   //wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(37, 37, 38)));

   RegisterClassExW(&wcex);

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
      CW_USEDEFAULT, 0, 475, 618, nullptr, nullptr, hInst, nullptr);

   if (!hWnd) {
      return FALSE;
   }

	hwndTextBox = CreateWindowW(
	   L"EDIT",  // Predefined class; Unicode assumed 
	   NULL,      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_LOWERCASE,  // Styles 
	   OBJ_STARTING_X,         // x position 
	   10,         // y position 
	   440,        // Button width
	   35,        // Button height
	   hWnd,     // Parent window
	   (HMENU) ID_EDIT1,       // No menu.
		hInst,
	   NULL);      // Pointer not needed.

	EditProc = (WNDPROC)SetWindowLongPtr(hwndTextBox, GWLP_WNDPROC, (LONG_PTR)subEditProc);

	SetFocus(hwndTextBox);

	hwndTextBox2 = CreateWindowW(
	   L"EDIT",  // Predefined class; Unicode assumed 
	   NULL,      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_LOWERCASE | ES_RIGHT | ES_READONLY,  // Styles 
	   OBJ_STARTING_X + 440/2,         // x position 
	   45,         // y position 
	   440/2,        // Button width
	   35,        // Button height
	   hWnd,     // Parent window
	   (HMENU)ID_EDIT2,       // No menu.
		hInst,
	   NULL);      // Pointer not needed.

	hwndTextBox3 = CreateWindowW(
		L"EDIT",  // Predefined class; Unicode assumed 
		NULL,      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_LOWERCASE,  // Styles 
		OBJ_STARTING_X,         // x position 
		80,         // y position 
		440/2,        // Button width
		35,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_EDIT3,       // No menu.
		hInst,
		NULL);      // Pointer not needed.

	EditProc3 = (WNDPROC)SetWindowLongPtr(hwndTextBox3, GWLP_WNDPROC, (LONG_PTR)subEditProc3);


   for (int i = 0, index=0; i < 7; i++) {

	   for (int j = 0; j < 5; j++,index++) {
		   hwndButton[index] = CreateWindowW (
			   L"BUTTON",  // Predefined class; Unicode assumed 
			   button_chars[index],      // Button text 
			   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON ,  // Styles 
			   OBJ_STARTING_X*(j+1) + BUTTON_WIDTH * j,         // x position 
			   BTT_STARTING_Y - BTT_SPACING_Y*i - BUTTON_HEIGHT * i,         // y position 
			   BUTTON_WIDTH,        // Button width
			   BUTTON_HEIGHT,        // Button height
			   hWnd,     // Parent window
			   (HMENU) (ID_BUTTON1 + index),       // No menu.
			   hInst,
			   NULL);      // Pointer not needed.

		   SendMessage(hwndButton[index], WM_SETFONT, (WPARAM)hFontButton, TRUE);
	   }
	}

   SendMessage(hwndTextBox, WM_SETFONT, (WPARAM)hFontEdit, TRUE);
   SendMessage(hwndTextBox2, WM_SETFONT, (WPARAM)hFontEdit2, TRUE);
   SendMessage(hwndTextBox3, WM_SETFONT, (WPARAM)hFontEdit, TRUE);

   ShowWindow(hWnd, SW_SHOW);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	if (message == WM_COMMAND) {

		int wmId = LOWORD(wParam);

		if (wmId == ID_BUTTON4) {
			//TODO: implement graph here
		}
		else if (wmId == ID_BUTTON5) {
			int n_chars = SendMessage(hwndTextBox, WM_GETTEXTLENGTH, 0, 0) + 1;

			if (!n_chars) {
				return DefWindowProc(hWnd, message, wParam, lParam);
			}

			mpfr::mpreal result;

			WCHAR* U_string = new WCHAR[n_chars];
			char* A_string = new char[n_chars];

			SendMessage(hwndTextBox, WM_GETTEXT, n_chars, (LPARAM)U_string);
			operations_history[0].push_back(U_string);
			wcstombs(A_string, U_string, n_chars);
			delete[] U_string;

			int n_chars_2 = SendMessage(hwndTextBox3, WM_GETTEXTLENGTH, 0, 0) + 1;

			if (n_chars_2) {

				WCHAR* U_string_2 = new WCHAR[n_chars_2];
				char* A_string_2 = new char[n_chars_2];

				SendMessage(hwndTextBox3, WM_GETTEXT, n_chars_2, (LPARAM)U_string_2);
				operations_history[1].push_back(U_string_2);
				wcstombs(A_string_2, U_string_2, n_chars_2);
				delete[] U_string_2;

				result = Evaluate<mpfr::mpreal>(A_string, A_string_2);
				delete[] A_string_2;
			}
			else {
				result = Evaluate<mpfr::mpreal>(A_string, NULL);
				operations_history[1].push_back(0);
			}

			delete[] A_string;
			int result_string_size = strlen(result.toString().c_str()) + 1;
			WCHAR* final_string = new WCHAR[result_string_size]();
			MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, result.toString().c_str(), -1, final_string, result_string_size);
			operations_history[2].push_back(final_string);

			if (wcscmp(final_string, L"nan")) {
				int str1_l = SendMessage(hwndTextBox3, WM_GETTEXTLENGTH, 0, 0) + 1;
				std::wstring last_value_string;

				if (str1_l - 1) {
					WCHAR* b_str1 = new WCHAR[str1_l];
					SendMessage(hwndTextBox3, WM_GETTEXT, str1_l, (LPARAM)b_str1);
					last_value_string.assign(b_str1);
					int pos_tr = last_value_string.find(L"ans=", 0);
					if (pos_tr != std::string::npos) {

						int pos_te = last_value_string.find(L',', pos_tr + 4);

						if (pos_te != std::string::npos) {
							last_value_string.erase(pos_tr + 4, pos_te - (pos_tr + 4));
						}
						else {
							last_value_string.erase(pos_tr + 4, std::string::npos);
						}

						last_value_string.insert(pos_tr + 4, final_string);
					}
					else {
						last_value_string.append(L",ans=");
						last_value_string.append(final_string);
					}
					delete[] b_str1;
				}
				else {
					last_value_string.assign(L"ans=");
					last_value_string.append(final_string);
				}
				SendMessage(hwndTextBox3, WM_SETTEXT, 0, (LPARAM)last_value_string.c_str());
			}

			SendMessage(hwndTextBox2, WM_SETTEXT, NULL, (LPARAM)final_string);
			delete[] final_string;

			SetFocus(last_focus);
		}
		else if (wmId == ID_BUTTON31) {
			SendMessage(hwndTextBox, EM_UNDO, NULL, NULL);
		}
		else if (wmId == ID_BUTTON34) {
			SendMessage(hwndTextBox, WM_SETTEXT, NULL, NULL);
			SendMessage(hwndTextBox2, WM_SETTEXT, NULL, NULL);
			SendMessage(hwndTextBox3, WM_SETTEXT, NULL, NULL);
			SetFocus(hwndTextBox);
		}
		else if (wmId == ID_BUTTON35) {
			int n_chars = SendMessage(last_focus, WM_GETTEXTLENGTH, 0, 0);
			SendMessage(last_focus, EM_SETSEL, (WPARAM)n_chars - 1, n_chars);
			SendMessage(last_focus, EM_REPLACESEL, NULL, (LPARAM)L"");
			SetFocus(last_focus);
		}
		else if ((wmId >= ID_BUTTON1) && (wmId <= ID_BUTTON33)) {
			int ndx = GetWindowTextLength(last_focus);
			SendMessage(last_focus, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
			SendMessage(last_focus, EM_REPLACESEL, 0, (LPARAM)button_chars[wmId - ID_BUTTON1]);
			SetFocus(last_focus);
		}
		else if (wmId == ID_FILE_EXPORTOPERATIONSTOFILE) {
			std::wstring str1, str2, str3;

			int str1_l = SendMessage(hwndTextBox, WM_GETTEXTLENGTH, 0, 0) + 1;
			int str2_l = SendMessage(hwndTextBox2, WM_GETTEXTLENGTH, 0, 0) + 1;
			int str3_l = SendMessage(hwndTextBox3, WM_GETTEXTLENGTH, 0, 0) + 1;

			WCHAR* b_str1 = new WCHAR[str1_l];
			WCHAR* b_str2 = new WCHAR[str2_l];
			WCHAR* b_str3 = new WCHAR[str3_l];

			SendMessage(hwndTextBox, WM_GETTEXT, str1_l, (LPARAM)b_str1);
			SendMessage(hwndTextBox2, WM_GETTEXT, str2_l, (LPARAM)b_str2);
			SendMessage(hwndTextBox3, WM_GETTEXT, str3_l, (LPARAM)b_str3);

			str1.assign(b_str1);
			str2.assign(b_str2);
			str3.assign(b_str3);

			delete[] b_str1;
			delete[] b_str2;
			delete[] b_str3;

			SaveSSCFile(L'n', str1, str2, str3);

			SetFocus(last_focus);
		}
		else if (wmId == ID_FILE_IMPORTOPERATIONSFROMFILE) {
			std::wstring str1, str2, str3;

			OpenSSCFile(str1, str2, str3);

			SendMessage(hwndTextBox, WM_SETTEXT, NULL, (LPARAM)str1.c_str());
			SendMessage(hwndTextBox2, WM_SETTEXT, NULL, (LPARAM)str2.c_str());
			SendMessage(hwndTextBox3, WM_SETTEXT, NULL, (LPARAM)str3.c_str());

			SetFocus(last_focus);
		}
		else if (wmId == IDM_EXIT) {
			DestroyWindow(hWnd);
		}
		else if (wmId == ID_HELP_SHORT) {
			InitHelpShortcutDialog();
		}
		else if (wmId == ID_HELP_GOTOQUICKMATH) {
			ShellExecute(0, 0, L"https://github.com/EdoardoLuciani/QuickMath", 0, 0, SW_SHOW);
		}
		else if (wmId == ID_FILE_HISTORY) {
			InitFileHistoryWindow(operations_history);
		}
		else {
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	else if (message == WM_RESTORE_CALCULATION) {
		SendMessage(hwndTextBox, WM_SETTEXT, NULL, (LPARAM)operations_history[0][(int)wParam].c_str());
		SendMessage(hwndTextBox2, WM_SETTEXT, NULL, (LPARAM)operations_history[2][(int)wParam].c_str());
		SendMessage(hwndTextBox3, WM_SETTEXT, NULL, (LPARAM)operations_history[1][(int)wParam].c_str());
	}
	else if (message == WM_SYSCOMMAND) {
		if (wParam == SC_RESTORE) {
			ShowWindow(hWnd, SW_RESTORE);
			SetFocus(last_focus);
		}
		else {
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	else if (message == WM_ACTIVATE) {
		SetFocus(last_focus);
	}    
	else if (message == WM_DESTROY) {
        PostQuitMessage(0);
	}
	else {
        return DefWindowProc(hWnd, message, wParam, lParam);
	}
    return 0;
}


template <typename T> T Evaluate(char* expression_input, char* variable_values_input) {

	typedef exprtk::symbol_table<T> symbol_table_t;
	typedef exprtk::expression<T>     expression_t;
	typedef exprtk::parser<T>             parser_t;

	expression_t expression;
	parser_t parser;
	symbol_table_t symbol_table;
	symbol_table.add_constants();
	std::string expression_string = expression_input;

	std::vector <T> var_values;
	std::vector <std::string> var_names;

	if (variable_values_input != nullptr) {
		char* variable_values = new char[strlen(variable_values_input) + 1];
		strcpy(variable_values, variable_values_input);

		char* token;
		char* var_name;
		char* var_value;

		token = strtok(variable_values, "  ,");

		while (token != NULL) {
			char* token_copy = new char[strlen(token) + 1]();
			char* dummy = nullptr;
			strcpy(token_copy, token);
			var_name = strtok_s(token_copy, "=", &dummy);
			var_value = strtok_s(NULL, "=", &dummy);

			if (var_name && var_value) {
			var_values.push_back(T(atof(var_value)));
			var_names.push_back(var_name);
			}

			delete[] token_copy;
			token = strtok(NULL, " ,");
		}
		delete[] variable_values;

		int true_values_range = var_values.capacity();

		var_names.resize(var_names.capacity() + 1);
		var_values.resize(var_values.capacity() + 1);

		for (int i = 0; i < true_values_range; i++) {
			symbol_table.add_variable(var_names.at(i), var_values.at(i));
		}
	}

	expression.register_symbol_table(symbol_table);
	parser.compile(expression_string, expression);

	return expression.value();
}