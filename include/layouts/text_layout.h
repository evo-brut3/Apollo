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
            void Exit();

        private:
            pu::element::TextBlock *textContainer;

            // location bar
            pu::element::TextBlock *locationText;
            pu::element::Image *locationIcon;
    };
}
