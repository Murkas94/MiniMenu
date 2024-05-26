#include "FileSystemViewer.h"
#include "../../Entries/GenericValueMenuEntry.h"

using namespace MiniMenu::Extras;

#if 0
#define DBGSERIAL Serial
#else
#define DBGSERIAL if(false) Serial
#endif

FileSystemViewer::FileSystemViewer(Adafruit_GFX& gfx, IFileSystemAdapter& fs, const char* const baseDir)
    : _menu(gfx)
    , _fileListTab(_menu.AddTab())
    , _fileSystemAdapter(fs)
{
    EnterDir(baseDir ? baseDir : "");
}

void FileSystemViewer::Update(bool forceUpdate)
{
    if(_backInput.IsTriggered())
    {
        // go one stage back
        if(!ExitDir())
        {
            // Dir-Selection was aborted, the menu was possible deleted on the callback, so return.
            return;
        }
    }
    if(_selectInput.IsTriggered())
    {
        // go one stage forward
        int16_t selectedIndex = _fileListTab.GetSelectedEntryIndex();
        if(selectedIndex > 0)
        {
            const auto& selectedEntry = _dirEntries[selectedIndex-1];
            switch(std::get<1>(selectedEntry))
            {
            case IFileSystemAdapter::EntryType::Directory:
                if(!EnterDir(std::get<0>(selectedEntry).c_str()))
                {
                    // Dir-Selection was aborted, the menu was possible deleted on the callback, so return.
                    return;
                }
                break;
            case IFileSystemAdapter::EntryType::File:
                {
                    std::string selectedFile = _currentDir + '/' + std::get<0>(selectedEntry);
                    DBGSERIAL.printf("Selected file: \"%s\"\n", selectedFile.c_str());
                    if(OnFileSelected)
                    {
                        OnFileSelected(selectedFile.c_str());
                        // Since a file was selected, the menu was possible deleted on the callback, so return.
                        return;
                    }
                    break;
                }
            }
        }
    }
    forceUpdate |= _forceUpdate || _backInput.IsTriggered() || _selectInput.IsTriggered();
    _forceUpdate = false;
    _menu.Handle(forceUpdate);
}

bool FileSystemViewer::EnterDir(const char* const dir)
{
    _currentDir = (_currentDir == "/") ? (std::string("/") + dir) : (_currentDir + '/' + dir);
    _currentFolder = dir;
    DBGSERIAL.printf("Entering Dir: \"%s\" (Folder: \"%s\")\n", _currentDir.c_str(), _currentFolder.c_str());
    if(OnDirEntered && !OnDirEntered(_currentDir.c_str())){ return false; }
    UpdateEntries();
    return true;
}

bool FileSystemViewer::ExitDir()
{
    if(_currentDir == "/"){ return true; }  // Can't go back on root
    size_t slashPos = _currentDir.find_last_of('/');
    if(slashPos == 0)
    {
        _currentDir = "/";
        _currentFolder = _currentDir;
    }
    else
    {
        _currentDir = _currentDir.substr(0, slashPos);
        slashPos = _currentDir.find_last_of('/');
        if(slashPos == std::string::npos)
        {
            // This should not be possible?
            _currentFolder = "/";
        }else
        {
            _currentFolder = _currentDir.substr(slashPos + 1);
        }
    }
    DBGSERIAL.printf("Entering Dir: \"%s\" (Folder: \"%s\")\n", _currentDir.c_str(), _currentFolder);
    if(OnDirEntered && !OnDirEntered(_currentDir.c_str())){ return false; }
    UpdateEntries();
    return true;
}

void FileSystemViewer::UpdateEntries()
{
// Remove old entries
    _fileListTab.ClearEntries();
    // Get new entries
    _dirEntries = _fileSystemAdapter.GetDirEntries(_currentDir.c_str());
    DBGSERIAL.println("Files in dir:");
    if(_dirEntries.empty())
    {
        DBGSERIAL.println("\tNo files.");
    }
    _fileListTab.AddEntry(new GenericValueMenuEntry(_menu, _currentFolder)).SetIsSelectable(false);
    _fileListTab.SetFixedEntryCount(1);
    // Create new entries
    for(auto& entry : _dirEntries)
    {
        // Add entries
        DBGSERIAL.printf("\t%s (%s)\n", std::get<0>(entry).c_str(), (std::get<1>(entry) == IFileSystemAdapter::EntryType::File) ? "File" : "Dir");
        _fileListTab.AddEntry(new GenericValueMenuEntry(_menu, std::get<0>(entry)));
    }
}