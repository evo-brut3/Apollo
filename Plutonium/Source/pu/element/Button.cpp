#include <pu/element/Button.hpp>

namespace pu::element
{
    Button::Button(u32 X, u32 Y, u32 Width, u32 Height, std::string Content, draw::Color TextColor, draw::Color Color, u32 fontSize) : Element::Element()
    {
        this->x = X;
        this->y = Y;
        this->w = Width;
        this->h = Height;
        this->cnt = Content;
        this->clr = Color;
        this->hover = false;
        this->hoverfact = 255;
        this->fnt = render::LoadSharedFont(render::SharedFont::Standard, fontSize);
        this->ntex = render::RenderText(this->fnt, Content, TextColor);
        this->clickcb = [](){};
    }

    Button::~Button()
    {
        if(this->fnt != NULL)
        {
            render::DeleteFont(this->fnt);
            this->fnt = NULL;
        }
        if(this->ntex != NULL)
        {
            render::DeleteTexture(this->ntex);
            this->ntex = NULL;
        }
    }

    u32 Button::GetX()
    {
        return this->x;
    }

    void Button::SetX(u32 X)
    {
        this->x = X;
    }

    u32 Button::GetY()
    {
        return this->y;
    }

    void Button::SetY(u32 Y)
    {
        this->y = Y;
    }

    u32 Button::GetWidth()
    {
        return this->w;
    }

    void Button::SetWidth(u32 Width)
    {
        this->w = Width;
    }

    u32 Button::GetHeight()
    {
        return this->h;
    }

    void Button::SetHeight(u32 Height)
    {
        this->h = Height;
    }

    std::string Button::GetContent()
    {
        return this->cnt;
    }

    void Button::SetContent(std::string Content)
    {
        this->cnt = Content;
        render::DeleteTexture(this->ntex);
        this->ntex = render::RenderText(this->fnt, Content, this->clr);
    }

    draw::Color Button::GetColor()
    {
        return this->clr;
    }

    void Button::SetColor(draw::Color Color)
    {
        this->clr = Color;
        render::DeleteTexture(this->ntex);
        this->ntex = render::RenderText(this->fnt, this->cnt, Color);
    }

    void Button::SetContentFont(render::NativeFont Font)
    {
        render::DeleteFont(this->fnt);
        this->fnt = Font;
        render::DeleteTexture(this->ntex);
        this->ntex = render::RenderText(this->fnt, this->cnt, this->clr);
    }

    void Button::SetOnClick(std::function<void()> ClickCallback)
    {
        this->clickcb = ClickCallback;
    }

    void Button::OnRender(render::Renderer *Drawer)
    {
        u32 rdx = this->GetProcessedX();
        u32 rdy = this->GetProcessedY();
        s32 clrr = this->clr.R;
        s32 clrg = this->clr.G;
        s32 clrb = this->clr.B;
        s32 nr = clrr - 70;
        if(nr < 0) nr = 0;
        s32 ng = clrg - 70;
        if(ng < 0) ng = 0;
        s32 nb = clrb - 70;
        if(nb < 0) nb = 0;
        draw::Color nclr(nr, ng, nb, this->clr.A);
        if(this->hover)
        {
            Drawer->RenderRectangleFill(this->clr, rdx, rdy, this->w, this->h);
            if(this->hoverfact < 255)
            {
                Drawer->RenderRectangleFill({ nr, ng, nb, this->hoverfact }, rdx, rdy, this->w, this->h);
                this->hoverfact += 48;
            }
            else Drawer->RenderRectangleFill(nclr, rdx, rdy, this->w, this->h);
        }
        else
        {
            Drawer->RenderRectangleFill(this->clr, rdx, rdy, this->w, this->h);
            if(this->hoverfact > 0)
            {
                Drawer->RenderRectangleFill({ nr, ng, nb, this->hoverfact }, rdx, rdy, this->w, this->h);
                this->hoverfact -= 48;
            }
            else Drawer->RenderRectangleFill(this->clr, rdx, rdy, this->w, this->h);
        }
        u32 xw = render::GetTextWidth(this->fnt, this->cnt);
        u32 xh = render::GetTextHeight(this->fnt, this->cnt);
        u32 tx = ((this->w - xw) / 2) + rdx;
        u32 ty = ((this->h - xh) / 2) + rdy;
        Drawer->RenderTexture(this->ntex, tx, ty);
    }

    void Button::OnInput(u64 Down, u64 Up, u64 Held, bool Touch, bool Focus)
    {
        if(this->hover)
        {
            if(!Touch)
            {
                (this->clickcb)();
                this->hover = false;
                this->hoverfact = 255;
            }
        }
        else
        {
            if(Touch)
            {
                this->hover = true;
                this->hoverfact = 0;
            }
        }
    }
}