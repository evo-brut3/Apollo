#include <algorithm>
#include "layouts/text_layout.h"
#include "app.h"
#include "utils.h"

extern MainApplication *app;

namespace ui
{
    TextLayout::TextLayout()
    {
        // text container
        this->textContainer = new pu::element::TextBlock(144, 122, "");
        this->textContainer->SetFont(GetRomFsFont("RobotoMono-Regular.ttf"), 22);
        this->Add(this->textContainer);

        // location bar
        this->locationText = new pu::element::TextBlock(145+25+10, 84, "");
        this->locationText->SetColor({10, 10, 10, 255}); // black
        this->Add(locationText);

        // location icon
        this->locationIcon = new pu::element::Image(145, 85, GetRomFsResource("file"));
        this->locationIcon->SetHeight(25);
        this->locationIcon->SetWidth(25);
        this->Add(locationIcon);
    }

    TextLayout::~TextLayout()
    {
        delete this->textContainer;
        delete this->locationText;
        delete this->locationIcon;
    }

    void TextLayout::Start(std::vector<std::string> _text, const std::string &_pathname)
    {
        app->SetInputControl(ControlType::TextLayout);

        this->locationText->SetText(_pathname);

        std::string outputtext;
        u32 maxlines = 17;
        for (auto &t : _text)
        {
            std::string buff = WrapText(t, 86);
            if (maxlines == 0)  break;
            size_t n = std::count(buff.begin(), buff.end(), '\n');
            maxlines -= n+1;
            outputtext += buff + "\n";
        }
        this->textContainer->SetText(outputtext);
    }

    void TextLayout::Exit()
    {
        app->LoadLayout(app->GetMainLayout());
        app->SetInputControl(ControlType::MainLayout);
    }

    void TextLayout::ScrollUp()
    {

    }

    void TextLayout::ScrollDown()
    {

    }
}
