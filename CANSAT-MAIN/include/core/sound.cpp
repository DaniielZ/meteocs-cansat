#include "core/sound.h"
void Sound::error(Cansat &cansat)
{
    for (int i = 0; i < cansat.config.BUZZER_ERROR_BEEPS; i++)
    {
        digitalWrite(cansat.config.BUZZER, HIGH);
        delay(cansat.config.BUZZER_SHORT_BEEP);
        digitalWrite(cansat.config.BUZZER, LOW);
        delay(cansat.config.BUZZER_SHORT_PAUSE);
    }
}
void Sound::init_success(Cansat &cansat)
{
    for (int i = 0; i < 5; i++)
    {
        digitalWrite(cansat.config.BUZZER, HIGH);
        delay(cansat.config.BUZZER_LONG_BEEP);
        digitalWrite(cansat.config.BUZZER, LOW);
        delay(cansat.config.BUZZER_LONG_PAUSE);
    }
    for (int i = 0; i < 5; i++)
    {
        digitalWrite(cansat.config.BUZZER, HIGH);
        delay(cansat.config.BUZZER_SHORT_BEEP);
        digitalWrite(cansat.config.BUZZER, LOW);
        delay(cansat.config.BUZZER_SHORT_PAUSE);
    }
}
void Sound::locate(Cansat &cansat)
{
    digitalWrite(cansat.config.BUZZER, HIGH);
    delay(cansat.config.BUZZER_LONG_BEEP);
    digitalWrite(cansat.config.BUZZER, LOW);
    delay(cansat.config.BUZZER_LONG_PAUSE);
}