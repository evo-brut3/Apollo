#pragma once
#include "common.h"

namespace fs
{
    inline bool Filter(char *_name);
    inline void MakeDir(const std::string &_path);

    std::vector<File> OpenDir(const std::string &_path);
    bool IsDir(const std::string &_path);
    bool Exists(const std::string &_pathname);
    void GetContents(const std::string &_path);
    void CopyFile(const std::string &_source, const std::string &_dest);
    void CopyDir(const std::string &_source, const std::string &_dest);
    int RenameFile(const std::string &_newname, const std::string &_pathname);
    int Remove(const std::string &_filepath);

    int DeleteDirRecursive(const std::string &_pathname);
    void ListFilesRecursive(const std::string &_baselocation, const std::string &_filename, std::vector<ClipboardNode> &_filelist, const std::string &_pathname = "");
    u32 CountFilesRecursive(const std::string &_pathname, u32 _number = 0);
}
