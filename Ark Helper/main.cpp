#include <thread>
#include <iostream>

#include "utils.h"
#include "clicker.h"
#include "log_mode.h"
#include "string_processing.h"
#include "log_mode.h"
#include "farm_mode.h"
#include "server_spam.h"
#include "drop_mode.h"

using namespace std::literals;

int main() {

	SetConsoleTitle(L"prizrak_p");

	std::thread observer(ObServer);

	const LPCWSTR target_window_Name = L"ArkAscended";
	HWND h_window_handle = FindWindow(NULL, target_window_Name);

	LeftMouseClicker left_click(h_window_handle);

	RightMouseClicker right_click(h_window_handle);

	SpaceButtonClicker space_click(h_window_handle);

	FarmMode farm(h_window_handle);

	LogMode log;

	ServerSpam spam;

	DropMode looting_drop;

	size_t delay = 0;

	MenuMessage();

	while (true) {
		
		switch (mode_selection.first)
		{
		case Command::LEFT_CLICK:
			if (h_window_handle != NULL) {
				left_click.PressingKeyUntilHotKeyIsInputed(is_enable);
			}
			else { 
				EnableDisableFunc(Command::NONE);
				std::cout << std::endl << "Process not found ("s << ConvertLPCWSTRToString(target_window_Name) << ")"s << std::endl;
			}
			break;
		case Command::RIGHT_CLICK:
			if (h_window_handle != NULL) {
				right_click.PressingKeyUntilHotKeyIsInputed(is_enable);
			}
			else { 
				EnableDisableFunc(Command::NONE);
				std::cout << std::endl << "Process not found ("s << ConvertLPCWSTRToString(target_window_Name) << ")"s << std::endl;
			}
			break;
		case Command::SPACE_CLICK:
			if (h_window_handle != NULL) {
				space_click.PressingKeyUntilHotKeyIsInputed(is_enable);
			}
			else {
				EnableDisableFunc(Command::NONE);
				std::cout << std::endl << "Process not found ("s << ConvertLPCWSTRToString(target_window_Name) << ")"s << std::endl;
			}
			break;
		case Command::OPEN_GIT:

			OpenGitHub();

			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;

			break;

		case Command::OPEN_YT:

			OpenYouTube();

			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;

			break;

		case Command::LOG_MODE:

			log.ChooseOptions();

			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;
			break;

		case Command::FARM_MODE:

			farm.FarmStart();

			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;
			break;

		case Command::DELAY:

			system("CLS");

			std::cout << "default - 200 ms" << std::endl;
			std::cout << "Time delay milliseconds: " << std::endl;
			std::cin >> delay;
			std::cout << std::endl;

			Beep(1000, 200);
			MenuMessage();

			left_click.SetDelay(delay);
			right_click.SetDelay(delay);
			space_click.SetDelay(delay);

			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;
			break;

		case Command::SPAM_MODE:
			mode_selection.second = Command::SELECT_SETTINGS;
			spam.SpamStart();
			break;
		case Command::DROP_MODE:
			mode_selection.second = Command::SELECT_SETTINGS;
			looting_drop.SupplyCreate();
			mode_selection.first = Command::NONE;
			mode_selection.second = Command::NONE;
		}
		
		Sleep(20);
	}
}