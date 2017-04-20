#include "ndxtools.h"

#ifndef min
#define min(a,b)  ((a) < (b) ? (a) : (b))
#endif

#define MAX_LEN 1000000

#define SOURCE_NORMAL 1
#define SOURCE_PAGER 2

NDXTools::NDXTools()
{
	display = XOpenDisplay(":0");
	root = XDefaultRootWindow(display);
	//Window type
	_NET_WM_WINDOW_TYPE = XInternAtom(display, "_NET_WM_WINDOW_TYPE", True);
	//Window active
	_NET_ACTIVE_WINDOW = XInternAtom(display, "_NET_ACTIVE_WINDOW", True);
	//Window state
	_NET_WM_STATE = XInternAtom(display, "_NET_WM_STATE", True);
	//Window allowed actions
	_NET_WM_ALLOWED_ACTIONS = XInternAtom(display, "_NET_WM_ALLOWED_ACTIONS", True);
	//Window types
	_NET_WM_WINDOW_TYPE_DESKTOP = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DESKTOP", True);
	_NET_WM_WINDOW_TYPE_DOCK = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", True);
	_NET_WM_WINDOW_TYPE_TOOLBAR = XInternAtom(display, "_NET_WM_WINDOW_TYPE_TOOLBAR", True);
	_NET_WM_WINDOW_TYPE_MENU = XInternAtom(display, "_NET_WM_WINDOW_TYPE_MENU", True);
	_NET_WM_WINDOW_TYPE_UTILITY = XInternAtom(display, "_NET_WM_WINDOW_TYPE_UTILITY", True);
	_NET_WM_WINDOW_TYPE_SPLASH = XInternAtom(display, "_NET_WM_WINDOW_TYPE_SPLASH", True);
	_NET_WM_WINDOW_TYPE_DIALOG = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DIALOG", True);
	_NET_WM_WINDOW_TYPE_NORMAL = XInternAtom(display, "_NET_WM_WINDOW_TYPE_NORMAL", True);
	_NET_WM_USER_TIME = XInternAtom(display, "_NET_WM_USER_TIME", True);

	//Icon
	_NET_WM_ICON = XInternAtom(display, "_NET_WM_ICON", True);
}

NDXTools::~NDXTools()
{

}

Atom NDXTools::atom(const char* atomName)
{
	Atom atom = XInternAtom(display, atomName, false);
	return atom;
}

bool NDXTools::getWindowProperty(Window window,
					   Atom atom,               // property
					   Atom reqType,            // req_type
					   unsigned long* resultLen,// nitems_return
					   unsigned char** result   // prop_return
					  ) const
{
	int  format;
	unsigned long type, rest;
	return XGetWindowProperty(display, window, atom, 0, 4096, false,
							  reqType, &type, &format, resultLen, &rest,
							  result)  == Success;
}
bool NDXTools::getRootWindowProperty(Atom atom,    // property
						   Atom reqType,            // req_type
						   unsigned long* resultLen,// nitems_return
						   unsigned char** result   // prop_return
						  ) const
{
	return getWindowProperty(root, atom, reqType, resultLen, result);
}

int NDXTools::clientMessage(Window _wid, Atom _msg,
							unsigned long data0,
							unsigned long data1,
							unsigned long data2,
							unsigned long data3,
							unsigned long data4)
{
	XClientMessageEvent msg;
	msg.window = _wid;
	msg.type = ClientMessage;
	msg.message_type = _msg;
	msg.send_event = true;
	msg.display = display;
	msg.format = 32;
	msg.data.l[0] = data0;
	msg.data.l[1] = data1;
	msg.data.l[2] = data2;
	msg.data.l[3] = data3;
	msg.data.l[4] = data4;

	int status = XSendEvent(display, root, false, (SubstructureRedirectMask | SubstructureNotifyMask) , (XEvent *) &msg);
	if (status == 1){
		return 1;
	}else{
		return 0;
	}
}

int NDXTools::raiseWindow(Window _wid)
{
	time_t t;
	time(&t);
	return clientMessage(_wid, atom("_NET_ACTIVE_WINDOW"),
				  SOURCE_PAGER,getWMUserTime(_wid));
}

void NDXTools::closeWindow(Window _wid)
{
	clientMessage(_wid, atom("_NET_CLOSE_WINDOW"),
				  0, // Timestamp
				  SOURCE_PAGER);
}

void NDXTools::moveWindow(Window _win, int _x, int _y)
{
	XMoveWindow(display, _win, _x, _y);
}

void NDXTools::resizeWindow(Window _wid, int _width, int _height)
{
	XResizeWindow(display, _wid, _width, _height);
}

Time NDXTools::getWMUserTime(Window _wid)
{
	time_t user_time = -1U;
	int format_ret;
	unsigned char *data_ret = NULL;
	unsigned long nitems_ret;
	unsigned long unused;
	Atom type_ret;
	if (XGetWindowProperty(display, _wid, _NET_WM_USER_TIME, 0l, 1l,
						   False, XA_CARDINAL, &type_ret, &format_ret,
						   &nitems_ret, &unused, &data_ret) == Success)
	{
		if (type_ret == XA_CARDINAL && format_ret == 32) {
			user_time = *((long *)data_ret);
		}
		if (data_ret) {
			XFree(data_ret);
		}
	}
	return (Time) user_time;
}

/*char * NDXTools::Get_Window_Property_Data_And_Type (Window _wid, Atom atom,
								   long *length, Atom *type, int *size)
{
	Atom actual_type;
	int actual_format;
	unsigned long nitems;
	unsigned long nbytes;
	unsigned long bytes_after;
	unsigned char *prop;
	int status;

	status = XGetWindowProperty(display, _wid, atom, 0, (MAX_LEN+3)/4,
				False, AnyPropertyType, &actual_type,
				&actual_format, &nitems, &bytes_after,
				&prop);
	if (status == BadWindow)
		std::cout << "window id does not exists! #" << _wid << std::endl;
	if (status != Success)
		std::cout << "XGetWindowProperty failed!" << _wid << std::endl;

	if (actual_format == 32)
	nbytes = sizeof(long);
	else if (actual_format == 16)
	nbytes = sizeof(short);
	else if (actual_format == 8)
	nbytes = 1;
	else if (actual_format == 0)
		nbytes = 0;

	*length = min(nitems * nbytes, MAX_LEN);
	*type = actual_type;
	*size = actual_format;
	return (char *)prop;
}
*/
