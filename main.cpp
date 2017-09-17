#include <iostream>
#include <fstream>
#include "LibraryHandler.hpp"

using namespace std;
#include <vector>

int main(int argc, char *argv[])
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	PhysFs::FileHandle::Initialize(argv[0]);
	std::string line;
	std::ifstream infile("mountlist.txt");
	while (std::getline(infile, line))
	{
		PhysFs::FileHandle::Mount(line);
	}
	PhysFs::FileHandle::Mount(PhysFs::FileHandle::GetBaseDir() + PhysFs::FileHandle::GetDirSeparator() + "data" );
	LibraryHandler handler;
	handler.prepareLibraries();
	handler.processLibraries();
	ExtensionFactory::getSingleton()->registerLibraries(L);
	PhysFs::FileHandle::SharedHandle handl = PhysFs::FileHandle::openRead("main.lua");
	std::vector<char> puff(handl->pFileLength());
	handl->pRead(puff.data(),1,handl->pFileLength());
	luaL_loadbuffer(L,puff.data(),puff.size(),"main.lua");
	lua_pcall(L, 0, 0, 0);
	lua_close(L);
	return 0;
}
