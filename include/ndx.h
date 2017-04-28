#ifndef NDX_H
#define NDX_H
#include "ndxtools.h"

class NDX
{
public:
	NDX();
	~NDX();
	std::vector <Window> GetWindowsInRoot();
	std::string GetWindowTitle(Window _wid);
	std::string GetApplicationName(Window _wid);
	bool RaiseWindow(Window _wid);
	std::string GetWindowType(Window _wid);
	std::vector <std::string> GetWindowState(Window _wid);
	std::vector <std::string> GetWindowAllowedActions(Window _wid);
	Icon GetWindowIcon(Window _wid);
	std::string GetWindowIconName(Window _wid);
	std::vector <Atom> NetSupported(Window _wid);
	unsigned long GetWindowPid(Window _wid);
	unsigned long GetWindowDesktop(Window _wid);
private:
	NDXTools * NDX_Tools;
};

#endif // NDX_H
