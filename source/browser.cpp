#include "browser.h"
#include "app.h"
#include "gui.h"
#include "fs.h"
#include "utils.h"

extern MainApplication *app;

Browser::Browser()
{
    this->LoadFiles(currentPath);
}

Browser::~Browser()
{
}

void Browser::LoadFiles(std::string _path)
{
    // If it is dir load files into vector - currentFiles, then change currentPath
    currentFiles = fs::OpenDir(_path);

    // to do: if openDir returns error then do not clear items and make error message
    // Sort elements from currentFiles and then load them into filesMenu
    SortFiles(currentFiles, sortType);
    app->GetMainLayout()->LoadItems(this->currentFiles);

    // Change current location to _path
    currentPath = _path;
    app->GetMainLayout()->SetLocationBarText(currentPath);

    // Set number of selected items to zero
    numberOfSelected = 0;
}

void Browser::Refresh()
{
    u32 index = app->GetMainLayout()->GetSelectedIndex();
    u32 scroll = app->GetMainLayout()->GetScrollIndex();
    this->LoadFiles(this->currentPath);
    app->GetMainLayout()->SetCursorPosition(index, scroll);
}

void Browser::NavigateBack()
{
    std::size_t n = std::count(this->currentPath.begin(), this->currentPath.end(), '/');
    std::size_t d = this->currentPath.find_last_of("/\\");

    if (n > 1)
    {
        this->currentPath.erase(this->currentPath.begin()+d, this->currentPath.end());

        this->LoadFiles(this->currentPath);
    }
}

void Browser::SelectFile()
{
    // Select or unselect if already selected
    u32 index = app->GetMainLayout()->GetSelectedIndex();
    currentFiles.at(index).selected = !currentFiles.at(index).selected;

    switch (currentFiles.at(index).selected)
    {
        case 0: // unselected
            app->GetMainLayout()->SetMenuElementIndexColor({10, 10, 10, 255}); // black
            numberOfSelected--;
        break;

        case 1: // selected
            app->GetMainLayout()->SetMenuElementIndexColor({213, 160, 29, 255}); // gold
            numberOfSelected++;
        break;
    }
}

void Browser::RenameFile()
{
    std::string pathname = this->GetFilePathName();
    std::string path = this->GetFilePath();
    std::string oldname = this->GetFileName();
    std::string newname = sys::GetTextFromSoftwareKeyboard("Error: Couldn't rename the file, the name was too long", "Enter new filename", oldname);
    fs::RenameFile(pathname, path + R"(/)" + newname);
    this->Refresh();
}

void Browser::RemoveFiles()
{
    app->LoadLayout(app->GetDeleteLayout());

    if (numberOfSelected > 0)
    {
        u32 number = 1;
        for (auto &f : currentFiles)
            number += fs::CountFilesRecursive(f.pathname);

        app->GetDeleteLayout()->Start(number);
        app->CallForRender();

        for (auto &f : currentFiles)
        {
            if (f.selected == true)
            {
                if (fs::DeleteDirRecursive(f.pathname) != 0)
                    app->SetDebugText("Error while removing: " + f.pathname);
                else
                    app->SetDebugText("Removed: " + f.pathname);
            }
        }
    }
    else
    {
        app->GetDeleteLayout()->Start(fs::CountFilesRecursive(this->GetFilePathName()));
        app->CallForRender();

        std::string pathname = this->GetFilePathName();
        if (fs::DeleteDirRecursive(pathname) != 0)
            app->SetDebugText("Error while removing: " + pathname);
        else
            app->SetDebugText("Removed: " + pathname);
    }

    this->Refresh();
    app->LoadLayout(app->GetMainLayout());
}

