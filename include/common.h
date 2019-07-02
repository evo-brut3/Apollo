#pragma once
#define SDCARD_PATH "sdmc:/"
#include <iostream>
#include <switch.h>
#include <vector>
#include <pu/Plutonium>

struct File
{
    std::string pathname;
    std::string name;
    std::string path;
    bool type; // 0 - file, 1 - dir
    std::string extension;
    bool selected = false;

    File(std::string _pn = "", std::string _n = "", std::string _p = "", bool _t = 0, std::string _x = "")
    : pathname(_pn),
      name(_n),
      path(_p),
      type(_t),
      extension(_x)
    {
    }
};

/*
// pathname - sdmc:/switch/text.txt
// name - text.txt
// path - sdmc:/switch/
*/

struct ClipboardNode
{
    std::string base;
    std::string path;
    bool directory;
    int overwrite;

    ClipboardNode(std::string _b = "", std::string _p = "", bool _d = 0, int _o = 0)
    : base(_b),
      path(_p),
      directory(_d), // to do: change it to "type"
      overwrite(_o) // 0 - don't overwrite, 1 - overwrite, 2 - inform that cannot overwrite
    {
    }
};

enum SortType
{
    Alphabetical = 0,
    Alphabetical_Reversed = 1
};
