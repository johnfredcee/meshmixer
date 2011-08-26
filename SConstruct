import os

if os.name == "posix":
	env = Environment()
	env["ENV"]["PKG_CONFIG_PATH"] = os.environ.get("PKG_CONFIG_PATH")
	env["CXXFLAGS"] = [ "-g",  "-fpermissive" ]
	env["CPPPATH"] = [ '/usr/include', './include' ]
	env["LIBPATH"] = [ '/usr/lib' ]
	env.ParseConfig("pkg-config OGRE --cflags --libs")
	env.ParseConfig("pkg-config assimp --cflags --libs")
	env.ParseConfig("pkg-config OIS --cflags --libs")
	env.ParseConfig("wx-config --cflags --libs")
	env.ParseConfig("pkg-config atk --cflags --libs")
	env.ParseConfig("pkg-config gdk-2.0 --cflags --libs")
	env.ParseConfig("pkg-config gdk-x11-2.0 --cflags --libs")
	env.ParseConfig("pkg-config gtk+-2.0 --cflags --libs")
	env.ParseConfig("pkg-config gtk+-x11-2.0 --cflags --libs")
	env.ParseConfig("pkg-config gl --cflags --libs")
	env.ParseConfig("pkg-config glu --cflags --libs")
	env.Append(LIBS = "X11")
	main_program_list=Glob("src/*.cpp")
	env.Program('bin/main', main_program_list)
