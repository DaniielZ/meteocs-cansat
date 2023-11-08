#include <core/cansat.h>

// Initialize SPI/I2C/PC Serial/Hardware Serial communication
void Cansat::init_all_com_bus(Config &config)
{
    SPI.setRX(config.SPI0_RX);
    SPI.setTX(config.SPI0_TX);
    SPI.setSCK(config.SPI0_SCK);
    SPI.begin();

    SPI1.setRX(config.SPI1_RX);
    SPI1.setTX(config.SPI1_TX);
    SPI1.setSCK(config.SPI1_SCK);
    SPI1.begin();

    Wire.setSCL(config.WIRE0_SCL);
    Wire.setSDA(config.WIRE0_SDA);

    // Wire1.setSCL(config.WIRE1_SCL);
    // Wire1.setSDA(config.WIRE1_SDA);

    // Initialize PC serial
    Serial.begin(config.PC_BAUDRATE);
    if (config.WAIT_PC)
    {
        while (!Serial)
        {
            delay(500);
        }
    }

    Serial1.setRX(config.SERIAL1_RX);
    Serial1.setTX(config.SERIAL1_TX);
    Serial1.setFIFOSize(256); // once had a problem of not reading serial properly but this seemed to fix it
    Serial1.begin(config.SERIAL1_BAUDRATE);
}

void Cansat::start_states(Cansat &cansat)
{
    current_state = State::PREP;
    prepare_state(cansat);
    current_state = State::ASCENT;
    ascent_state(cansat);
    current_state = State::DESCENT;
    descent_state(cansat);
}
