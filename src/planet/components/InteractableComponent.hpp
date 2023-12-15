#pragma once

#include <cstdint>

using InteractableType = uint16_t;
struct Interactable
{
    InteractableType typeFlag = 0;
    InteractableType canInteractWithFlags = 0;
};
