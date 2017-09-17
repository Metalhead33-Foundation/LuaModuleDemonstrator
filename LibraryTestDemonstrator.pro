TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

include(ext/luamodulaloader2/qmake_modules/findLuaJIT.pro)
include(ext/luamodulaloader2/qmake_modules/findLibraryLinker.pro)
include(ext/luamodulaloader2/qmake_modules/findPhysFS.pro)
LIBS += -L"$$_PRO_FILE_PWD_/ext/luamodulaloader2/build/" -lLuaLibLoader2
# LIBS += "$$_PRO_FILE_PWD_/ext/Physfs4Cpp/build/libPhysfs4Cpp.so"
INCLUDEPATH += ext/luamodulaloader2
LIBS += -L"$$_PRO_FILE_PWD_/ext/Physfs4Cpp/build/" -lPhysfs4Cpp
# LIBS += "$$_PRO_FILE_PWD_/ext/luamodulaloader2/build/libLuaLibLoader2.so"
INCLUDEPATH += ext/Physfs4Cpp/include

SOURCES += main.cpp \
    LibraryHandler.cpp

HEADERS += \
    LibraryHandler.hpp
