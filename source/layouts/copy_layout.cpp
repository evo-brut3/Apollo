#include <string>
#include "layouts/copy_layout.h"
#include "app.h"

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

        this->copyTextHeader->SetText(this->action + std::to_string(this->copyProgressBar->GetProgress()) + " of " + std::to_string(this->numberOfElements) + this->item);
        this->copyProgressBar->SetMaxValue(this->numberOfElements);
        this->copyProgressBar->ClearProgress();
        app->CallForRender();
    }

    void CopyLayout::Update(const std::string &_from, const std::string &_to)
    {
        /*
        if (_from.length() > 58)
        {
            _from.erase(_from.begin(), _from.end()-58);
            _from.insert(0, "...");
        }

        if (_to.length() > 59)
        {
            _to.erase(_to.begin(), _to.end()-59);
            _to.insert(0, "...");
        }
        */

        this->copyTextFrom->SetText("From: " + ((_from.length() > 58) ? "..." + _from.substr(_from.length() - 58) : _from));
        this->copyTextTo->SetText("To: " + ((_to.length() > 59) ? "..." + _to.substr(_to.length() - 59) : _to));

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
        if (_item.length() > 54)
        {
            _item.erase(_item.begin(), _item.end()-54);
            _item.insert(0, "...");
        }

        this->copyTextFrom->SetText("Deleting: " + _item);
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
