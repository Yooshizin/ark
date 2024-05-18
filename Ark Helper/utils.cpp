#include <iostream>
#include <fstream>
#include <stdexcept>

#include "string_processing.h"
#include "utils.h"

namespace utils {
	using namespace std::literals;

	void CheckFileSettings(const std::filesystem::path& path) {
		if (std::filesystem::exists(path)) {
			return;
		}
		std::filesystem::create_directories(path.parent_path());

		std::ofstream out(path, std::ios::out);
		if (!out.is_open()) {
			throw std::invalid_argument("The settings file was not created or opened: "s + path.string());
		}

		{
			out << "webhook"s << std::endl;
			out << "https://discord.com/api/webhooks/exemple/exemple"s << std::endl << std::endl;
		}

		{
			out << "ServerSpam"s << std::endl;

			out << "pess_to_start: 0, 0"s << std::endl;
			out << "search_server: 0, 0"s << std::endl;
			out << "refresh_server: 0, 0"s << std::endl;
			out << "join_to_server: 0, 0"s << std::endl;
			out << "join_to_server_mode: 0, 0"s << std::endl;
			out << "first_server: 0, 0"s << std::endl;
			out << "create_people: 0, 0"s << std::endl;
			out << "back_if_server_not_found: 0, 0"s << std::endl;
			out << "back_if_server_not_found2: 0, 0"s << std::endl;
			out << "exit_to_main_menu: 0, 0"s << std::endl << std::endl;
		}

		{
			out << "DropMode"s << std::endl;

			out << "search_window: 0, 0"s << std::endl;
			out << "first_tp_name_in_list: 0, 0"s << std::endl;
			out << "teleporting: 0, 0"s << std::endl;
			out << "take_everything: 0, 0"s << std::endl;
			out << "give_everything: 0, 0"s << std::endl;
			out << "close_inventory: 0, 0"s << std::endl;
			out << "names" << std::endl;
			out << "name of teleports: DROP1, DROP2, DROP3, DROPN"s << std::endl;
			out << "name of teleport for saving loot: SAVELOOT"s << std::endl << std::endl;
		}

		{
			out << "FarmMode"s << std::endl;

			out << "drop_everything: 0, 0"s << std::endl;
			out << "search_window: 0, 0"s << std::endl << std::endl;
		}

		out.close();
	}

	void EditWebhook(const std::filesystem::path& path) {
		std::string webhook;
		std::cout << "Enter webhook: "s;
		std::getline(std::cin, webhook);

		std::ifstream input(path);
		if (!input.is_open()) {
			throw std::invalid_argument("The file wasn't opened: "s + path.string());
		}

		std::vector<std::string> lines;
		std::string line;

		while (std::getline(input, line)) {
			lines.push_back(std::move(line));
		}
		input.close();
		lines[1] = webhook;

		std::ofstream out(path, std::ios::out);
		if (!out.is_open()) {
			throw std::invalid_argument("The file wasn't opened: "s + path.string());
		}

		for (const auto& a : lines) {
			out << a << std::endl;
		}

		out.close();
		std::cout << "OK"s << std::endl;
	}

	void EnableDisableFunc(Command com) {
		if (is_enable == false && com != Command::NONE) {
			mode_selection.first = com;
			is_enable = true;
		}
		else {
			mode_selection.first = Command::NONE;
			is_enable = false;
			Beep(6000, 200);
			Beep(6000, 200);
		}
	}

	void GetCursorPosition(bool& a_waiting_key_press, int& x, int& y) noexcept {

		POINT* pCursorPos = new POINT;
		a_waiting_key_press = true;

		system("cls");
		std::cout << "F1 - Get cursor position\nF2 - Disable\n"s;
		while (a_waiting_key_press) {
			if (GetAsyncKeyState(VK_F1)) {
				Beep(200, 200);
				if (GetCursorPos(pCursorPos)) {
					x = pCursorPos->x;
					y = pCursorPos->y;
					std::cout << "X: "s << x << ", Y: "s << y << std::endl;
				}
				else {
					std::cout << "Failed to get cursor position"s << std::endl;
				}
				Sleep(50);
			}
			if (GetAsyncKeyState(VK_F2)) {
				a_waiting_key_press = false;
				Beep(200, 250);
				Sleep(300);
				Beep(200, 250);
				system("cls");
				processing::PrintMenuMessage1();
			}
			Sleep(20);
		}
	}

	void GetSetCursorPosition(int& x, int& y) {
		POINT* p_cursor_pos = new POINT;
		bool button_pressed = false;

		std::cout << "F1 - Get cursor position"s << std::endl;
		while (button_pressed == false) {
			if (GetAsyncKeyState(VK_F1)) {
				Beep(200, 200);

				if (GetCursorPos(p_cursor_pos)) {
					x = p_cursor_pos->x;
					y = p_cursor_pos->y;
					std::cout << "A success."s << std::endl;
				}
				else {
					std::cout << "A position of the cursor wasn't gotten."s << std::endl;
				}

				button_pressed = true;
				Sleep(300);
			}
		}

		delete p_cursor_pos;
	}

