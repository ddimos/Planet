#include "systems/InteractionSystem.hpp"
#include "events/Events.hpp"
#include "components/InteractableComponent.hpp"
#include "core/Log.hpp"

#include <algorithm>

void InteractionSystem::onInit()
{
    m_dispatcherRef->sink<CollisionEvent>().connect<&InteractionSystem::receiveCollisionEvent>(this);
}

void InteractionSystem::onUpdate(float _dt)
{
    for(auto interactionIt = m_interactions.begin(); interactionIt != m_interactions.end();)
    {
        if(interactionIt->interactionStopped)
        {
            LOG_DEBUG("Interaction finished");
            m_dispatcherRef->trigger<InteractionEvent>(
                { interactionIt->entityA, interactionIt->entityB,
                InteractionEvent::InteractionState::FINISHED }
            );
            interactionIt = m_interactions.erase(interactionIt); 
            continue;
        }
        
        interactionIt->interactionStopped = true;
        ++interactionIt;
    }
}

void InteractionSystem::receiveCollisionEvent(const CollisionEvent& _event)
{
    if (!m_registryRef->all_of<Interactable>(_event.entityA) || !m_registryRef->all_of<Interactable>(_event.entityB))
        return;
    
    Interactable& interactableA = m_registryRef->get<Interactable>(_event.entityA);
    Interactable& interactableB = m_registryRef->get<Interactable>(_event.entityB);

    if ( !(interactableA.canInteractWithFlags & interactableB.typeFlag) || !(interactableB.canInteractWithFlags & interactableA.typeFlag))
        return;
    
    auto it = std::find_if( m_interactions.begin(), m_interactions.end(), 
        [&_event](const InteractionPair& _interaction) {
            return _interaction.entityA == _event.entityA && _interaction.entityB == _event.entityB
                || _interaction.entityA == _event.entityB && _interaction.entityB == _event.entityA;
    });

    if (it != m_interactions.end())
    {
        it->interactionStopped = false;
    }
    else
    {
        LOG_DEBUG("Interaction started");
        m_dispatcherRef->trigger<InteractionEvent>(
            { _event.entityA, _event.entityB, InteractionEvent::InteractionState::STARTED }
        );
        m_interactions.push_back({ _event.entityA, _event.entityB, false });
    }

    // TO USE IT WHEN NEEDED
    //
    // auto itA = std::find_if( interactableA.interactions.begin(), interactableA.interactions.end(), 
    //     [&_event](const Interactable::Interaction& _interaction) {
    //         return _interaction.entityIntereactWith == _event.entityB;
    // });
    // auto itB = std::find_if( interactableB.interactions.begin(), interactableB.interactions.end(),
    //     [&_event](const Interactable::Interaction& _interaction) {
    //         return _interaction.entityIntereactWith == _event.entityA;
    // });

    // if (itA != interactableA.interactions.end() && itB != interactableB.interactions.end())
    // {
    //     itB->interactionStopped = false;

    // }
    // else if (itA == interactableA.interactions.end() && itB == interactableB.interactions.end())
    // {
    //     LOG("Interaction started");
    //     m_dispatcherRef->trigger<InteractionEvent>(
    //         { _event.entityA, _event.entityB, InteractionEvent::InteractionState::STARTED }
    //     );

    //     interactableA.interactions.push_back( { _event.entityB, false} );
    //     interactableB.interactions.push_back( { _event.entityA, false} );
    // }
    // else
    // {
    //     assert(false && "Mismatch between interactable entities.");
    // }
}
