#pragma once
#include <iostream>
#include <vector>
#include "common.h"

extern std::string currentPath;
extern std::vector<File> currentFiles;

class Browser
{
    public:
        Browser();
        ~Browser();
        void LoadFiles(std::string _path);
        void Refresh();
        void NavigateBack();
        void SelectFile();
        void RenameFile();
        void RemoveFiles();
        void CopyFiles();
        void MoveFiles();
        void PasteFiles();
        void ChangeSortType(SortType _sorttype);
        u32 GetNumberOfSelected();
        u32 GetClipboardSize();

        std::string GetFileName();
        std::string GetFilePath();
        std::string GetFilePathName();
    private:
        u32 numberOfSelected = 0;
        bool moveFlag = 0; // 0 - copy, 1 - cut
        bool emptyDir = false;
        SortType sortType = SortType::Alphabetical;
        std::string currentPath = SDCARD_PATH;
        std::vector<File> currentFiles;
        std::vector<ClipboardNode> clipboard;
        inline void CopyFileOrDir(std::string _source, std::string _dest, bool _type);
        inline void CopyFileOrDirOverwrite(std::string _source, std::string _dest, bool _type);
};
