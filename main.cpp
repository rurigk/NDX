#include <iostream>
#include "include/ndx.h"


int main()
{
	NDX * ndx = new NDX();
	std::vector <Window> windows;
	std::vector <std::string> states;
	std::vector <std::string> allowed;
	windows = ndx->GetWindowsInRoot();
	for(int i = 0; i < windows.size() ;i++){
		std::cout << ndx->GetWindowTitle((Window)windows[i])<<std::endl;
		std::cout << ndx->GetWindowPid((Window)windows[i])<<std::endl;
		std::cout << ndx->GetWindowDesktop((Window)windows[i])<<std::endl;
		std::cout << ndx->GetWindowType((Window)windows[i])<<std::endl;
		states = ndx->GetWindowState((Window)windows[i]);
		for (int i = 0; i < states.size(); i++) {
			std::cout << (std::string)states[i] << std::endl;
		}
		allowed = ndx->GetWindowAllowedActions((Window)windows[i]);
		for (int i = 0; i < allowed.size(); i++) {
			std::cout << (std::string)allowed[i] << std::endl;
		}
		Icon imgdata = ndx->GetWindowIcon((Window)windows[i]);
	}
	Icon imgdata = ndx->GetWindowIcon((Window)windows[windows.size()-1]);
	std::cout << "Width: " << imgdata.width << " Height: " << imgdata.height << " Data element size " << imgdata.data.size() << std::endl;
	return 0;
}

