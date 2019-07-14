#include <string>
#include "layouts/delete_layout.h"
#include "app.h"
#include "utils.h"

extern MainApplication *app;

namespace ui
{
    DeleteLayout::DeleteLayout()
    {
        this->deleteTextHeader = new pu::element::TextBlock(146+30+30, 220, "");
        this->Add(this->deleteTextHeader);
        this->deleteCurrent = new pu::element::TextBlock(146+30+30, 300, "");
        this->Add(this->deleteCurrent);
        this->deleteProgressBar = new pu::element::ProgressBar(146+30, 340, 1044, 25, 100.0f);
        this->deleteProgressBar->SetColor({76, 176, 80, 255});
        this->Add(this->deleteProgressBar);
    }

    DeleteLayout::~DeleteLayout()
    {
        delete this->deleteCurrent;
        delete this->deleteTextHeader;
        delete this->deleteProgressBar;
    }

    void DeleteLayout::Start(u32 _value)
    {
        if (_value > 1)
            this->deleteTextHeader->SetText("Deleting " + std::to_string(_value) + " items...");
        else
            this->deleteTextHeader->SetText("Deleting " + std::to_string(_value) + " item...");

        this->deleteProgressBar->SetMaxValue(_value);
        this->deleteProgressBar->ClearProgress();
    }

    void DeleteLayout::Update(std::string _current)
    {
        this->deleteCurrent->SetText("Item: " + ShortenText(_current, 56, "..."));
        this->deleteProgressBar->IncrementProgress(1);
    }
}
