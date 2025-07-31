#pragma once

namespace Scarlett
{

class Texture
{
public:
    virtual ~Texture() = default;

    virtual void Create(const Filepath& filepath) = 0;
    virtual void Create(const uint8* buffer, const uint32 width, const uint32 height) = 0;
protected:
    uint32 mWidth  { 0 };
    uint32 mHeight { 0 };
};

} // Namespace Scarlett.