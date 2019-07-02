#include <string>
#include "layouts/copy_layout.h"
#include "app.h"

namespace ui
{
    extern MainApplication *app;

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
        std::string action = "";
        if (_moveFlag)
            action = "Moving ";
        else
            action = "Copying ";

        std::string item = "";
        if (_value > 1)
            item = " items...";
        else
            item = " item...";

        this->copyTextHeader->SetText(action + std::to_string(_value) + item);
        this->copyProgressBar->SetMaxValue(_value);
        this->copyProgressBar->ClearProgress();
    }

    void CopyLayout::Update(std::string _from, std::string _to)
    {
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

        this->copyTextFrom->SetText("From: " + _from);
        this->copyTextTo->SetText("To: " + _to);

        this->copyProgressBar->IncrementProgress(1);
    }

    void CopyLayout::Finish()
    {
        this->copyTextHeader->SetText("Finishing...");
        this->copyTextTo->SetText("");
    }

    void CopyLayout::FinishUpdate(std::string _item)
    {
        if (_item.length() > 54)
        {
            _item.erase(_item.begin(), _item.end()-54);
            _item.insert(0, "...");
        }

        this->copyTextFrom->SetText("Deleting: " + _item);
    }

    void CopyLayout::Reset()
    {
        this->copyProgressBar->ClearProgress();
        this->copyTextHeader->SetText("");
        this->copyTextTo->SetText("");
        this->copyTextFrom->SetText("");
    }
}
