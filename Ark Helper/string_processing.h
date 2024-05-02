#pragma once
#include <vector>
#include <string>
#include <Windows.h>

bool SplitIntoWordsAndFindWord(const std::string& text) noexcept;

void MenuMessage() noexcept;

std::string ConvertLPCWSTRToString(LPCWSTR lpcwszStr) noexcept;
