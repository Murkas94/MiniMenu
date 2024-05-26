#ifndef MINIMENU_FILESYSTEMADAPTER_H
#define MINIMENU_FILESYSTEMADAPTER_H

#include <vector>
#include <tuple>
#include <string>

namespace MiniMenu
{
namespace Extras
{

class IFileSystemAdapter
{
public:
    virtual ~IFileSystemAdapter(){}

    enum EntryType
    {
        Directory,
        File
    };
    typedef std::tuple<std::string, EntryType> Entry;
    typedef std::vector<Entry> EntryList;
    
    // Should return all files and folders in the given directory.
    // Should return entries of root-directory if nullptr or empty string is given.
    virtual EntryList GetDirEntries(const char* const dir);
};

}
}

#endif