	void Observe() {
		while (true) {

			switch (page_menu) {
			case Page::FIRST:
				if (mode_selection.first == Command::NONE && mode_selection.second == Command::NONE && page_menu == Page::FIRST) {
					ObserveSelectingModeFromTheFirstPage();
				}
				if (mode_selection.first == Command::LOG_MODE && mode_selection.second == Command::SELECT_SETTINGS) {
					ObserveLogModeSelectSettings();
				}
				if (mode_selection.first == Command::LOG_MODE && mode_selection.second == Command::START) {
					ObserveLogModeStart();
				}
				if (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::SELECT_SETTINGS) {
					ObserveFarmModeSelectSettings();
				}
				if (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::START) {
					ObserveFarmModeStart();
				}
				if (mode_selection.first == Command::SPAM_MODE && mode_selection.second == Command::SELECT_SETTINGS) {
					ObserveSpamModeSelectSettings();
				}
				if (mode_selection.first == Command::SPAM_MODE && mode_selection.second == Command::START) {
					ObserveSpamModeStart();
				}

				Sleep(100);
				break;

			case Page::SECOND:
				if (page_menu == Page::SECOND && mode_selection.first == Command::NONE && mode_selection.second == Command::NONE) {
					ObserveSelectingModeFromTheSecondPage();
				}

				break;
			}
		}
	}

	void ObserveDropModeSelectSettings() {
		while (mode_selection.first == Command::DROP_MODE && mode_selection.second == Command::SELECT_SETTINGS) {
			if (GetAsyncKeyState(VK_F1)) {
				mode_selection.second = Command::START;
			}
			if (GetAsyncKeyState(VK_F2)) {
				drop_mode_command = DropModeEnum::EDIT_COORDS;
			}
			if (GetAsyncKeyState(VK_F3)) {
				drop_mode_command = DropModeEnum::EDIT_TP_NAME;
			}
			Sleep(100);
		}
	}

	void ObserveEditDropModeSettings() {
		while (mode_selection.first == Command::EDIT_DROP_MODE) {
			if (GetAsyncKeyState(VK_F1)) {
				mode_selection.second = Command::EDIT_DROP_COORD;
				Sleep(200);
			}
			if (GetAsyncKeyState(VK_F2)) {
				mode_selection.second = Command::EDIT_DROP_NAME;
				Sleep(200);
			}
			Sleep(100);
		}
	}

