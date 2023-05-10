#pragma once

#include "systems/BaseSystem.hpp"
#include "core/Random.h"

class AsteroidSystem : public BaseSystem
{
public:
    AsteroidSystem();

private:
    void onUpdate(float _dt) override;
    
    void checkSpawn(float _dt);
    float getNewSpawnFrequency();
    void spawn();

    Random m_randomizer;

    const float m_maxDistanceFromPlanet = 0.f; 
    const float m_minDistanceFromPlanet = 0.f;
    const float m_maxSpawnFrequencyS = 0.f; 
    const float m_minSpawnFrequencyS = 0.f;
   
    float m_spawnTimeS = 0.;
    bool m_isPressed = false;
};
