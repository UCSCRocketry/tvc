#include <Arduino.h>
#include <Adafruit_BNO08x.h>

bool SIMULATE_IMU = true; // test code without hardware

Adafruit_BNO08x bno08x(-1);
sh2_SensorValue_t sensorValue; // Variable to hold sensor data

bool initIMUPipeline()
{
    if (SIMULATE_IMU)
    {
        Serial.println("Initialized in MOCK mode.");
        return true;
    }

    // I2C initialization (Teensy 4.1 default pins: SDA0=18, SCL0=19)
    if (!bno08x.begin_I2C()) // starts communication with the sensor over I2C
    {
        return false;
    }

    if (!bno08x.enableReport(SH2_GAME_ROTATION_VECTOR)) // provides real, i, j, k quaternion values
    {
        return false;
    }

    Serial.println("[Plumbing] BNO085 Hardware Connected & Configured.");
    return true;
}

bool readIMUPipeline(float &real, float &i, float &j, float &k)
{
    if (SIMULATE_IMU)
    {
        // Simulate a quaternion rotation over time
        static float t = 0.0f;
        t += 0.05f;
        real = cos(t / 2.0f);
        i = 0.0f;
        j = 0.0f;
        k = sin(t / 2.0f);
        delay(50);
        return true; 
    }

    if (bno08x.getSensorEvent(&sensorValue)) //checks if new data is gathered from the sensor
    {
        if (sensorValue.sensorId == SH2_GAME_ROTATION_VECTOR)
        {
            real = sensorValue.un.gameRotationVector.real;
            i = sensorValue.un.gameRotationVector.i;
            j = sensorValue.un.gameRotationVector.j;
            k = sensorValue.un.gameRotationVector.k;
            return true;
        }
    }
    return false;
}

void setup()
{
    Serial.begin(115200);
    while (!Serial && millis() < 3000)
        ;

    Serial.println("--- System Booting ---");

    if (!initIMUPipeline())
    {
        Serial.println("ERROR: IMU Pipeline failed to start!");
        while (1)
        {
            delay(10);
        }
    }
}

void loop()
{
    float qReal, qI, qJ, qK;

    //Prints the quaternion values to the serial monitor if new data is available
    if (readIMUPipeline(qReal, qI, qJ, qK))
    {
        Serial.print("Pipeline Stream -> Real: ");
        Serial.print(qReal, 3);
        Serial.print("\tI: ");
        Serial.print(qI, 3);
        Serial.print("\tJ: ");
        Serial.print(qJ, 3);
        Serial.print("\tK: ");
        Serial.println(qK, 3);
    }
}
