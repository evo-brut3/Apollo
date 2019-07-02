#include <string.h>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "fs.h"
#include "app.h"

extern MainApplication *app;

namespace fs
{
    std::vector<std::string> Directories;
    std::vector<std::string> Files;

    inline bool Filter(char *_name) // is it necessary?
    {
        return (strcmp(_name, ".") && strcmp(_name, ".."));
    }

    inline void MakeDir(std::string _path)
    {
        mkdir(_path.c_str(), 0777);
    }

    std::vector<File> OpenDir(std::string _path)
    {
        DIR* dir;
        struct dirent* ent;
        std::vector<File> files;

        dir = opendir(_path.c_str());
        if (dir!=NULL)
        {
            while ((ent = readdir(dir)))
            {
                File _f
                (
                    _path + R"(/)" + ent->d_name,               // pathname
                    ent->d_name,                                // name
                    _path,                                      // path
                    IsDir(_path + R"(/)" + ent->d_name)         // type
                );

                files.push_back(File(_f));
            }
            app->SetDebugText("Loaded: " + _path);
        }
        else
            app->SetDebugText("Error loading: " + _path);

        closedir(dir);
        return files;
    }

    bool IsDir(std::string _path)
    {
        struct stat buf;
        stat((char *)_path.c_str(), &buf);
        return S_ISDIR(buf.st_mode);
    }

    bool Exists(const std::string &_pathname)
    {
        struct stat buff;
        return (stat(_pathname.c_str(), &buff) == 0);
    }

    void GetContents(std::string _path)
    {
        Files.clear();
        Directories.clear();
        DIR *dir;
        struct dirent *ent;
        dir = opendir(_path.c_str());

        if (dir)
        {
            while ((ent = readdir(dir)))
            {
                if (Filter(ent->d_name))
                {
                    std::string p = ent->d_name;

                    switch(IsDir(_path + R"(/)" + p))
                    {
                        case 0:
                            Files.push_back(p);
                        break;

                        case 1:
                            Directories.push_back(p);
                        break;
                    }
                }
            }
        }
        closedir(dir);
    }

    void CopyFile(std::string _source, std::string _dest)
    {
        std::ifstream source(_source, std::ios::binary);
        std::ofstream dest(_dest, std::ios::binary);

        source.seekg(0, std::ios::end);
        std::ifstream::pos_type size = source.tellg();
        source.seekg(0);

        char* buffer = new char[size];

        source.read(buffer, size);
        dest.write(buffer, size);

        delete[] buffer;
        source.close();
        dest.close();
    }

    void CopyDir(std::string _source, std::string _dest)
    {
        MakeDir(_dest);
        GetContents(_source);

        if (!Directories.empty())
        {
            for (auto &d : Directories)
            {
                CopyDir(_source + R"(/)" + d, _dest + R"(/)" + d);
            }
        }

        GetContents(_source);
        if (!Files.empty())
        {
            for (auto &f : Files)
            {
                CopyFile(_source + R"(/)" + f, _dest + R"(/)" + f);
            }
        }
    }

    int RenameFile(std::string _pathname, std::string _newname)
    {
        return (rename(_pathname.c_str(), _newname.c_str()));
    }

    int Remove(std::string _filepath)
    {
        int result;
        switch (IsDir(_filepath))
        {
            case 0:
                result = remove(_filepath.c_str());
            break;

            case 1:
                result = rmdir(_filepath.c_str());
            break;
        }
        return result;
    }

    int DeleteDirRecursive(std::string _pathname)
    {
        int result;
        DIR *dir = opendir(_pathname.c_str());
        struct dirent *ent;

        while ((ent = readdir(dir)) != NULL)
        {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
            {
                std::string filepath = _pathname + R"(/)" + ent->d_name;
                result = Remove(filepath);

                if (result != 0)
                {
                    DeleteDirRecursive(filepath);
                    result = Remove(filepath); // is it necessery?
                }

                app->GetDeleteLayout()->Update(_pathname);
                app->CallForRender();
            }
        }
        closedir(dir);

        result = Remove(_pathname); // got this, so the above seems useless
        app->GetDeleteLayout()->Update(_pathname);
        app->CallForRender();
        return result;
    }

    void ListFilesRecursive(std::string _baselocation, std::string _filename, std::vector<ClipboardNode> &_filelist, std::string _pathname)
    {
        std::string path;

        DIR *dir = opendir((_baselocation + _filename + _pathname).c_str());
        struct dirent *ent;

        if (!dir)
            return;

        while ((ent = readdir(dir)) != NULL)
        {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
            {
                path = _pathname + R"(/)" + ent->d_name;
                ClipboardNode node
                (
                    _baselocation,                                      // base
                    R"(/)" + _filename + path,                          // pathname
                    IsDir(_baselocation + _filename + path)             // directory
                );
                _filelist.push_back(node);

                ListFilesRecursive(_baselocation, _filename, _filelist, path);
            }
        }
        closedir(dir);
    }

    u32 CountFilesRecursive(std::string _pathname, u32 _number)
    {
        DIR *dir = opendir((_pathname).c_str());
        struct dirent *ent;

        if (!dir)
            return 0;

        while ((ent = readdir(dir)) != NULL)
        {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
            {
                _number++;
                _number += CountFilesRecursive(_pathname + R"(\)" + ent->d_name);
            }
        }
        closedir(dir);

        return _number;
    }
}
