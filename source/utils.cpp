#include <string>
#include "utils.h"
#include "app.h"
#include "fs.h"

extern MainApplication *app;

void SortFiles(std::vector<File> &_files, SortType _sorttype)
{
    switch (_sorttype)
    {
        case SortType::Alphabetical:
            std::sort(_files.begin(), _files.end(), CompareNames);
        break;

        case SortType::Alphabetical_Reversed:
            std::sort(_files.rbegin(), _files.rend(), CompareNamesReversed);
        break;

        default:
            std::sort(_files.begin(), _files.end(), CompareNames);
        break;
    }
}

std::string GetRomFsFileExt(bool _ext)
{
    switch (_ext)
    {
        case 0:
            return "file";
        break;

        case 1:
            return "dir";
        break;

        default:
            return "file";
        break;
    }
}

std::string GetRomFsResource(std::string _name, std::string _dir)
{
    return (R"(romfs:/)" + _dir + "/" + _name + R"(.png)");
}

bool CompareNames(File _f, File _g)
{
    std::transform(_f.name.begin(), _f.name.end(), _f.name.begin(), ::tolower);
    std::transform(_g.name.begin(), _g.name.end(), _g.name.begin(), ::tolower);

    switch (_f.type - _g.type)
    {
        case 0:
            return (_f.name < _g.name);
        break;

        case -1:
            return false;;
        break;

        case 1:
            return true;
        break;
    }
}

bool CompareNamesReversed(File _f, File _g)
{
    std::transform(_f.name.begin(), _f.name.end(), _f.name.begin(), ::tolower);
    std::transform(_g.name.begin(), _g.name.end(), _g.name.begin(), ::tolower);

    switch (_f.type - _g.type)
    {
        case 0:
            return (_f.name < _g.name);
        break;

        case -1:
            return true;;
        break;

        case 1:
            return false;
        break;
    }
}

namespace sys
{
    std::string GetTextFromSoftwareKeyboard(std::string _header, std::string _guide, std::string _initial) // to do: check exception if error
    {
        std::string text;
        SwkbdConfig kbd;
        char tmpoutstr[FS_MAX_PATH] = {0};
        Result rc = swkbdCreate(&kbd, 0);

        if (R_SUCCEEDED(rc))
        {
            swkbdConfigMakePresetDefault(&kbd);
            swkbdConfigSetHeaderText(&kbd, _header.c_str());
            swkbdConfigSetGuideText(&kbd, _guide.c_str());
            swkbdConfigSetInitialText(&kbd, _initial.c_str());
            rc = swkbdShow(&kbd, tmpoutstr, sizeof(tmpoutstr));
            if (R_SUCCEEDED(rc))
            {
                text = std::string(tmpoutstr);
            }
        }
        swkbdClose(&kbd);
        return text;
    }

    std::string GetCurrentTime()
    {
        time_t lTime = time(NULL);
        struct tm* timestruct = localtime((const time_t *) &lTime);
        int hour = timestruct->tm_hour;
        int min = timestruct->tm_min;
        int sec = timestruct->tm_sec;
        char time[9];
        std::sprintf(time, "%02d:%02d:%02d", hour, min, sec);
        return time;
    }

    u32 GetBatteryLevel()
    {
        u32 battery = 0;
        psmGetBatteryChargePercentage(&battery);
        return battery;
    }

    bool IsBatteryCharging()
    {
        ChargerType chargertype = ChargerType_None;
        psmGetChargerType(&chargertype);
        return (chargertype > ChargerType_None);
    }

    u32 GetWiFiConnectionSignal()
    {
        NifmInternetConnectionType contype;
        NifmInternetConnectionStatus constatus;
        u32 wifistrength = 0;

        Result rc = nifmGetInternetConnectionStatus(&contype, &wifistrength, &constatus);

        if (R_SUCCEEDED(rc) && contype == 1)
            return wifistrength;
        else
            return 5; // no connection or airplane mode or ethernet connection or nifmError
    }

    bool IsNRO()
    {
        return envIsNso();
    }
}
