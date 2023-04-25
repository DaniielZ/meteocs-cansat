#include "sensors/sensor_manager.h"
void Sensor_manager::init(Cansat &cansat)
{
    // GPS UART0
    gps_serial = SoftwareSerial(cansat.config.GPS_RX, cansat.config.GPS_TX);
    gps_serial.begin(cansat.config.GPS_BAUDRATE);
    // MAGNETO WIRE0
    magneto_wire = TwoWire(i2c0, cansat.config.LIS2MDL_SDA, cansat.config.LIS2MDL_SCL);
    magneto = Adafruit_LIS2MDL(12345);
    magneto.enableAutoRange(true);
    if (!magneto.begin(30U, &magneto_wire))
    {
        cansat.log.error(cansat, "Magneto error");
    }
    // BARO WIRE1
    baro_wire = TwoWire(i2c1, cansat.config.MS5611_SDA, cansat.config.MS5611_SCL);
    baro = MS5611(cansat.config.MS5611_ADDRESS);
    if (!baro.begin(&baro_wire))
    {
        cansat.log.error(cansat, "Baro error");
    }
    // HUMIDITY WIRE1
    humidity_wire = TwoWire(i2c1, cansat.config.SHTC3_SDA, cansat.config.SHTC3_SCL);
    if (!humidity.begin(&humidity_wire))
    {
        cansat.log.error(cansat, "Humidity error");
    }

    // Log innit complete
    cansat.log.info("Sensor innit complete");
}

void Sensor_manager::read_gps()
{
    while (gps_serial.available() > 0)
    {
        gps.encode(gps_serial.read());
        if (gps.location.isUpdated())
        {
            data.gps_lat = gps.location.lat();
            data.gps_lng = gps.location.lng();
            data.gps_height = gps.altitude.meters();
            data.gps_sattelites = gps.satellites.value();
        }
    }
}
void Sensor_manager::read_magneto()
{
}
void Sensor_manager::read_baro()
{
}
void Sensor_manager::read_humidity()
{
}

void Sensor_manager::read_data()
{
    read_gps();
    read_magneto();
    read_baro();
    read_humidity();
}