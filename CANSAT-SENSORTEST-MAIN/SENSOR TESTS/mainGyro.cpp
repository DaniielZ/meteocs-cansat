#include <Wire.h>
#include "I3G4250D.h"

I3G4250D i3g4250d;
I3G4250D_DataScaled i3g4250d_data = {0};
uint8_t I2C_I3G4250_SCL = 17;
uint8_t I2C_I3G4250_SDA = 16;
void setup()
{
    uint8_t id = 0;
    uint8_t error = -1;
    Serial.begin(115200);
    while (!Serial)
    {
        delay(100);
    }

    Wire.setSCL(I2C_I3G4250_SCL);
    Wire.setSDA(I2C_I3G4250_SDA);
    Serial.println("Gyroscope example");
    error = i3g4250d.I3G4250D_Init(0x0F, 0x00, 0x00, 0x00, 0x00, I3G4250D_SCALE_500);
    if (error != 0)
    {
        Serial.println("init fail");
    }
    i3g4250d.readRegister(0x0F, &id, 1);
    Serial.print("Gyroscope Device ID = ");
    Serial.println(id, HEX);
}

void loop()
{
    i3g4250d_data = i3g4250d.I3G4250D_GetScaledData();
    Serial.println();
    Serial.print("Gyroscope X(dps) = ");
    Serial.println(i3g4250d_data.x);
    Serial.print("Gyroscope Y(dps) = ");
    Serial.println(i3g4250d_data.y);
    Serial.print("Gyroscope Z(dps) = ");
    Serial.println(i3g4250d_data.z);
    delay(1000);
}