	void ObserveFarmModeSelectSettings() {
		while (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::SELECT_SETTINGS) {
			if (GetAsyncKeyState(VK_F1)) {
				mode_selection.second = Command::START;
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F2)) {
				resources = Resources::FLINT;
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F3)) {
				resources = Resources::STONE;
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F4)) {
				resources = Resources::WOOD;
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F5)) {
				resources = Resources::BERRY;
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F6)) {
				resources = Resources::THATCH;
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F7)) {
				resources = Resources::SAND;
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F8)) {
				resources = Resources::METAL;
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F9)) {
				resources = Resources::DELAY;
				Sleep(500);
			}
			Sleep(100);
		}
	}

	void ObserveFarmModeStart() {
		while (mode_selection.first == Command::FARM_MODE && mode_selection.second == Command::START) {
			if (GetAsyncKeyState(VK_F2)) {
				mode_selection.second = Command::NONE;
				EnableDisableFunc(Command::NONE);
			}
			Sleep(100);
		}
	}

	void ObserveLogModeSelectSettings() {
		while (mode_selection.first == Command::LOG_MODE && mode_selection.second == Command::SELECT_SETTINGS) {

			if (GetAsyncKeyState(VK_F1)) {
				Beep(3000, 400);
				screen_resolution = ScreenResolution::S_1920x1080;
			}
			if (GetAsyncKeyState(VK_F2)) {
				Beep(3000, 400);
				screen_resolution = ScreenResolution::S_2560x1440;
			}
			if (GetAsyncKeyState(VK_F3)) {
				Beep(3000, 400);
				screen_resolution = ScreenResolution::S_2048x1080;
			}
			if (GetAsyncKeyState(VK_F4)) {
				Beep(3000, 400);
				screen_resolution = ScreenResolution::S_3840x2160;
			}
			if (GetAsyncKeyState(VK_F5)) {
				Beep(3000, 400);
				screen_resolution = ScreenResolution::FULL_SCREEN;
			}
			Sleep(100);
		}
	}

	void ObserveLogModeStart() {
		while (mode_selection.first == Command::LOG_MODE && mode_selection.second == Command::START) {
			if (GetAsyncKeyState(VK_F2)) {
				mode_selection.second = Command::NONE;
				screen_resolution = ScreenResolution::NONE;
				EnableDisableFunc(Command::NONE);
			}
			Sleep(100);
		}
	}

	void ObserveSelectingModeFromTheFirstPage() {

		while (mode_selection.first == Command::NONE && mode_selection.second == Command::NONE && page_menu == Page::FIRST) {
			if (!GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_F1)) {
				Beep(500, 400);
				EnableDisableFunc(Command::LEFT_CLICK);
				while (mode_selection.first == Command::LEFT_CLICK) {
					if (GetAsyncKeyState(VK_F1)) {
						EnableDisableFunc(Command::NONE);
					}
					Sleep(100);
				}
				Sleep(500);
			}
			if (!GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_F2)) {
				Beep(1000, 400);
				EnableDisableFunc(Command::RIGHT_CLICK);
				while (mode_selection.first == Command::RIGHT_CLICK) {
					if (GetAsyncKeyState(VK_F2)) {
						EnableDisableFunc(Command::NONE);
					}
					Sleep(100);
				}
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F3)) {
				Beep(1500, 400);
				EnableDisableFunc(Command::SPACE_CLICK);
				while (mode_selection.first == Command::SPACE_CLICK) {
					if (GetAsyncKeyState(VK_F3)) {
						EnableDisableFunc(Command::NONE);
					}
					Sleep(100);
				}
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F4)) {
				Beep(2000, 400);
				EnableDisableFunc(Command::OPEN_GIT);
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F5)) {
				Beep(2500, 400);
				EnableDisableFunc(Command::OPEN_YT);
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F6)) {
				Beep(3000, 400);
				EnableDisableFunc(Command::LOG_MODE);
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F7)) {
				Beep(3500, 400);
				EnableDisableFunc(Command::FARM_MODE);
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F8)) {
				Beep(4000, 400);
				EnableDisableFunc(Command::DELAY);
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_F9)) {
				Beep(4500, 400);
				EnableDisableFunc(Command::SPAM_MODE);
				Sleep(500);
			}
			if (GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_F2)) {
				page_menu = Page::SECOND;
				Sleep(500);
			}
			Sleep(100);
		}
	}

	void ObserveSelectingModeFromTheSecondPage() {
		while (page_menu == Page::SECOND && mode_selection.first == Command::NONE && mode_selection.second == Command::NONE) {

			if (GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_F1)) {
				page_menu = Page::FIRST;
				Sleep(500);
			}
			if (!GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_F1)) {
				EnableDisableFunc(Command::DROP_MODE);
				mode_selection.second = Command::SELECT_SETTINGS;
				ObserveDropModeSelectSettings();
			}
			if (!GetAsyncKeyState(VK_RBUTTON) && GetAsyncKeyState(VK_F2)) {
				EnableDisableFunc(Command::EDIT_SPAM_MOD);
			}
			if (GetAsyncKeyState(VK_F3)) {
				EnableDisableFunc(Command::EDIT_DROP_MODE);
				ObserveEditDropModeSettings();
			}
			if (GetAsyncKeyState(VK_F4)) {
				EnableDisableFunc(Command::EDIT_WEBHOOK);
				while (mode_selection.first == Command::EDIT_WEBHOOK) {
					// sleep (stub)
				}
			}
			if (GetAsyncKeyState(VK_F5)) {
				EnableDisableFunc(Command::EDIT_FARM_COORD);
				while (mode_selection.first == Command::EDIT_FARM_COORD) {
					// sleep (stub)
				}
			}
			Sleep(100);
		}
	}

	void ObserveSpamModeSelectSettings() {
		while (mode_selection.first == Command::SPAM_MODE && mode_selection.second == Command::SELECT_SETTINGS) {
			if (GetAsyncKeyState(VK_F1)) {
				mode_selection.second = Command::START;
			}
			if (GetAsyncKeyState(VK_F2)) {
				spam_command = SpamCommand::EDIT;
			}
			Sleep(100);
		}
	}

	void ObserveSpamModeStart() {
		while (mode_selection.first == Command::SPAM_MODE && mode_selection.second == Command::START) {
			if (GetAsyncKeyState(VK_F1)) {
				spam_command = SpamCommand::START;
			}

			if (GetAsyncKeyState(VK_F2)) {
				mode_selection.second = Command::NONE;
				spam_command = SpamCommand::NONE;
				EnableDisableFunc(Command::NONE);
			}
		}
	}

	void OpenGitHub() noexcept {
		LPCWSTR url = L"https://github.com/prizrak56";
		HINSTANCE result = ShellExecute(NULL, L"open", url, NULL, NULL, SW_SHOWNORMAL);
	}

	void OpenYouTube() noexcept {
		LPCWSTR url = L"https://www.youtube.com/channel/UCa4dY-BwibiLpV1JsdHfXDw";
		HINSTANCE result = ShellExecute(NULL, L"open", url, NULL, NULL, SW_SHOWNORMAL);
	}

	std::vector<std::string> ReadSettings(const std::filesystem::path& path_settings) {
		std::ifstream input(path_settings, std::ios::in);

		if (!input.is_open()) {
			throw std::invalid_argument("File wasn't opened: "s + path_settings.string());
		}

		std::vector<std::string> settings;
		std::string line;

		while (std::getline(input, line)) {
			settings.push_back(std::move(line));
		}
		input.close();

		return settings;
	}

	void SimulateKeyPress(WORD keyCode) {
		INPUT input{};

		input.type = INPUT_KEYBOARD;
		input.ki.wScan = 0;
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		input.ki.wVk = keyCode;
		input.ki.dwFlags = 0;

		SendInput(1, &input, sizeof(INPUT));
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}
} // namespace utils