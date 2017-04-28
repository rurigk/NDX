#include "ndx.h"

NDX::NDX()
{
	NDX_Tools = new NDXTools();
}

NDX::~NDX()
{
	delete NDX_Tools;
}

std::vector <Window> NDX::GetWindowsInRoot(){
	std::vector <Window> windows;
	Atom ax = XInternAtom(NDX_Tools->display, "_NET_CLIENT_LIST" , true);
	Atom actualType;
		int format;
		unsigned long numItems, bytesAfter;
		unsigned char *data =0;
		int status = XGetWindowProperty(NDX_Tools->display,
									NDX_Tools->root,
									ax,
									0L,
									(~0L),
									false,
									AnyPropertyType,
									&actualType,
									&format,
									&numItems,
									&bytesAfter,
									&data);
	if (status >= Success && numItems)
	{
			long *array = (long*) data;
			for (u_long k = 0; k < numItems; k++)
			{
				Window w = (Window) array[k];
				windows.push_back(w);
			}
			XFree(data);
	}
	return windows;
}

std::string NDX::GetWindowTitle(Window _wid){

	std::string name = "";
	//first try the modern net-wm ones
	unsigned long length;
	unsigned char *data = NULL;
	Atom utf8Atom = NDX_Tools->atom("UTF8_STRING");

	if (NDX_Tools->getWindowProperty(_wid, NDX_Tools->atom("_NET_WM_VISIBLE_NAME"), utf8Atom, &length, &data))
	{
		if(data != NULL){
			name.assign((char*)data);
		}
		XFree(data);

	}

	if (name.empty())
	{
		if (NDX_Tools->getWindowProperty(_wid, NDX_Tools->atom("_NET_WM_NAME"), utf8Atom, &length, &data))
		{
			if(data != NULL){
				name.assign((char*)data);
			}
			XFree(data);
		}
	}

	if (name.empty())
	{
		if (NDX_Tools->getWindowProperty(_wid, NDX_Tools->atom("XA_WM_NAME"), XA_STRING, &length, &data))
		{
			if(data != NULL){
				name.assign((char*)data);
			}
			XFree(data);
		}
	}

	if (name.empty())
	{
		Status ok = XFetchName(NDX_Tools->display, _wid, (char**) &data);
		if(data != NULL){
			name.assign((char*)data);
		}
		if (0 != ok) XFree(data);
	}

	if (name.empty())
	{
		XTextProperty prop;
		if (XGetWMName(NDX_Tools->display, _wid, &prop))
		{
			name.assign((char*) prop.value);
			XFree(prop.value);
		}
	}

return name;
}

std::string NDX::GetApplicationName(Window _wid)
{
	XClassHint hint;
	std::string ret;

	if (XGetClassHint(NDX_Tools->display, _wid, &hint))
	{
		if (hint.res_name)
		{
			ret = hint.res_name;
			XFree(hint.res_name);
		}
		if (hint.res_class)
		{
			XFree(hint.res_class);
		}
	}

	return ret;
}

bool NDX::RaiseWindow(Window _wid){
	if(NDX_Tools->raiseWindow(_wid) == 1){
		return true;
	}else{
		return false;
	}
}
std::string NDX::GetWindowType(Window _wid){
	std::string wtype = "";
	char *an;
	int status,di;
	unsigned char *prop_ret = NULL;
	unsigned long dl;
	Atom da,prop;
	status = XGetWindowProperty(NDX_Tools->display, _wid, NDX_Tools->_NET_WM_WINDOW_TYPE, 0L, sizeof (Atom), False,
									XA_ATOM, &da, &di, &dl, &dl, &prop_ret);

	if (status == Success && prop_ret)
	{
		prop = ((Atom *)prop_ret)[0];
		an = XGetAtomName(NDX_Tools->display, prop);
		if (an){
			wtype = std::string(an);
			XFree(an);
		}
	}
	return wtype;
}
std::vector <std::string> NDX::GetWindowState(Window _wid){
	std::vector <std::string> state;
	char *an;
	int status,di;
	unsigned char *prop_ret = NULL;
	unsigned long dl,nitems;
	Atom da,prop;
	status = XGetWindowProperty(NDX_Tools->display, _wid, NDX_Tools->_NET_WM_STATE, 0L, sizeof (Atom), False,
									XA_ATOM, &da, &di, &nitems, &dl, &prop_ret);

	if (status == Success && prop_ret)
	{
		//int total = sizeof(prop_ret);
		for (int i = 0; i < nitems; i++) {
			prop = ((Atom *)prop_ret)[i];
			//std::cout << prop << std::endl;
			unsigned long aid = prop;
			if(aid < 1000 && aid > 0){
				an = XGetAtomName(NDX_Tools->display, prop);
				std::string st(an);
				if (an){
					if(st.find("_NET") != std::string::npos){
						state.push_back(st);
					}
					XFree(an);
				}
			}

		}
	}
	return state;
}

