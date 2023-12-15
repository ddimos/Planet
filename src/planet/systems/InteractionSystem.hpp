#pragma once

#include "systems/BaseSystem.hpp"
#include <vector>

class CollisionEvent;

class InteractionSystem : public BaseSystem
{
public:
    InteractionSystem() = default;

private:
    void onInit() override;
    void onUpdate(float _dt) override;

    void receiveCollisionEvent(const CollisionEvent& _event);

    struct InteractionPair
    {
        entt::entity entityA = entt::null;
        entt::entity entityB = entt::null;
        bool interactionStopped = false;
    };
    std::vector<InteractionPair> m_interactions;
};
