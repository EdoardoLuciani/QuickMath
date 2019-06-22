#include "external_functions.hpp"
#include "stdafx.h"


std::wstring ssc_file_header_normal_calculation_1 = L".ssc-?-1.0";
std::wstring separator_1 = { 0x00,0x00 };
std::wstring end_file_1 = L".cend";

void SaveSSCFile(WCHAR file_type, std::wstring& T1, std::wstring& T2, std::wstring& T3) {
	std::wstring header = ssc_file_header_normal_calculation_1;
	header[5] = file_type;

	OPENFILENAME ofn;
	WCHAR path[MAX_PATH] = L".ssc";
	HWND hwnd = NULL;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = L"Saved Calculations (*.ssc)\0*.ssc\0";
	ofn.lpstrFile = path;
	ofn.nMaxFile = MAX_PATH;
	ofn.nFilterIndex = 1;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"C:";
	ofn.lpstrDefExt = L"ssc";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	GetSaveFileName(&ofn);

	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf16<wchar_t>());

	std::wofstream ofile(path, std::ios::out);
	ofile.imbue(utf8_locale);

	if (file_type == L'n') {
		ofile.write(header.c_str(), header.size());

		ofile.write(separator_1.c_str(), separator_1.size());
		ofile.write(L"T1:", 3);
		ofile.write(T1.c_str(), T1.size());
		ofile.write(separator_1.c_str(), separator_1.size());
		ofile.write(L"T3:", 3);
		ofile.write(T3.c_str(), T3.size());
		ofile.write(separator_1.c_str(), separator_1.size());
		ofile.write(end_file_1.c_str(), end_file_1.size());
	}
	ofile.close();
}


WCHAR OpenSSCFile(std::wstring& T1, std::wstring& T2, std::wstring& T3) {

	std::wstring header = ssc_file_header_normal_calculation_1;
	OPENFILENAME ofn;
	WCHAR path[MAX_PATH] = L".ssc";
	HWND hwnd = NULL;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = L"Saved Calculations (*.ssc)\0*.ssc\0";
	ofn.lpstrFile = path;
	ofn.nMaxFile = MAX_PATH;
	ofn.nFilterIndex = 1;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"C:";
	ofn.lpstrDefExt = L"ssc";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	GetOpenFileName(&ofn);

	WCHAR file_type;

	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf16<wchar_t>());

	std::wifstream ofile(path, std::ios::in);
	ofile.imbue(utf8_locale);

	ofile.seekg(10);
	ofile.get(file_type);
	ofile.seekg(std::ios_base::beg);

	header[5] = file_type;

	if (file_type == L'n') {

		WCHAR *file_header = new WCHAR[32767];
		SecureZeroMemory(file_header, 32767 * 2);
		ofile.read(file_header, 10);

		if (memcmp(header.c_str(), file_header, 10) != 0) {
			//File is not recognized
			return -1;
		}
		SecureZeroMemory(file_header, 10);
		ofile.read(file_header, 2);

		if (memcmp(separator_1.c_str(), file_header, 2) != 0) {
			//File is not recognized
			return -1;
		}
		SecureZeroMemory(file_header, 2);
		ofile.read(file_header, 3);

		if (memcmp(L"T1:", file_header, 6) != 0) {
			return -1;
		}
		SecureZeroMemory(file_header, 3);

		ofile.getline(file_header, 32767, 0x0000);
		T1.assign(file_header);
		ofile.seekg((int)ofile.tellg() - 2);

		SecureZeroMemory(file_header, 32767 * 2);

		ofile.read(file_header, 2);
		if (memcmp(separator_1.c_str(), file_header, 2) != 0) {
			//File is not recognized
			return -1;
		}
		SecureZeroMemory(file_header, 2);

		ofile.read(file_header, 3);

		if (memcmp(L"T3:", file_header, 6) != 0) {
			return -1;
		}
		SecureZeroMemory(file_header, 3);

		ofile.getline(file_header, 32767, 0x00);
		T3.assign(file_header);

		delete[] file_header;

		ofile.close();
	}
	return file_type;
}

