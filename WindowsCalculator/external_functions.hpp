#pragma once
#include "stdafx.h"

template <typename T> T Evaluate(char *expression_input, char* variable_values_input);
void SaveSSCFile(WCHAR file_type, std::wstring& T1, std::wstring& T2, std::wstring& T3);
WCHAR OpenSSCFile(std::wstring& T1, std::wstring& T2, std::wstring& T3);

