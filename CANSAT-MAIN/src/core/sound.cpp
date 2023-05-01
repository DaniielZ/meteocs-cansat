#include "core/sound.h"
#include "config.h"
void Sound::error(Config &config)
{
    for (int i = 0; i < config.BUZZER_ERROR_BEEPS; i++)
    {
        digitalWrite(config.BUZZER, HIGH);
        delay(config.BUZZER_SHORT_BEEP);
        digitalWrite(config.BUZZER, LOW);
        delay(config.BUZZER_SHORT_PAUSE);
    }
}
void Sound::init_success(Config &config)
{
    for (int i = 0; i < 5; i++)
    {
        digitalWrite(config.BUZZER, HIGH);
        delay(config.BUZZER_LONG_BEEP);
        digitalWrite(config.BUZZER, LOW);
        delay(config.BUZZER_LONG_PAUSE);
    }
    for (int i = 0; i < 5; i++)
    {
        digitalWrite(config.BUZZER, HIGH);
        delay(config.BUZZER_SHORT_BEEP);
        digitalWrite(config.BUZZER, LOW);
        delay(config.BUZZER_SHORT_PAUSE);
    }
}
void Sound::locate(Config &config)
{
    digitalWrite(config.BUZZER, HIGH);
    delay(config.BUZZER_LONG_BEEP);
    digitalWrite(config.BUZZER, LOW);
    delay(config.BUZZER_LONG_PAUSE);
}