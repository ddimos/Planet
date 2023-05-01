#pragma once
#include "systems/BaseSystem.hpp"
#include "components/Components.hpp"
#include <unordered_map>

class PhysicsSystem : public BaseSystem
{
public:
    using NotificationPairs = std::unordered_map<CollidableType, CollidableType>;

    PhysicsSystem(NotificationPairs&& _notificationPairs);

private:
    void onUpdate(float _dt) override;

    const NotificationPairs m_notificationPairs;
};
