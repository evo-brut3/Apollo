#pragma once
#include "common.h"

namespace ui
{
    class DeleteLayout : public pu::Layout
    {
        public:
            DeleteLayout();
            ~DeleteLayout();
            void Start(u32 _value);
            void Update(std::string _current);

        private:
            pu::element::TextBlock *deleteTextHeader;
            pu::element::TextBlock *deleteCurrent;
            pu::element::ProgressBar *deleteProgressBar;
    };
}
