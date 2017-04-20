{
	"targets": [
		{
			"target_name": "Node_NDX",
			"sources": [ 
				"NDX.cc",
				"include/ndx.cpp",
				"include/ndxtools.cpp",
				"include/ndx.h",
				"include/ndxtools.h",
			],
			"libraries": [
				"-lX11",
			],
			"cflags": [ "-std=c++11" ]
		}
	]
}
