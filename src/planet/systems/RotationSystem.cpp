#include "systems/RotationSystem.hpp"
#include "components/RotationComponent.hpp"
#include "components/TransformComponent.hpp"

#include "core/Engine.hpp"
#include "core/Utils.hpp"

void RotationSystem::onUpdate(float _dt)
{
    for(auto &&[entity, transformComp, rotComp] : m_registryRef->view<Transform, Rotation>().each())
    {
        transformComp.rotation += rotComp.speed * _dt;
        transformComp.rotation = normalizeAngle(transformComp.rotation);
    }
}
