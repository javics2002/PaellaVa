#pragma once
#include "ParticleSystem.h"
#include "../sdlutils/SDLUtils.h"
#include "../SDL2_image-2.0.1/include/SDL_image.h"

class ParticleExample : public ParticleSystem
{
public:
    ParticleExample() {}
    virtual ~ParticleExample() {}

    enum PatticleStyle
    {
        NONE,
        FIRE,
        FIRE_WORK,
        SUN,
        GALAXY,
        FLOWER,
        METEOR,
        SPIRAL,
        EXPLOSION,
        SMOKE,
        SNOW,
        RAIN
    };

    PatticleStyle style_ = NONE;
    void setStyle(PatticleStyle style);
    SDL_Texture* getDefaultTexture()
    {
        static SDL_Texture* t = sdlutils().images().at("particle").getTexture();
        //printf(SDL_GetError());
        return t;
    }
};
