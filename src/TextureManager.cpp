#include "TextureManager.h"
#include "Texture.h"
#include "tga.h"
#include <iostream>

namespace g3dcommon
{
  int TextureManager::LoadTexture(const std::string& pathName, const std::string& textureName)
  {
    if ("" == textureName)
    {
      return -1;
    }

    for (auto it : textures)
    {
      if (it.second->name == textureName)
      {
        return it.first;
      }
    }

    TGA* pTgafile = TGAOpen(const_cast<char *>((pathName + textureName).c_str()), (char *)"rb");
    if (pTgafile == nullptr || pTgafile->last != TGA_OK)
    {
      TGAClose(pTgafile);
      std::cout << "Open TGA file: " << textureName.c_str() << " failed !" << std::endl;
      return -1;
    }
    TGAData tgaData;
    tgaData.flags = TGA_IMAGE_DATA | TGA_IMAGE_ID | TGA_RGB;
    if (TGAReadImage(pTgafile, &tgaData) != TGA_OK)
    {
      TGAClose(pTgafile);
      std::cout << "Read TGA image failed !" << std::endl;
      return -1;
    }

    Texture * texture = new Texture();
    texture->mipmap.resize(1);
    texture->name = textureName;
    texture->mipmap[0].width = texture->width = pTgafile->hdr.width;
    texture->mipmap[0].height = texture->height = pTgafile->hdr.height;
    size_t colorChannels = pTgafile->hdr.depth / 8;
    size_t colorNum = texture->width * texture->height;
    texture->mipmap[0].texels.resize(4 * colorNum);
    if (colorChannels == 3)
    {
      for (size_t i = 0; i < colorNum; i++)
      {
        memcpy(&texture->mipmap[0].texels[i * 4], &tgaData.img_data[i * 3], 3);
        texture->mipmap[0].texels[i * 4 + 3] = 255;
      }
    }
    else if (colorChannels == 4)
    {
      memcpy(&texture->mipmap[0].texels[0], tgaData.img_data, 4 * texture->width * texture->height);
    }
    else
    {
      std::cout << "TGA image depth error !" << std::endl;
      delete texture;
      return -1;
    }
    Sampler2D sample2D;
    sample2D.GenerateMips(*texture,0);
    size_t textureId = textures.size();
    textures.insert(std::make_pair(textureId, texture));
    return textureId;
  }

  Texture* TextureManager::GetTexture(int texId)
  {
    auto it = textures.find(texId);
    if (textures.end() != it)
    {
      return it->second;
    }
    return nullptr;
  }
}