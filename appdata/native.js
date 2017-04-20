const ndx = require('../build/Release/Node_NDX');
var wm_windows = [];
var canvas = document.createElement('canvas');
var ctx=canvas.getContext("2d");


function Window(){
	this.id = 0;
	this.title = "";
	this.type = "";
	this.state = [];
	this.allowed = [];
	this.icon_data = [];
	this.icon = null;
}
Window.prototype.isUserSpaceWindow = function() {
	switch(this.type){
		case "_NET_WM_WINDOW_TYPE_NORMAL":
		case "_NET_WM_WINDOW_TYPE_DIALOG":
		case "_NET_WM_WINDOW_TYPE_SPLASH":
		case "_NET_WM_WINDOW_TYPE_UTILITY":
		case "_NET_WM_WINDOW_TYPE_MENU":
			return true;
		break;
		default:
			return false;
		break
	}
};

function WM_GetWindows(){
	wm_windows = [];
	var windows = ndx.GetWindowsInRoot();
	for (var i = 0; i < windows.length; i++) {
		var win = new Window();
		win.id = windows[i];
		win.title = ndx.GetWindowTitle(windows[i]);
		win.type = ndx.GetWindowType(windows[i]);
		win.state = ndx.GetWindowState(windows[i]);
		win.allowed = ndx.GetWindowAllowedActions(windows[i]);
		win.icon_data = ndx.GetWindowIcon(windows[i]);

		if(win.icon_data.width != 0 && win.icon_data.height != 0){
			var myImageData = ctx.createImageData(win.icon_data.width, win.icon_data.height);
			myImageData.data.set(win.icon_data.data);
			win.icon = GenerateImage(win.icon_data.width, win.icon_data.height,myImageData);
		}
		wm_windows[wm_windows.length] = win;
	}
}

function GenerateImage(width,height,imgData){
	canvas.width = width;
	canvas.height = height;
	ctx.putImageData(imgData,0,0);
	return canvas.toDataURL("image/png");;
}

function GetNormalWindows(){
	var windows = [];
	for (var i = 0; i < wm_windows.length; i++) {
		if(wm_windows[i].isUserSpaceWindow()){
			windows[windows.length] = wm_windows[i];
		}
	}
	return windows;
}

function ShowWindowsList(windows){
	var html = "";
	for (var i = 0; i < windows.length; i++) {
		html += "<div><img width='30' src='"+windows[i].icon+"'>"+windows[i].title+" : "+windows[i].id+"</div>";
	}
	document.getElementById("windows").innerHTML = html;
}

window.addEventListener('load',function(){
	/**/
	setInterval(function(){
		WM_GetWindows();
		ShowWindowsList(GetNormalWindows());
	},1500)
	/**/
	//WM_GetWindows();
})