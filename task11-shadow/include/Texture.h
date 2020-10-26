#ifndef __TEXTURE_H__
#define __TEXTURE_H__


#include "stb_image.h"

#include <glad/glad.h>

class Texture
{
public:
    static unsigned int LoadTextureFromFile(const char* path);
};

#endif // !__TEXTURE_H__
