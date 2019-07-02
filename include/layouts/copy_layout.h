#pragma once
#include "common.h"

namespace ui
{
    class CopyLayout : public pu::Layout
    {
        public:
            CopyLayout();
            ~CopyLayout();
            void Start(u32 _value, bool _moveFlag);
            void Update(std::string _from, std::string _to);
            void Finish();
            void FinishUpdate(std::string _item);
            void Reset();

        private:
            pu::element::TextBlock *copyTextHeader;
            pu::element::TextBlock *copyTextFrom;
            pu::element::TextBlock *copyTextTo;
            pu::element::ProgressBar *copyProgressBar;
    };
}
