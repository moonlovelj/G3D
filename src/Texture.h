#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Color.h"
#include <vector>

namespace g3dcommon
{
    static const int kMaxMipLevels = 14;

    enum SampleMethod
    {
        NEAREST,
        BILINEAR,
        TRILINEAR
    };

    struct MipLevel
    {
        size_t width;
        size_t height;
        std::vector<unsigned char> texels;
    };

    struct Texture
    {
        size_t width;
        size_t height;
        std::string name;
        std::vector<MipLevel> mipmap;
    };

    class Sampler2D
    {
    public:
        Sampler2D(SampleMethod method = NEAREST);
        ~Sampler2D();

        void GenerateMips(Texture &tex, int startLevel);

        Color SampleNearest(const Texture &tex, float u, float v, int level = 0);

        Color SampleBilinear(const Texture &tex, float u, float v, int level = 0);

    private:

        SampleMethod method;
    };

}

#endif