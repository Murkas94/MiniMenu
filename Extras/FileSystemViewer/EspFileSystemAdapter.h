#ifndef MINIMENU_ESPFILESYSTEMADAPTER_H
#define MINIMENU_ESPFILESYSTEMADAPTER_H

#include "FileSystemAdapter.h"
#include <FS.h>

namespace MiniMenu
{
namespace Extras
{

class EspFileSystemAdapter : public IFileSystemAdapter
{
private:
    fs::FS& _fileSystem;
public:
    EspFileSystemAdapter(fs::FS& fileSystem)
        : _fileSystem(fileSystem)
    {}
    
    // Should return all files and folders in the given directory.
    // Should return entries of root-directory if nullptr or empty string is given.
    std::vector<Entry> GetDirEntries(const char* const dir) override;
};

}
}

#endif