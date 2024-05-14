#pragma once
#include <string>
#include <windows.h>
#include <filesystem>

std::string ReadImage(const std::filesystem::path& image_path) noexcept;

HBITMAP CaptureScreenshotForWebhook(int x, int y, int width, int height) noexcept;

void CaptureScreen(const char* savePath, int x, int y, int width, int height) noexcept;
