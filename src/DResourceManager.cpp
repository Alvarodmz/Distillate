#include "DResourceManager.hpp"
#include "DTextureResource.hpp"
#include "DResource.hpp"

#if defined(GL_RENDER)
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <png.h>
#endif

#include "DUtils.hpp"

namespace Distillate {
    DTextureLoader *DResourceManager::textureLoader = new DTextureLoader();

    DResourceManager::DResourceManager() {}
    DResourceManager::~DResourceManager() {}

    bool DResourceManager::loadTexture(const std::string &filename)
    {
        return loadTexture(filename, filename);
    }

    bool DResourceManager::loadTexture(const std::string &filename, const std::string &resourceid)
    {
        if(_resources[resourceid])
        {
#ifdef DEBUG 
        fprintf(stdout,"Loading from buffer %s, with id %s\n", filename.c_str(), resourceid.c_str()); 
#endif
            return true;
        }

#ifdef DEBUG 
        fprintf(stdout,"Trying load texture %s, with id %s\n", filename.c_str(), resourceid.c_str()); 
#endif

        DTextureResource *texRes =  new DTextureResource(filename, resourceid); 
        if(!texRes) 
        {
            fprintf(stderr, "Cannot alloc texRes\n");
            return false;
        }

        switch(DTextureLoader::checkTexture(texRes))
        {
            case DTextureLoader::PNG_TEXTURE:
                textureLoader->impl = new DPNGTextureImplementation();
                textureLoader->impl->process(texRes);
                break;
            default:
                fprintf(stderr, "Unknow type\n");
                return false;
                break;
        }

        _resources[resourceid] = texRes;
        return true;
    }

    bool DResourceManager::createTexture(unsigned int width, unsigned int height, unsigned int color, const std::string &resourceid)
    {
        if(_resources[resourceid])
        {
#ifdef DEBUG 
        fprintf(stdout,"Loading from buffer %s\n",  resourceid.c_str()); 
#endif
            return true;
        }

#ifdef DEBUG
        fprintf(stdout, "Creating texture\n");
#endif
        

        DTextureResource *texRes = new DTextureResource("created",resourceid);
        if(!texRes) 
        {
            fprintf(stderr, "Cannot alloc texRes\n");
            return false;
        }

        texRes->w = width;
        texRes->h = height;

        textureLoader->impl = new DAutoTextureImplementation();
        textureLoader->impl->process(texRes);
        _resources[resourceid] = texRes;
        return true;
    }

    DTextureResource *DResourceManager::texture(const std::string& resourceid)
    {
        return static_cast<DTextureResource*>(_resources[resourceid]);
    }
}
