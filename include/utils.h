#pragma once
#include "common.h"

void SortFiles(std::vector<File> &_files, SortType _sorttype);
std::string GetRomFsFileExt(bool _ext);
std::string GetRomFsResource(std::string _name, std::string _dir = "Common");
bool CompareNames(File _f, File _g);
bool CompareNamesReversed(File _f, File _g);

namespace sys
{
    std::string GetTextFromSoftwareKeyboard(std::string _header = "", std::string _guide = "", std::string _initial = "");
    std::string GetCurrentTime();
    u32 GetBatteryLevel();
    bool IsBatteryCharging();
    u32 GetWiFiConnectionSignal();
    bool IsNRO();
}
