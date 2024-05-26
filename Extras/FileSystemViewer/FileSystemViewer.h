#ifndef MINIMENU_FILESYSTEMVIEWER_H
#define MINIMENU_FILESYSTEMVIEWER_H

#include "../../MiniMenu.h"
#include "FileSystemAdapter.h"
#include <functional>

namespace MiniMenu
{
namespace Extras
{

class FileSystemViewer
{
private:
    Menu _menu;
    MenuTab& _fileListTab;
    IFileSystemAdapter& _fileSystemAdapter;
    bool _forceUpdate = true;

    Input::ButtonValue _selectInput;
    Input::ButtonValue _backInput;

    std::string _currentFolder;
    std::string _currentDir;
    IFileSystemAdapter::EntryList _dirEntries;

    bool EnterDir(const char* const dir);
    bool ExitDir();

    void UpdateEntries();

public:
    FileSystemViewer(Adafruit_GFX& gfx, IFileSystemAdapter& fs, const char* const baseDir = nullptr);

    inline Menu& GetMenu(){ return _menu; }
    void Update(bool forceUpdate = false);

    inline Input::ButtonValue& GetUpInput(){ return _menu.input.up; }
    inline Input::ButtonValue& GetDownInput(){ return _menu.input.down; }
    inline Input::ButtonValue& GetSelectInput(){ return _selectInput; }
    inline Input::ButtonValue& GetBackInput(){ return _backInput; }

    std::function<void(const char* const)> OnFileSelected;
    // Gets called when a dir is entered. When the function returns false, entering gets aborted.
    std::function<bool(const char* const)> OnDirEntered;
};

}
}

#endif
