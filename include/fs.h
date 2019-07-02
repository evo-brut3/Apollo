#pragma once
#include "common.h"

namespace fs
{
    inline bool Filter(char *_name);
    inline void MakeDir(std::string _path);

    std::vector<File> OpenDir(std::string _path);
    bool IsDir(std::string _path);
    bool Exists(const std::string &_pathname);
    void GetContents(std::string _path);
    void CopyFile(std::string _source, std::string _dest);
    void CopyDir(std::string _source, std::string _dest);
    int RenameFile(std::string _newname, std::string _pathname);
    int Remove(std::string _filepath);

    int DeleteDirRecursive(std::string _pathname);
    void ListFilesRecursive(std::string _baselocation, std::string _filename, std::vector<ClipboardNode> &_filelist, std::string _pathname = "");
    u32 CountFilesRecursive(std::string _pathname, u32 _number = 0);
}
