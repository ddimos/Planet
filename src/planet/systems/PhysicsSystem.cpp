#include "systems/PhysicsSystem.hpp"
#include "components/BodyComponent.hpp"
#include "components/CollidableComponent.hpp"
#include "components/TransformComponent.hpp"
#include "events/Events.hpp"
#include "core/Utils.hpp"
#include <iterator>

namespace
{
    struct CollisionPair;
    bool circleVsCircle(CollisionPair& _pair);

    struct CollisionPair
    {
        CollisionPair(
            entt::entity _A, Collidable& _collidableA, Transform& _transformA, Body* _bodyA,
            entt::entity _B, Collidable& _collidableB, Transform& _transformB, Body* _bodyB);

        bool canColide() const;
        bool isColliding();
        bool needToResolve() const;

        void resolveCollision();
        void correctPosition();

        bool needToNotify(const PhysicsSystem::NotificationPairs& _pairs) const;

        sf::Vector2f getPositionA() { return m_transformA.position; }
        float        getRadiusA() { return m_collidableA.radius; }
        sf::Vector2f getPositionB() { return m_transformB.position; }
        float        getRadiusB() { return m_collidableB.radius; }

        float         m_penetration = 0.0f;
        sf::Vector2f  m_normal = {0.0f, 0.0f};

    private:
        entt::entity  m_A;
        Collidable&   m_collidableA;
        Transform&    m_transformA;
        Body*         m_bodyA = nullptr;
        entt::entity  m_B;
        Collidable&   m_collidableB;
        Transform&    m_transformB;
        Body*         m_bodyB = nullptr;

    };

    CollisionPair::CollisionPair(
            entt::entity _A, Collidable& _collidableA, Transform& _transformA, Body* _bodyA,
            entt::entity _B, Collidable& _collidableB, Transform& _transformB, Body* _bodyB)
        : m_A(_A)
        , m_collidableA(_collidableA)
        , m_transformA(_transformA)
        , m_bodyA(_bodyA)
        , m_B(_B)
        , m_collidableB(_collidableB)
        , m_transformB(_transformB)
        , m_bodyB(_bodyB)
    {
    }

    bool CollisionPair::canColide() const
    {
        return m_collidableA.canColideWithFlags & m_collidableB.typeFlag 
            && m_collidableB.canColideWithFlags & m_collidableA.typeFlag;
    }

    bool CollisionPair::isColliding()
    {
        // only a circle shape
        return circleVsCircle(*this);
    }

    bool CollisionPair::needToResolve() const
    {
        return m_collidableA.needToResolve || m_collidableB.needToResolve;
    }    

    void CollisionPair::resolveCollision()
    {
        sf::Vector2f velocityA{0.f, 0.f};
        sf::Vector2f velocityB{0.f, 0.f};

        if (m_bodyA)
            velocityA = m_bodyA->velocity;
        if (m_bodyB)
            velocityB = m_bodyB->velocity;

        sf::Vector2f const velFromAtoB = velocityB - velocityA;
        float const velAlongN = dotProduct(velFromAtoB, m_normal);

        // moving away each other
        if (velAlongN > 0.0f)
            return;

        // restitution
        float const e = 0.8f;
        float const j = -(1 - e) * velAlongN / 1.f;//(A->invMass + B->invMass);
        sf::Vector2f const impulse = j * m_normal;
        if (m_bodyA)
        {
            m_bodyA->velocity -= impulse;//A->invMass * impulse;
            m_bodyA->velocity *= 0.999f;
        }
        if (m_bodyB)
        {
            m_bodyB->velocity += impulse;//A->invMass * impulse;
            m_bodyB->velocity *= 0.999f;
        }
    }

    void CollisionPair::correctPosition()
    {
        const float percent = 0.4f;
        const float kSlop = 0.05f;
        const float invMassSum = 1.f; // m_A->invMass + m_B->invMass
        sf::Vector2f const correction = (std::max(m_penetration - kSlop, 0.0f) / (invMassSum)) * percent * m_normal;
        
        const bool correctForBoth = m_collidableA.needToResolve && m_collidableB.needToResolve;
        if (m_collidableA.needToResolve)
            m_transformA.position -= correction * ((correctForBoth)?1.f:2.f) ;//m_A->invMass * correction;
        if (m_collidableB.needToResolve)
            m_transformB.position += correction* ((correctForBoth)?1.f:2.f);//A->invMass * correction;
    }

    bool CollisionPair::needToNotify(const PhysicsSystem::NotificationPairs& _pairs) const
    {
        auto itA = _pairs.find(m_collidableA.typeFlag);
        if (itA != _pairs.end())
            return itA->second & m_collidableB.typeFlag;
        
        auto itB = _pairs.find(m_collidableB.typeFlag);
        if (itB != _pairs.end())
            return itB->second & m_collidableA.typeFlag;
        
        return false;
    }

    bool circleVsCircle(CollisionPair & _pair)
    {
        sf::Vector2f fromAtoB = _pair.getPositionA() - _pair.getPositionB();
        
        float r = _pair.getRadiusA() + _pair.getRadiusB();
        float dist = vectorLength(fromAtoB);
        
        if (r <= dist)
            return false;

        if (dist == 0.f)
        {
            _pair.m_penetration = std::max(_pair.getRadiusA(), _pair.getRadiusB());
            _pair.m_normal = {1.f, 0.f};
        }
        else
        {
            _pair.m_penetration = r - dist;
            _pair.m_normal = -fromAtoB / dist;
        }
        
        return true;
    }
} // namespace

PhysicsSystem::PhysicsSystem(NotificationPairs&& _notificationPairs)
    : m_notificationPairs(move(_notificationPairs))
{
}

void PhysicsSystem::onUpdate(float _dt)
{
    std::vector<CollisionPair> collidingPairs;

    auto view = m_registryRef->view<Transform, Collidable>();
    for(auto it = view.begin(), last = view.end(); it != last; ++it) 
    {
        auto entityA = *it;
        auto& transformA = view.get<Transform>(entityA);
        auto& collidableA = view.get<Collidable>(entityA);
        Body* bodyA = (m_registryRef->all_of<Body>(entityA)) ? &m_registryRef->get<Body>(entityA) : nullptr;

        for(auto it2 = std::next(it); it2 != view.end(); ++it2) 
        {
            auto entityB = *it2;

            auto& transformB = view.get<Transform>(entityB);
            auto& collidableB = view.get<Collidable>(entityB);
            Body* bodyB = (m_registryRef->all_of<Body>(entityB)) ? &m_registryRef->get<Body>(entityB) : nullptr;

            CollisionPair pair(entityA, collidableA, transformA, bodyA, entityB, collidableB, transformB, bodyB);
            if (!pair.canColide())
                continue;
            if (!pair.isColliding())
                continue;

            if (pair.needToNotify(m_notificationPairs))
                m_dispatcherRef->trigger<CollisionEvent>({entityA, entityB});

            if (!pair.needToResolve())
                continue;

            collidingPairs.push_back(pair);
        }
    }

    for (auto& pair : collidingPairs)
    {
        for (size_t i = 0; i < 10; i++)
            pair.resolveCollision();
    }

    for(auto &&[entity, transform, body] : m_registryRef->view<Transform, Body>().each())
    {
        float invMass = 1.f;
        body.velocity += body.force * invMass * _dt;
        transform.position += body.velocity * _dt;

        body.force = {};
    }

    for (auto& pair : collidingPairs)
    {
        pair.correctPosition();
    }
}
