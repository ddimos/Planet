#include "systems/ChildSystem.hpp"

#include "components/ChildOfEntityComponent.hpp"
#include "components/TransformComponent.hpp"

#include "core/Utils.hpp"

void ChildSystem::onUpdate(float _dt)
{
    for(auto &&[entity, transformComp, childComp] : m_registryRef->view<Transform, ChildOfEntity>().each())
    {
        auto& parentTransform = m_registryRef->get<Transform>(childComp.parent);
        transformComp.position = rotateVector(childComp.offset, parentTransform.rotation) + parentTransform.position;
        transformComp.rotation = parentTransform.rotation;
    }
}
