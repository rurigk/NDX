// hello.cc
#include <node.h>
#include <cstring>
#include "include/ndx.h"

NDX * ndx;

namespace Node_NDX {

	using namespace v8;


	void Method(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
	}

	void GetWindowsInRoot(const v8::FunctionCallbackInfo<v8::Value>&args) {
		Isolate * isolate = args.GetIsolate();
		Local<Array> a = Array::New(isolate);
		std::vector <Window> windows;
		windows = ndx->GetWindowsInRoot();
		for(uint i = 0; i < windows.size() ;i++){
			a->Set(i, Number::New(isolate, (long)windows[i]));
		}
		args.GetReturnValue().Set(a);
	}
	void GetWindowTitle(const v8::FunctionCallbackInfo<v8::Value>&args) {
		Isolate * isolate = args.GetIsolate();
		long value = args[0]->Int32Value();
		Window w = (Window)value;
		std::string str = ndx->GetWindowTitle(w);
		Local<String> retval = String::NewFromUtf8(isolate, str.c_str());
		args.GetReturnValue().Set(retval);
	}
	void GetWindowType(const v8::FunctionCallbackInfo<v8::Value>&args) {
		Isolate * isolate = args.GetIsolate();
		long value = args[0]->Int32Value();
		Window w = (Window)value;
		std::string str = ndx->GetWindowType(w);
		Local<String> retval = String::NewFromUtf8(isolate, str.c_str());
		args.GetReturnValue().Set(retval);
	}
	void GetWindowState(const v8::FunctionCallbackInfo<v8::Value>&args) {
		Isolate * isolate = args.GetIsolate();
		long value = args[0]->Int32Value();
		Window w = (Window)value;

		Local<Array> a = Array::New(isolate);
		std::vector <std::string> states;
		states = ndx->GetWindowState(w);
		for(uint i = 0; i < states.size() ;i++){
			a->Set(i, String::NewFromUtf8(isolate, states[i].c_str()));
		}
		args.GetReturnValue().Set(a);
	}
	void GetWindowAllowedActions(const v8::FunctionCallbackInfo<v8::Value>&args) {
		Isolate * isolate = args.GetIsolate();
		long value = args[0]->Int32Value();
		Window w = (Window)value;

		Local<Array> a = Array::New(isolate);
		std::vector <std::string> actions;
		actions = ndx->GetWindowAllowedActions(w);
		for(uint i = 0; i < actions.size() ;i++){
			a->Set(i, String::NewFromUtf8(isolate, actions[i].c_str()));
		}
		args.GetReturnValue().Set(a);
	}

	void GetWindowIcon(const v8::FunctionCallbackInfo<v8::Value>&args) {
		Isolate * isolate = args.GetIsolate();
		long value = args[0]->Int32Value();
		Window w = (Window)value;
		Icon icon = ndx->GetWindowIcon(w);
		
		HandleScope scope(isolate);
		auto context = isolate->GetCurrentContext();

		Local<Object> obj = Object::New(isolate);
		Local<ArrayBuffer> ab = ArrayBuffer::New(isolate,icon.data.size());
		Local<Uint8ClampedArray> ca = Uint8ClampedArray::New (ab, 0, icon.data.size());
		for(uint i = 0; i < icon.data.size() ;i++){
			ca->Set(context,i, Number::New(isolate, icon.data[i]));
		}
		obj->Set(String::NewFromUtf8(isolate, "width"), Number::New(isolate, (int)icon.width));
		obj->Set(String::NewFromUtf8(isolate, "height"), Number::New(isolate, (int)icon.height));
		obj->Set(String::NewFromUtf8(isolate, "data"), ca);
		args.GetReturnValue().Set(obj);
	}

	void RaiseWindow(const v8::FunctionCallbackInfo<v8::Value>&args) {
		Isolate * isolate = args.GetIsolate();
		long value = args[0]->Int32Value();
		Window w = (Window)value;
		bool r = ndx->RaiseWindow(w);
		args.GetReturnValue().Set(Boolean::New(isolate, r));
	}

	void init(Local<Object> exports) {
		ndx = new NDX();
		NODE_SET_METHOD(exports, "GetWindowsInRoot", GetWindowsInRoot);
		NODE_SET_METHOD(exports, "GetWindowTitle", GetWindowTitle);
		NODE_SET_METHOD(exports, "GetWindowType", GetWindowType);
		NODE_SET_METHOD(exports, "GetWindowState", GetWindowState);
		NODE_SET_METHOD(exports, "GetWindowAllowedActions", GetWindowAllowedActions);
		NODE_SET_METHOD(exports, "RaiseWindow", RaiseWindow);
		NODE_SET_METHOD(exports, "GetWindowIcon", GetWindowIcon);
	}

	NODE_MODULE(Node_NDX, init)

}  // namespace demo
