#pragma once
#include "common.h"
#include "gui.h"
#include "browser.h"
// all in use, dont know about common.h
#include "layouts/copy_layout.h"
#include "layouts/delete_layout.h"

int Initialize();
void Finalize();

class MainApplication : public pu::Application
{
    public:
        MainApplication();
        ~MainApplication();
        void Update();
        MainLayout *GetMainLayout();
        ui::CopyLayout *GetCopyLayout();
        ui::DeleteLayout *GetDeleteLayout();
        Browser *GetBrowser();

        void CopyAction();
        void MoveAction();
        void PasteAction();
        void RenameAction();
        void DeleteAction();
        void NavigateBackAction();
        void SelectAction();
        void SortAction();
        void InfoAction();
        void HelpAction();
        void ExitAction();

        // debug
        void SetDebugText(std::string _text);
        //void LogDebugText(std::string _text);

    private:
        /// Helpful variables
        bool helpinfo;

        // previous values
        bool prehelpinfo;
        bool prebatcharg;
        u32 prebatstatus;
        u32 prewifistatus;
        std::string pretime;

        /// Layout instances
        // container for all layouts
        std::vector<pu::Layout*> allLayouts;
        void InitAllLayouts();

        // instances
        MainLayout *mainLayout;
        ui::CopyLayout *copyLayout;
        ui::DeleteLayout *deleteLayout;

        /// Module instances
        Browser *browser;
        void InitAllModules();

        /// Layouts' mandatory elements
        // container for all elements
        std::vector<pu::element::Element*> allElements;
        // container for all menu items
        std::vector<pu::element::MenuItem*> allMenuItems;

        // debug
        pu::element::TextBlock *debugText;

        // details
        pu::element::Rectangle *footerLine;
        pu::element::Rectangle *footerBottom;

        // status bar
        pu::element::TextBlock *clockText;
        pu::element::Image *batteryStatus;
        pu::element::Image *batteryCharging;
        pu::element::Image *wifiStatus;

        // sidebar
        pu::element::Rectangle *sidebarBackground;
        pu::element::Menu *sidebarMenu;
        pu::element::Image *sidebarLogo;

        // sidebar buttons
        pu::element::MenuItem *copyButton;
        pu::element::MenuItem *pasteButton;
        pu::element::MenuItem *deleteButton;
        pu::element::MenuItem *renameButton;
        pu::element::MenuItem *exitButton;

        // buttons
        pu::element::Image *button_A_icon;
        pu::element::Button *button_A;
        pu::element::Image *button_B_icon;
        pu::element::Button *button_B;
        pu::element::Image *button_Y_icon;
        pu::element::Button *button_Y;
        pu::element::Image *button_R_STICK_icon;
        pu::element::Button *button_R_STICK;
        pu::element::Image *button_L_STICK_icon;
        pu::element::Button *button_L_STICK;
};
