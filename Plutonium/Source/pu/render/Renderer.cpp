#include <pu/render/Renderer.hpp>

namespace pu::render
{
    NativeRenderer purend = NULL;

    void Renderer::Initialize()
    {
        if(!this->initialized)
        {
            Result rc = romfsInit();
            this->okromfs = (rc == 0);
            plInitialize();
            SDL_Init(SDL_INIT_EVERYTHING);
            this->rendwd = SDL_CreateWindow("Plutonium", 0, 0, 1280, 720, 0);
            purend = SDL_CreateRenderer(this->rendwd, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
            this->rendsf = SDL_GetWindowSurface(this->rendwd);
            SDL_SetRenderDrawBlendMode(purend, SDL_BLENDMODE_BLEND);
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
            IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF | IMG_INIT_WEBP);
            TTF_Init();
            this->initialized = true;
            this->basea = -1;
            this->basex = 0;
            this->basey = 0;
        }
    }

    void Renderer::Finalize()
    {
        if(this->initialized)
        {
            TTF_Quit();
            IMG_Quit();
            SDL_DestroyRenderer(purend);
            SDL_FreeSurface(this->rendsf);
            SDL_DestroyWindow(this->rendwd);
            SDL_Quit();
            plExit();
            if(this->okromfs) romfsExit();
            this->initialized = false;
        }
    }

    bool Renderer::HasInitialized()
    {
        return this->initialized;
    }

    bool Renderer::HasRomFs()
    {
        return this->okromfs;
    }

    void Renderer::InitializeRender(draw::Color Color)
    {
        SDL_SetRenderDrawColor(purend, Color.R, Color.G, Color.B, Color.A);
        SDL_RenderClear(purend);
    }

    void Renderer::FinalizeRender()
    {
        SDL_RenderPresent(purend);
    }

    void Renderer::RenderTexture(NativeTexture Texture, u32 X, u32 Y, int AlphaMod)
    {
        SDL_Rect pos;
        pos.x = X + this->basex;
        pos.y = Y + this->basey;
        if(AlphaMod >= 0) SetAlphaValue(Texture, (u8)AlphaMod);
        if(this->basea >= 0) SetAlphaValue(Texture, (u8)this->basea);
        SDL_QueryTexture(Texture, NULL, NULL, &pos.w, &pos.h);
        SDL_RenderCopy(purend, Texture, NULL, &pos);
    }

    void Renderer::RenderTextureScaled(NativeTexture Texture, u32 X, u32 Y, u32 Width, u32 Height, int AlphaMod)
    {
        SDL_Rect pos;
        pos.x = X + this->basex;
        pos.y = Y + this->basey;
        pos.w = Width;
        pos.h = Height;
        if(AlphaMod >= 0) SetAlphaValue(Texture, (u8)AlphaMod);
        if(this->basea >= 0) SetAlphaValue(Texture, (u8)this->basea);
        SDL_RenderCopyEx(purend, Texture, NULL, &pos, 0, NULL, SDL_FLIP_NONE);
    }

    void Renderer::RenderRectangle(draw::Color Color, u32 X, u32 Y, u32 Width, u32 Height)
    {
        SDL_Rect rect;
        rect.x = X + this->basex;
        rect.y = Y + this->basey;
        rect.w = Width;
        rect.h = Height;
        u8 alpha = Color.A;
        if(this->basea >= 0) alpha = (u8)this->basea;
        SDL_SetRenderDrawColor(purend, Color.R, Color.G, Color.B, alpha);
        SDL_RenderDrawRect(purend, &rect);
    }

    void Renderer::RenderRectangleFill(draw::Color Color, u32 X, u32 Y, u32 Width, u32 Height)
    {
        SDL_Rect rect;
        rect.x = X + this->basex;
        rect.y = Y + this->basey;
        rect.w = Width;
        rect.h = Height;
        u8 alpha = Color.A;
        if(this->basea >= 0) alpha = (u8)this->basea;
        SDL_SetRenderDrawColor(purend, Color.R, Color.G, Color.B, alpha);
        SDL_RenderFillRect(purend, &rect);
    }
	
