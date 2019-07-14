#include <vector>
#include <algorithm>
#include "gui.h"
#include "browser.h"
#include "app.h"
#include "fs.h"
#include "utils.h"

extern MainApplication *app;
extern std::vector <File> currentFiles;
extern std::string currentPath;

MainLayout::MainLayout()
{
    app->SetBackgroundColor({240, 240, 240, 255});

    // empty dir text
    this->emptyDirText = new pu::element::TextBlock(1, 1, "This directory is empty.");
    this->emptyDirText->SetHorizontalAlign(pu::element::HorizontalAlign::Center);
    this->emptyDirText->SetVerticalAlign(pu::element::VerticalAlign::Center);
    this->emptyDirText->SetColor({10, 10, 10, 255}); // black
    this->Add(emptyDirText);

    // location bar
    this->locationText = new pu::element::TextBlock(145+25+10, 84, "sdcard:/");
    this->locationText->SetColor({10, 10, 10, 255}); // black
    this->Add(locationText);

    // location icon
    this->locationIcon = new pu::element::Image(145, 85, GetRomFsResource("dir"));
    this->locationIcon->SetHeight(25);
    this->locationIcon->SetWidth(25);
    this->Add(locationIcon);

    // files menu
    this->filesMenu = new pu::element::Menu(144, 87+35, (1280-164), {240, 240, 240, 255}, 35, 15);
    this->filesMenu->SetOnFocusColor({254, 254, 254, 255});
    this->filesMenu->SetScrollbarColor({198, 198, 198, 255});
    this->filesMenu->SetDrawShadow(false);
    this->Add(this->filesMenu);
    this->SetElementOnFocus(this->filesMenu);
}

MainLayout::~MainLayout()
{
    delete this->emptyDirText;
    delete this->filesMenu;
    delete this->locationText;
    delete this->locationIcon;
}

void MainLayout::LoadItems(std::vector<File> &files)
{
    if (files.size() > 0)
    {
        // Focus on filesMenu
        this->filesMenu->SetVisible(true);
        this->emptyDirText->SetVisible(false);

        // Clean filesMenu before adding new elements
        this->filesMenu->ClearItems();

        // Draw every element of this vector
        for (auto &element : files)
        {
            // Check the size of the element and align it if it is a file
            char itemsize[11] = "";
            if (element.type == 0)  std::sprintf(itemsize, "%10s", (FormatSize(element.size).c_str()));

            // Set the name of the element and align size to it
            std::string elementname = ShortenText(element.name, 54, "...", 1);
            pu::element::MenuItem *item = new pu::element::MenuItem(elementname, itemsize);
            item->SetSecondNameFont(GetRomFsFont("RobotoMono-Regular.ttf"), 20);
            this->filesMenu->SetSecondNamePositionPercent(0.83);
            item->SetSecondNameColor({65, 65, 65, 255});

            // to do: check whether directory is empty/file
            item->SetIcon(GetRomFsResource(GetRomFsFileExt(element.type)));
            item->AddOnClick(std::bind(&MainLayout::OpenAction, this, element.pathname), KEY_A);
            this->filesMenu->AddItem(item);
        }

        // Select first item in the list
        this->filesMenu->SetSelectedIndex(0);
    }
    else
    {
        // Focus on emptyDirText
        this->filesMenu->SetVisible(false);
        this->emptyDirText->SetVisible(true);
    }
}

u32 MainLayout::GetSelectedIndex()
{
    return this->filesMenu->GetSelectedIndex();
}

u32 MainLayout::GetScrollIndex()
{
    return this->filesMenu->GetScrollIndex();
}

bool MainLayout::IsEmptyDirTextShown()
{
    return this->emptyDirText->IsVisible();
}

void MainLayout::SetCursorPosition(u32 _cursor, u32 _scroll)
{
    this->filesMenu->SetSelectedIndex(_cursor);
    this->filesMenu->SetScrollIndex(_scroll);
}

void MainLayout::SetMenuElementIndexColor(pu::draw::Color _color)
{
    this->filesMenu->GetSelectedItem()->SetColor(_color);
}

void MainLayout::SetLocationBarText(std::string _text)
{
    this->locationText->SetText(ShortenText(_text, 62, "..."));
}

void MainLayout::OpenAction(std::string _pathname)
{
    switch (fs::IsDir(_pathname + R"(/)"))
    {
        case 0:

        break;

        case 1:
            app->GetBrowser()->LoadFiles(_pathname);
        break;
    }
}

void MainLayout::SetOnFocus(bool _focus)
{
    this->filesMenu->SetOnFocus(_focus);
}
