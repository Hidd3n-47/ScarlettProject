#pragma once

namespace Scarlett
{

class Texture
{
public:
    Texture()           = default;
    virtual ~Texture()  = default;

    Texture(const Texture&)               = delete;
    Texture(Texture&&)                    = delete;
    Texture& operator=(Texture&&)         = delete;
    Texture& operator=(const Texture&)    = delete;
protected:
    uint32 mWidth  { 0 };
    uint32 mHeight { 0 };

    virtual void Create(const uint8* buffer, const uint32 width, const uint32 height) = 0;
};

} // Namespace Scarlett.