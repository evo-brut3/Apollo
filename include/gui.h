#pragma once
#include "common.h"

class MainLayout : public pu::Layout
{
    public:
        MainLayout();
        ~MainLayout();
        void LoadItems(std::vector<File> &files);
        u32 GetSelectedIndex();
        u32 GetScrollIndex();
        bool IsEmptyDirTextShown();
        void SetCursorPosition(u32 _cursor, u32 _scroll);
        void SetMenuElementIndexColor(pu::draw::Color _color);
        void SetLocationBarText(std::string _text);
        void OpenAction(std::string _pathname);
        void SetOnFocus(bool _focus);

    private:
        pu::element::TextBlock *emptyDirText;
        pu::element::Menu *filesMenu;

        // location bar
        pu::element::TextBlock *locationText;
        pu::element::Image *locationIcon;

        // to do: remember user choice of sorttype
        SortType sortType = SortType::Alphabetical;
        int ItemsToShow = 8;
};
