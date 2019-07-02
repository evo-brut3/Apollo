#include <pu/element/Rectangle.hpp>

namespace pu::element
{
    Rectangle::Rectangle(u32 X, u32 Y, u32 Width, u32 Height, draw::Color RecColor, u32 BorderRadius) : Element::Element()
    {
        this->x = X;
        this->y = Y;
        this->w = Width;
        this->h = Height;
        this->clr = RecColor;
        this->borderr = BorderRadius;
		this->drawSideShadow = false;
		this->sideShadowWidth = 5;
		this->sideShadowMainAlpha = 160;
    }

    u32 Rectangle::GetX()
    {
        return this->x;
    }

    void Rectangle::SetX(u32 X)
    {
        this->x = X;
    }

    u32 Rectangle::GetY()
    {
        return this->y;
    }

    void Rectangle::SetY(u32 Y)
    {
        this->y = Y;
    }

    u32 Rectangle::GetWidth()
    {
        return this->w;
    }

    void Rectangle::SetWidth(u32 Width)
    {
        this->w = Width;
    }

    u32 Rectangle::GetHeight()
    {
        return this->h;
    }

    void Rectangle::SetHeight(u32 Height)
    {
        this->h = Height;
    }

    u32 Rectangle::GetBorderRadius()
    {
        return this->h;
    }

    void Rectangle::SetBorderRadius(u32 Radius)
    {
        this->borderr = Radius;
    }

    draw::Color Rectangle::GetColor()
    {
        return this->clr;
    }

    void Rectangle::SetColor(draw::Color RecColor)
    {
        this->clr = RecColor;
    }

	void Rectangle::SetDrawSideShadow(bool drawSideShadow, u32 sideShadowWidth, u32 sideShadowMainAlpha)
	{
		this->drawSideShadow = drawSideShadow;
		this->sideShadowWidth = sideShadowWidth;
		this->sideShadowMainAlpha = sideShadowMainAlpha;
	}
	
    void Rectangle::OnRender(render::Renderer *Drawer)
    {
        u32 rdx = this->GetProcessedX();
        u32 rdy = this->GetProcessedY();
        Drawer->RenderRectangleFill(this->clr, rdx, rdy, this->w, this->h);
		
		if (this->drawSideShadow)
			Drawer->RenderSideShadowSimple(this->x+this->w, this->y, this->sideShadowWidth, this->h, this->sideShadowMainAlpha);	
    }

    void Rectangle::OnInput(u64 Down, u64 Up, u64 Held, bool Touch, bool Focus)
    {
    }
}