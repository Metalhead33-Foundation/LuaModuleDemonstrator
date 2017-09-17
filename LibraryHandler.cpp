#include "LibraryHandler.hpp"
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstdlib>

LibraryHandler::LibraryHandler()
{

}
LibraryHandler::~LibraryHandler()
{
	for(LibraryIterator it = libraries.begin(); it != libraries.end();++it)
	{
		remove(it->temporary_path.c_str());
	}
}
void LibraryHandler::prepareLibraries()
{
	PhysFs::FileHandle::StringList libs = PhysFs::FileHandle::EnumerateFiles("/modules/unix");
	for(PhysFs::FileHandle::StringList::iterator it = libs.begin(); it != libs.end(); ++it)
	{
		LibraryHeader head;
		std::string path = "/modules/unix/" + *it;
		head.original_path = *it;
		PhysFs::FileHandle::SharedHandle hand = PhysFs::FileHandle::openRead(path.c_str());
		head.temporary_path = "/tmp/" + *it;
		head.fd = open(head.temporary_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
		if (head.fd == -1)
		{
			perror("Error opening file for writing");
			exit(EXIT_FAILURE);
		}

		// Stretch the file size to the size of the (mmapped) array of char

		if (lseek(head.fd, hand->pFileLength(), SEEK_SET) == -1)
		{
			close(head.fd);
			perror("Error calling lseek() to 'stretch' the file");
			exit(EXIT_FAILURE);
		}
		if (write(head.fd, "", 1) == -1)
		{
			close(head.fd);
			perror("Error writing last byte of the file");
			exit(EXIT_FAILURE);
		}
		head.cont = mmap(0, hand->pFileLength(), PROT_READ | PROT_WRITE, MAP_SHARED, head.fd, 0);
		if (head.cont == MAP_FAILED)
		{
			close(head.fd);
			perror("Error mmapping the file");
			exit(EXIT_FAILURE);
		}
		hand->pRead(head.cont,1,hand->pFileLength());
		// Write it now to disk
		if (msync(head.cont, hand->pFileLength(), MS_SYNC) == -1)
		{
			perror("Could not sync the file to disk");
		}

		// Don't forget to free the mmapped memory
		if (munmap(head.cont, hand->pFileLength()) == -1)
		{
			close(head.fd);
			perror("Error un-mmapping the file");
			exit(EXIT_FAILURE);
		}

		// Un-mmaping doesn't close the file, so we still need to do that.
		close(head.fd);
		libraries.push_back(head);
	}
}
void LibraryHandler::processLibraries()
{
	for(LibraryIterator it = libraries.begin(); it != libraries.end();++it)
	{
		ExtensionFactory::getSingleton()->registerFramework(it->original_path.c_str(),it->temporary_path.c_str());
	}
	ExtensionFactory::getSingleton()->prepareLibraries();
}
