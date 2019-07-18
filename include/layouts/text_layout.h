#pragma once
#include "common.h"

namespace ui
{
    class TextLayout : public pu::Layout
    {
        public:
            TextLayout();
            ~TextLayout();
            void Start(std::vector<std::string> _text, const std::string &_pathname);
            void ScrollUp();
            void ScrollDown();
            void ScrollUpFast();
            void ScrollDownFast();
            void End();

        private:
            u32 scroll = 0;
            std::vector<std::string> originalTextFile;
            std::vector<std::string> wrappedTextFile;
            pu::element::TextBlock *textContainer;
            pu::element::TextBlock *locationText;
            pu::element::Image *locationIcon;
            void LoadTextIntoContainer();
            void PrepareTextToView();
    };
}
