#pragma once
#include "core/cansat.h"
// THE SOUNDS WILL PAUSE ANY OTHER ACTIONS ON BOARD
class Cansat;
class Sound
{
public:
    void error(Cansat &cansat);
    void init_success(Cansat &cansat);
    void locate(Cansat &cansat);
};
