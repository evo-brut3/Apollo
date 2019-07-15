#pragma once
#include "common.h"

namespace fs
{
    inline bool Filter(char *_name);
    inline void MakeDir(const std::string &_pathname);

    std::vector<File> OpenDir(const std::string &_pathname);
    bool IsDir(const std::string &_pathname);
    bool Exists(const std::string &_pathname);
    u32 GetSize(const std::string &_pathname);
    std::string GetPermissions(const std::string &_pathname);
    std::pair<std::vector<std::string>, std::vector<std::string>> GetContents(const std::string &_path);
    void CopyFile(const std::string &_source, const std::string &_dest);
    void CopyDir(const std::string &_source, const std::string &_dest);
    void CopyFileOverwrite(const std::string &_source, const std::string &_dest, bool _moveflag);
    void CopyDirOverwrite(const std::string &_source, const std::string &_dest, bool _moveflag);
    int Rename(const std::string &_newname, const std::string &_pathname);
    int Remove(const std::string &_pathname);

    int DeleteDirRecursive(const std::string &_pathname);
    void ListFilesRecursive(const std::string &_baselocation, const std::string &_filename, std::vector<ClipboardNode> &_filelist, const std::string &_pathname = "");
    std::pair<u32, u32> CountFilesAndDirsRecursive(const std::string &_pathname);
    u32 GetDirSizeRecursive(const std::string &_pathname);
}