std::vector <std::string> NDX::GetWindowAllowedActions(Window _wid){
	std::vector <std::string> actions;
	char *an;
	int status,di;
	unsigned char *prop_ret = NULL;
	unsigned long dl,nitems;
	Atom da,prop;
	status = XGetWindowProperty(NDX_Tools->display, _wid, NDX_Tools->_NET_WM_ALLOWED_ACTIONS, 0L, sizeof (Atom), False,
									XA_ATOM, &da, &di, &nitems, &dl, &prop_ret);

	if (status == Success && prop_ret)
	{
		//int total = sizeof(prop_ret);
		for (int i = 0; i < nitems; i++) {
			prop = ((Atom *)prop_ret)[i];
			//std::cout << prop << std::endl;
			unsigned long aid = prop;
			if(aid < 1000 && aid > 0){
				an = XGetAtomName(NDX_Tools->display, prop);
				std::string st(an);
				if (an){
					if(st.find("_NET") != std::string::npos){
						actions.push_back(st);
					}
					XFree(an);
				}
			}

		}
	}
	return actions;
}

Icon NDX::GetWindowIcon(Window _wid){
	Icon icon;
	std::vector <int> imgdata;
	int status;
	int format;
	unsigned long type, nitems, extra;
	unsigned long* data = 0;

	status = XGetWindowProperty(NDX_Tools->display, _wid, NDX_Tools->_NET_WM_ICON,
												0L, LONG_MAX, False, XA_CARDINAL,
												&type, &format, &nitems, &extra,
												(unsigned char**)&data);
	if (status == Success && data)
	{
		int w = data[0];
		int h = data[1];
		for (int i=0; i<(w*h); ++i){
			unsigned int a, r, g, b;
			a = (data[i+2] & 0xff000000) >> 24;
			r = (data[i+2] & 0x00ff0000) >> 16;
			g = (data[i+2] & 0x0000ff00) >> 8;
			b = (data[i+2] & 0x000000ff);
			imgdata.push_back(r);
			imgdata.push_back(g);
			imgdata.push_back(b);
			imgdata.push_back(a);
			//std::cout << "r:" << r  << " g:" << g << " b:" << b << " a:" << a << std::endl;
		}
		icon.data = imgdata;
		icon.width = w;
		icon.height = h;
	}else{
		icon.data = imgdata;
		icon.width = 0;
		icon.height = 0;
	}
	return icon;
}

std::vector <Atom> NDX::NetSupported(Window _wid){
	std::vector <Atom> atoms;
	int status,di;
	unsigned char *prop_ret = NULL;
	unsigned long dl;
	Atom da,prop;
	status = XGetWindowProperty(NDX_Tools->display, _wid, NDX_Tools->_NET_WM_ALLOWED_ACTIONS, 0L, sizeof (Atom), False,
									XA_ATOM, &da, &di, &dl, &dl, &prop_ret);

	if (status == Success && prop_ret)
	{
		int total = sizeof(prop_ret);
		for (int i = 0; i < total; i++) {
			prop = ((Atom *)prop_ret)[i];
			atoms.push_back(prop);
		}
	}
	return atoms;
}


std::string NDX::GetWindowIconName(Window _wid){

	std::string name = "";
	//first try the modern net-wm ones
	unsigned long length;
	unsigned char *data = NULL;
	Atom utf8Atom = NDX_Tools->atom("UTF8_STRING");

	if (NDX_Tools->getWindowProperty(_wid, NDX_Tools->atom("_NET_WM_VISIBLE_ICON_NAME"), utf8Atom, &length, &data))
	{
		if(data != NULL){
			name.assign((char*)data);
		}
		XFree(data);

	}

	if (name.empty())
	{
		if (NDX_Tools->getWindowProperty(_wid, NDX_Tools->atom("_NET_WM_ICON_NAME"), utf8Atom, &length, &data))
		{
			if(data != NULL){
				name.assign((char*)data);
			}
			XFree(data);
		}
	}

return name;
}

unsigned long NDX::GetWindowPid(Window _wid){
	unsigned long length;
	unsigned char *data = NULL;
	unsigned long pid;
	if (NDX_Tools->getWindowProperty(_wid, NDX_Tools->atom("_NET_WM_PID"), XA_CARDINAL, &length, &data))
	{
		if(data != NULL){
			pid = *(unsigned long*)data;
		}
		XFree(data);
	}
	return pid;
}

unsigned long NDX::GetWindowDesktop(Window _wid){
	unsigned long length;
	unsigned char *data = NULL;
	unsigned long pid;
	if (NDX_Tools->getWindowProperty(_wid, NDX_Tools->atom("_NET_WM_DESKTOP"), XA_CARDINAL, &length, &data))
	{
		if(data != NULL){
			pid = *(unsigned long*)data;
		}
		XFree(data);
	}
	return pid;
}

