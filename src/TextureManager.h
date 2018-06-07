#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include <map>

namespace g3dcommon
{
  struct Texture;
  /**
  * Texture manager.
  */
  class TextureManager
  {
  private:
    TextureManager() { }
  public:
    static TextureManager& GetInstance()
    {
      static TextureManager instance;
      return instance;
    }

    int LoadTexture(const std::string& pathName, const std::string& textureName);

    Texture* GetTexture(int textureId);

  private:
    std::map<size_t, Texture*> textures;
  };
}

#endif