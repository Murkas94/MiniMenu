#include "EspFileSystemAdapter.h"

#if 0
#define DBGSERIAL Serial
#else
#define DBGSERIAL if(false) Serial
#endif

using namespace MiniMenu::Extras;
using namespace fs;

std::vector<IFileSystemAdapter::Entry> EspFileSystemAdapter::GetDirEntries(const char* const dir)
{
    DBGSERIAL.printf("\"GetDirEntries\": (%s)\n", dir);
    fs::File root = _fileSystem.open((dir == nullptr || strlen(dir) == 0) ? "/" : dir);
    
    std::vector<Entry> entries;

    if(!root)
    {
        DBGSERIAL.println("\tDir does not exist.");
        return entries;
    }

    fs::File file = root.openNextFile();

    if(!file)
    {
        DBGSERIAL.println("\tNo Entries.");
        return entries;
    }
 
    while(file)
    {
        entries.emplace_back(Entry(std::string(file.name()), file.isDirectory() ? EntryType::Directory : EntryType::File));
        DBGSERIAL.printf("\t%s\n", file.name());
        file = root.openNextFile();
    }

    return entries;
}