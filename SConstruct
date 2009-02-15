import os

mode = ARGUMENTS.get('mode', "debug")

# Setup our Build Environment, Smarter scripts might be able to change this at the command line,
if mode == "debug":
    print "Debug Build"
    env = Environment(  CCFLAGS='-Wall -ggdb -g3 -O0 -DEXT_HASH',
                        LDFLAGS='-g3'  )
else:
    print "Release Build"
    env = Environment( CFLAGS="-Wall -O2" )
    
# Our External Lib
env["ENV"]["PKG_CONFIG_PATH"] = os.environ.get("PKG_CONFIG_PATH")
env.ParseConfig('pkg-config OGRE --libs --cflags')
env.ParseConfig('wx-config --libs --cxxflags')
env.ParseConfig('pkg-config --libs --cflags gtk+-2.0')
env.ParseConfig('pkg-config --libs --cflags gdk-2.0')
env.ParseConfig('pkg-config --libs --cflags x11')

env.Append(LIBS =  r"assimp")
env.Append(LIBS =  r"GL")

env.Append(LIBPATH="./contrib/assimp/lib")

env.Append(CPPPATH = "./contrib/assimp/include")
env.Append(CPPPATH = "./include")

#check for required include files and libraries
conf = Configure(env)

#if not conf.CheckCXXHeader('Ogre.h'):
#   print 'Ogre must be installed!'
#   Exit(1)
if not conf.CheckLib('OgreMain'):
    print 'OgreMain library must be in path'
    Exit(1)



if mode=="debug":
    env.Program('bin/linux/debug/meshmixer_d', Glob("src/*.cpp"))
else:
    env.Program('bin/linux/release/meshmixer', Glob("src/*.cpp"))
