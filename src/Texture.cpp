#include "Texture.h"
#include "Common.h"
#include <cmath>
#include <iostream>

namespace g3dcommon
{

    inline Color Sip(const MipLevel &tex, int x, int y)
    {
        x = std::min(std::max(0, x), static_cast<int>(tex.width - 1));
        y = std::min(std::max(0, y), static_cast<int>(tex.height - 1));
        int index = 4 * (y * tex.width + x);
        return Color(tex.texels[index], tex.texels[index + 1], tex.texels[index + 2], tex.texels[index + 3]);
    }

    Sampler2D::Sampler2D(SampleMethod method)
        : method(method)
    {

    }

    Sampler2D::~Sampler2D()
    {

    }

    void Sampler2D::GenerateMips(Texture &tex, int startLevel)
    {
        if (startLevel < 0 || startLevel >= tex.mipmap.size())
        {
            std::cerr << "Invalid start mip level" << std::endl;
        }

        size_t baseWidth = tex.mipmap[startLevel].width;
        size_t baseHeight = tex.mipmap[startLevel].height;
        int numSubLevels = (int) (log2f(std::max(baseWidth, baseHeight)));
        numSubLevels = std::min(numSubLevels, kMaxMipLevels - startLevel - 1);
        tex.mipmap.resize(size_t(startLevel + numSubLevels + 1));
        std::cout << tex.name << " mip levels : " << startLevel + numSubLevels + 1 << std::endl;

        size_t width = baseWidth;
        size_t height = baseHeight;
        for (size_t i = 1; i <= numSubLevels; ++i)
        {
            MipLevel &mipLevel = tex.mipmap[startLevel + i];
            width = size_t(std::max(1.f, width / 2.f));
            height = size_t(std::max(1.f, height / 2.f));
            mipLevel.width = width;
            mipLevel.height = height;
            mipLevel.texels = std::vector<unsigned char>(4 * width * height);
        }

        for (size_t i = 1; i < tex.mipmap.size(); ++i)
        {
            MipLevel &mipLevel = tex.mipmap[i];
            for (size_t j = 0; j < mipLevel.width * mipLevel.height; ++j)
            {
                size_t x = j % mipLevel.width;
                size_t y = j / mipLevel.width;
                Color c = 0.25f * Sip(tex.mipmap[i - 1], 2 * x, 2 * y)
                    + 0.25f * Sip(tex.mipmap[i - 1], 2 * x + 1, 2 * y)
                    + 0.25f * Sip(tex.mipmap[i - 1], 2 * x, 2 * y + 1)
                    + 0.25f * Sip(tex.mipmap[i - 1], 2 * x + 1, 2 * y + 1);
                mipLevel.texels[j * 4] = c.R();
                mipLevel.texels[j * 4 + 1] = c.G();
                mipLevel.texels[j * 4 + 2] = c.B();
                mipLevel.texels[j * 4 + 3] = c.A();
            }
        }
    }

    Color Sampler2D::SampleNearest(const Texture &tex, float u, float v, int level)
    {
        if (level < 0 || level >= tex.mipmap.size())
        {
            level = 0;
        }
        const MipLevel &mipTex = tex.mipmap[level];
        int x = static_cast<int>(std::floor((mipTex.width - 1) * u + 0.5f));
        int y = static_cast<int>(std::floor((mipTex.height - 1) * v + 0.5f));
        return Sip(mipTex, x, y);
    }

    Color Sampler2D::SampleBilinear(const Texture &tex, float u, float v, int level)
    {
        if (level < 0 || level >= tex.mipmap.size())
        {
            level = 0;
        }
        const MipLevel &mipTex = tex.mipmap[level];
        float x = (mipTex.width - 1) * u + 0.5f;
        float y = (mipTex.height - 1) * v + 0.5f;
        int sx = std::floor(x), sy = std::floor(y);
        float tx = x - sx, ty = y - sy;
        Color c1 = Interpolate(Sip(mipTex, sx, sy), Sip(mipTex, sx + 1, sy), tx);
        Color c2 = Interpolate(Sip(mipTex, sx, sy + 1), Sip(mipTex, sx + 1, sy + 1), tx);
        return Interpolate(c1, c2, ty);
    }

}
