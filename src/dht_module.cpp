// Module: dht_module.cpp
// Purpose: Handles initialization and data acquisition from DHT temperature and humidity sensors.
// Functions:
// - setupDHT(): Initializes all connected DHT sensors.
// - readDHT(): Reads temperature and humidity values from each DHT sensor and passes them by reference.


#include "dht_module.h"

// Initialize DHT sensors at the defined pins
DHT dht1(DHTPIN_1, DHTTYPE);
DHT dht2(DHTPIN_2, DHTTYPE);
DHT dht3(DHTPIN_3, DHTTYPE);
DHT dht4(DHTPIN_4, DHTTYPE);
DHT dht5(DHTPIN_5, DHTTYPE);

// Function to initialize DHT sensors
void setupDHT() {
    dht1.begin();
    dht2.begin();
    dht3.begin();
    dht4.begin();
    dht5.begin();
}

// Function to read temperature and humidity from DHT sensors
void readDHT(float &temp1, float &hum1, float &temp2, float &hum2, float &temp3, float &hum3, float &temp4, float &hum4, float &temp5, float &hum5) {
    temp1 = dht1.readTemperature();
    hum1 = dht1.readHumidity();
    temp2 = dht2.readTemperature();
    hum2 = dht2.readHumidity();
    temp3 = dht3.readTemperature();
    hum3 = dht3.readHumidity();
    temp4 = dht4.readTemperature();
    hum4 = dht4.readHumidity();
    temp5 = dht5.readTemperature();
    hum5 = dht5.readHumidity();

    // Optional: Output sensor values for debugging
    /*
    Serial.printf("DHT Sensor Values:\n Temp1: %.2f, Hum1: %.2f\n Temp2: %.2f, Hum2: %.2f\n Temp3: %.2f, Hum3: %.2f\n Temp4: %.2f, Hum4: %.2f\n Temp5: %.2f, Hum5: %.2f\n",
                  temp1, hum1, temp2, hum2, temp3, hum3, temp4, hum4, temp5, hum5);
    */
}
