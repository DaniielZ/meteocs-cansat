#pragma once

// THE SOUNDS WILL PAUSE ANY OTHER ACTIONS ON BOARD
class Config;
class Sound
{
public:
    void error(Config &config);
    void init_success(Config &config);
    void locate(Config &config);
};
