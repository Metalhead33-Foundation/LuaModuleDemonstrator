#ifndef LIBRARYHANDLER_HPP
#define LIBRARYHANDLER_HPP
#include <ExtensionFactory.hpp>
#include <Physfs4Cpp.hpp>

class LibraryHandler
{
public:
	struct LibraryHeader
	{
		std::string original_path;
		std::string temporary_path;
		int fd;
		void* cont;
	};
	typedef std::vector<LibraryHeader> LibraryVector;
	typedef LibraryVector::iterator LibraryIterator;
	LibraryVector libraries;
	LibraryHandler();
	~LibraryHandler();
	void prepareLibraries();
	void processLibraries();
};

#endif // LIBRARYHANDLER_HPP