    void Renderer::RenderRoundedRectangle(draw::Color Color, u32 X, u32 Y, u32 Width, u32 Height, u32 Radius)
    {
        u8 alpha = Color.A;
        if(this->basea >= 0) alpha = (u8)this->basea;
        roundedRectangleRGBA(purend, X + this->basex, Y + this->basey, X + this->basex + Width, Y + this->basey + Height, Radius, Color.R, Color.G, Color.B, alpha);
        SDL_SetRenderDrawBlendMode(purend, SDL_BLENDMODE_BLEND);
    }

    void Renderer::RenderRoundedRectangleFill(draw::Color Color, u32 X, u32 Y, u32 Width, u32 Height, u32 Radius)
    {
        u8 alpha = Color.A;
        if(this->basea >= 0) alpha = (u8)this->basea;
        roundedBoxRGBA(purend, X + this->basex, Y + this->basey, X + this->basex + Width, Y + this->basey + Height, Radius, Color.R, Color.G, Color.B, alpha);
        SDL_SetRenderDrawBlendMode(purend, SDL_BLENDMODE_BLEND);
    }

    void Renderer::RenderCircle(draw::Color Color, u32 X, u32 Y, u32 Radius)
    {
        u8 alpha = Color.A;
        if(this->basea >= 0) alpha = (u8)this->basea;
        circleRGBA(purend, X + this->basex, Y + this->basey, Radius - 1, Color.R, Color.G, Color.B, alpha);
        aacircleRGBA(purend, X + this->basex, Y + this->basey, Radius - 1, Color.R, Color.G, Color.B, alpha);
    }

    void Renderer::RenderCircleFill(draw::Color Color, u32 X, u32 Y, u32 Radius)
    {
        u8 alpha = Color.A;
        if(this->basea >= 0) alpha = (u8)this->basea;
        filledCircleRGBA(purend, X + this->basex, Y + this->basey, Radius - 1, Color.R, Color.G, Color.B, alpha);
        aacircleRGBA(purend, X + this->basex, Y + this->basey, Radius - 1, Color.R, Color.G, Color.B, alpha);
    }

    void Renderer::RenderShadowSimple(u32 X, u32 Y, u32 Width, u32 Height, u32 BaseAlpha, u8 MainAlpha)
    {
        bool crop = false;
        u32 shw = Width;
        u32 shx = X;
        u32 shy = Y;
        for(s32 al = BaseAlpha; al > 0; al -= (180 / Height))
        {
            this->RenderRectangleFill({ 130, 130, 130, (al * (MainAlpha / 255)) }, shx + this->basex, shy + this->basey, shw, 1);
            if(crop)
            {
                shw -= 2;
                shx++;
            }
            crop = !crop;
            shy++;
        }
    }
	
	void Renderer::RenderSideShadowSimple(u32 X, u32 Y, u32 Width, u32 Height, u32 BaseAlpha, u8 MainAlpha)
    {
        bool crop = false;
		u32 shh = Height;
        u32 shx = X;
        u32 shy = Y;
        for(s32 al = BaseAlpha; al > 0; al -= (180 / Width))
        {
            this->RenderRectangleFill({ 130, 130, 130, (al * (MainAlpha / 255)) }, shx + this->basex, shy + this->basey, 1, shh);
            if(crop)
            {
                shh -= 2;
				shy++;
            }
            crop = !crop;
            shx++;
        }
    }

    void Renderer::SetBaseRenderPosition(u32 X, u32 Y)
    {
        this->basex = X;
        this->basey = Y;
    }

    void Renderer::UnsetBaseRenderPosition()
    {
        this->SetBaseRenderPosition(0, 0);
    }

    void Renderer::SetBaseRenderAlpha(u8 Alpha)
    {
        this->basea = (int)Alpha;
    }

    void Renderer::UnsetBaseRenderAlpha()
    {
        this->basea = -1;
    }

    NativeRenderer GetMainRenderer()
    {
        return purend;
    }
}
