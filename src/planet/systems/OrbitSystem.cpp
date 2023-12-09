#include "systems/OrbitSystem.hpp"
#include "components/Components.hpp"

#include "core/Engine.hpp"
#include "core/Utils.hpp"

void OrbitSystem::onUpdate(float _dt)
{
    for(auto &&[entity, transformComp, orbitComp] : m_registryRef->view<Transform, Orbit>().each())
    {
        auto polarPos = convertToPolar(transformComp.position);

        // TODO implement the kepler orbit
        polarPos.phi += orbitComp.speed * _dt;
        transformComp.position = convertToCartesian(polarPos);
    }
}
