#include <string>
#include "layouts/copy_layout.h"
#include "app.h"
#include "utils.h"

extern MainApplication *app;

namespace ui
{
    CopyLayout::CopyLayout()
    {
        this->copyTextHeader = new pu::element::TextBlock(146+30+30, 220, "");
        this->Add(this->copyTextHeader);
        this->copyTextFrom = new pu::element::TextBlock(146+30+30, 270, "");
        this->Add(this->copyTextFrom);
        this->copyTextTo = new pu::element::TextBlock(146+30+30, 300, "");
        this->Add(this->copyTextTo);
        this->copyProgressBar = new pu::element::ProgressBar(146+30, 340, 1044, 25, 100.0f);
        this->copyProgressBar->SetColor({76, 176, 80, 255});
        this->Add(this->copyProgressBar);
    }

    CopyLayout::~CopyLayout()
    {
        delete this->copyTextFrom;
        delete this->copyTextTo;
        delete this->copyTextHeader;
        delete this->copyProgressBar;
    }

    void CopyLayout::Start(u32 _value, bool _moveFlag)
    {
        this->numberOfElements = _value;

        if (_moveFlag)
            this->action = "Moving ";
        else
            this->action = "Copying ";

        if (this->numberOfElements > 1)
            this->item = " items...";
        else
            this->item = " item...";

        this->copyTextHeader->SetText(this->action + std::to_string((u32) this->copyProgressBar->GetProgress()) + " of " + std::to_string(this->numberOfElements) + this->item);
        this->copyProgressBar->SetMaxValue(this->numberOfElements);
        this->copyProgressBar->ClearProgress();
        app->CallForRender();
    }

    void CopyLayout::Update(const std::string &_from, const std::string &_to)
    {
        this->copyTextFrom->SetText("From: " + ShortenText(_from, 58, "..."));
        this->copyTextTo->SetText("To: " + ShortenText(_to, 59, "..."));

        this->copyProgressBar->IncrementProgress(1);
        this->copyTextHeader->SetText(this->action + std::to_string((u32) this->copyProgressBar->GetProgress()) + " of " + std::to_string(this->numberOfElements) + this->item);
        app->CallForRender();
    }

    void CopyLayout::Finish()
    {
        this->copyTextHeader->SetText("Finishing...");
        this->copyTextTo->SetText("");
        app->CallForRender();
    }

    void CopyLayout::FinishUpdate(std::string _item)
    {
        this->copyTextFrom->SetText("Deleting: " + ShortenText(_item, 54, "..."));
        app->CallForRender();
    }

    void CopyLayout::Reset()
    {
        this->copyProgressBar->ClearProgress();
        this->copyTextHeader->SetText("");
        this->copyTextTo->SetText("");
        this->copyTextFrom->SetText("");
        app->CallForRender();
    }
}
