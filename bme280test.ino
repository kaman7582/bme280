#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>



#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

unsigned long delayTime;

#define Addr 0x76

void setup() {
    Serial.begin(112500);
    Serial.println("Bme280 init");

    bool status;
    
    // init the bme object address shoule be 0x76
    status = bme.begin(Addr);
    if (!status) {
        Serial.println("Can not find the Bme280 sensor, check the right I2C address");
        while (1);
    }
    Serial.println("start testing");
    delayTime = 1000;
    Serial.println("-------------");
}

void sensor_read() {
    Serial.print(" Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
    
    Serial.print(" Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
    
    Serial.print(" Pressure = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print(" Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");
    Serial.println("-------------");
}

void loop() { 
    sensor_read();
    delay(delayTime);
}
