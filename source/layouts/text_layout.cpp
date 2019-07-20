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
        this->scroll = 0;
        this->locationText->SetText(ShortenText(_pathname, 62, "..."));
        this->originalTextFile = _text;
        this->PrepareTextToView();
        this->LoadTextIntoContainer();
    }

    void TextLayout::End()
    {
        this->originalTextFile.clear();
        this->wrappedTextFile.clear();
        this->locationText->SetText("");
        this->scroll = 0;
        app->LoadLayout(app->GetMainLayout());
    }

    void TextLayout::ScrollUp()
    {
        this->scroll -= (this->scroll == 0) ? 0 : 1;
        this->LoadTextIntoContainer();
    }

    void TextLayout::ScrollDown()
    {
        this->scroll += (this->wrappedTextFile.size()-3 < this->scroll) ? 0 : 1;
        this->LoadTextIntoContainer();
    }

    void TextLayout::LoadTextIntoContainer()
    {
        std::string outputtext;
        u32 maxlines = 17;
        for (std::vector<std::string>::size_type t = 0 + this->scroll; t != this->wrappedTextFile.size(); t++)
        {
            if (maxlines == 0)  break;
            maxlines--;
            outputtext += this->wrappedTextFile.at(t) + "\n";
        }
        this->textContainer->SetText(outputtext);
    }

    void TextLayout::PrepareTextToView()
    {
        this->wrappedTextFile.clear();
        u32 pos = 0;
        for (auto &t : this->originalTextFile)
        {
            std::vector<std::string> txtcntr = DivideLongText(t, 85);
            this->wrappedTextFile.insert(this->wrappedTextFile.begin() + pos, txtcntr.begin(), txtcntr.end());
            pos += txtcntr.size();
        }
    }
}
