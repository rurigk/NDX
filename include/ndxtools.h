#ifndef NDXTOOLS_H
#define NDXTOOLS_H

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vector>
#include <iostream>
#include <time.h>
#include <climits>

struct Icon{
	int width;
	int height;
	std::vector <int> data;
};

class NDXTools
{
public:
	NDXTools();
	~NDXTools();
	Atom atom(const char* atomName);
	int clientMessage(Window _wid, Atom _msg,
						  long unsigned int data0,
						  long unsigned int data1 = 0,
						  long unsigned int data2 = 0,
						  long unsigned int data3 = 0,
	long unsigned int data4 = 0);
	void moveWindow(Window _win, int _x, int _y) ;
	int raiseWindow(Window _wid) ;
	void resizeWindow(Window _wid, int _width, int _height) ;
	void closeWindow(Window _wid) ;
	Window root; //the actual root window on the used screen
	Display *display; //the actual display

	/** \warning Do not forget to XFree(result) after data are processed!
	*/
	bool getWindowProperty(Window window,
						   Atom atom,               // property
						   Atom reqType,            // req_type
						   unsigned long* resultLen,// nitems_return
						   unsigned char** result   // prop_return
						  ) const;

	/** \warning Do not forget to XFree(result) after data are processed!
	*/
	bool getRootWindowProperty(Atom atom,               // property
							   Atom reqType,            // req_type
							   unsigned long* resultLen,// nitems_return
							   unsigned char** result   // prop_return
							  ) const;
	Time getWMUserTime(Window _wid);
	//Window type
	Atom _NET_WM_WINDOW_TYPE;
	//Window active
	Atom _NET_ACTIVE_WINDOW;
	//Window state
	Atom _NET_WM_STATE;
	//Window allowed actions
	Atom _NET_WM_ALLOWED_ACTIONS;
	//Window types
	Atom _NET_WM_WINDOW_TYPE_DESKTOP;
	Atom _NET_WM_WINDOW_TYPE_DOCK;
	Atom _NET_WM_WINDOW_TYPE_TOOLBAR;
	Atom _NET_WM_WINDOW_TYPE_MENU;
	Atom _NET_WM_WINDOW_TYPE_UTILITY;
	Atom _NET_WM_WINDOW_TYPE_SPLASH;
	Atom _NET_WM_WINDOW_TYPE_DIALOG;
	Atom _NET_WM_WINDOW_TYPE_NORMAL;

	Atom _NET_WM_USER_TIME;

	//Icon
	Atom _NET_WM_ICON;

	//char * Get_Window_Property_Data_And_Type (Window _wid,Atom atom, long *length, Atom *type, int *size);

};

#endif // NDXTOOLS_H