void Browser::CopyFiles()
{
    clipboard.clear();

    if (numberOfSelected > 0)
    {
        for (auto &f : currentFiles)
        {
            if (f.selected == true)
            {
                ClipboardNode node
                (
                    f.path,                                     // base
                    R"(/)" + f.name,                            // pathname
                    fs::IsDir(f.path + R"(/)" + f.name)         // directory
                );
                clipboard.push_back(node);
            }
        }
    }
    else
    {
        ClipboardNode node
        (
            this->GetFilePath(),                        // base
            R"(/)" + this->GetFileName(),               // pathname
            fs::IsDir(this->GetFilePathName())          // directory
        );
        clipboard.push_back(node);
    }

    this->moveFlag = 0;
}

void Browser::MoveFiles()
{
    this->CopyFiles();
    this->moveFlag = 1;
}

void Browser::PasteFiles()
{
    // Check if the destined folder is the source folder
    if (clipboard.at(0).base == currentPath)
        return;

    u32 number = 1;
    for (auto &f : clipboard)
    {
        // Check if the destined folder is the subfolder of the source folder
        std::size_t pos = this->currentPath.find(f.base + f.path);
        if (pos == 0)
        {
            app->CreateShowDialog("Cannot paste files!", "The destined directory is the subdirectory of the source folder.", {"Ok"}, true);
            return;
        }

        // List all of files of a single element of clipboard
        std::vector<ClipboardNode> allFiles;
        allFiles.push_back(f);
        fs::ListFilesRecursive(f.base, f.path, allFiles);

        // Check if there is a possibility that there are already existing files or if there are
        // already other files/dirs which name is the same and flag them in clipboard
        for (auto &g : allFiles)
        {
            if (fs::Exists(currentPath + g.path))
            {
                f.overwrite = 1;
                break;
            }
        }

        // Count all files in clipboard
        number += allFiles.size();
    }
    int overwritestatus = 0; // 0 - ask about overwriting, 1 - overwrite everything, 2 - do not overwrite anything

    app->GetCopyLayout()->Reset();
    app->LoadLayout(app->GetCopyLayout());
    app->GetCopyLayout()->Start(number, this->moveFlag);
    //app->CallForRender();

    for (auto &f : clipboard)
    {
        switch (f.overwrite)
        {
            case 0: // no need to check if there are already the same files/dirs
                this->CopyFileOrDir(f.base + f.path, currentPath + f.path, f.directory);
            break;

            case 1: // check if there are already the same files/dirs
                this->CopyFileOrDirOverwrite(f.base + f.path, currentPath + f.path, f.directory);
            break;
        }
    }

    /*
    if (this->moveFlag == 1)
    {
        app->GetCopyLayout()->Finish();
        for (auto &f : clipboard)
        {
            fs::DeleteDirRecursive(f.base + f.path);
            app->GetCopyLayout()->FinishUpdate(f.base + f.path);
        }

        this->moveFlag = 0;
        this->clipboardHeader.clear();
        this->clipboard.clear();
    }
    */
    this->clipboard.clear();
    this->Refresh();
    app->LoadLayout(app->GetMainLayout());
}

void Browser::ChangeSortType(SortType _sorttype)
{
    this->sortType = _sorttype;
    this->Refresh();
}

u32 Browser::GetNumberOfSelected()
{
    return this->numberOfSelected;
}

u32 Browser::GetClipboardSize()
{
    return this->clipboard.size();
}

std::string Browser::GetFileName()
{
    return currentFiles.at(app->GetMainLayout()->GetSelectedIndex()).name;
}

std::string Browser::GetFilePath()
{
    return currentFiles.at(app->GetMainLayout()->GetSelectedIndex()).path;
}

std::string Browser::GetFilePathName()
{
    return currentFiles.at(app->GetMainLayout()->GetSelectedIndex()).pathname;
}

inline void Browser::CopyFileOrDir(std::string _source, std::string _dest, bool _type)
{
    if (_type)
        fs::CopyDir(_source, _dest);
    else
        fs::CopyFile(_source, _dest);
}

inline void CopyFileOrDirOverwrite(std::string _source, std::string _dest, bool _type)
{
    if (_type)
        fs::CopyDirOverwrite(_source, _dest);
    else
        fs::CopyFileOverwrite(_source, _dest);
}
