#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <SparkFunMLX90614.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     1000
#define PERSON_SENSOR D5
float tempAmbiente;
float tempObjeto;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
IRTherm sensor;


// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()

{
    Serial.begin(9600);

    Serial.print("Initializing pulse oximeter..");

    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    // The default current for the IR LED is 50mA and it could be changed
    //   by uncommenting the following line. Check MAX30100_Registers.h for all the
    //   available options.
    // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);

    //Serial.println("Beat!");
}



/*****************************************/
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  //Serial.println("Adafruit MLX90614 test");
  sensor.begin(0x5B);
  sensor.setUnit(TEMP_C);//podemos ainda utilizar TEMP_F para Fahrenheit                                                           //ou TEMP_K para Kelvin
  mlx.begin();
}

void loop() {
	    // Make sure to call update as fast as possible
    pox.update();

    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        if(pox.getHeartRate()>0.0 && pox.getSpO2() >0)
        {
        //Serial.print("Heart rate:");
        //Serial.println(pox.getHeartRate());
        //Serial.print("bpm / SpO2:");
        Serial.println(pox.getSpO2());
        //Serial.println("%");
        //delay(5000);
        }
        tsLastReport = millis();

	
  //Serial.println(digitalRead(PERSON_SENSOR));// 0 person has come
  delay(1000);

  if (sensor.read())
  {
    tempAmbiente = sensor.ambient();
    tempObjeto = sensor.object();
    if (tempObjeto > 28)
    {
      
      //Serial.print("Ambient = "); Serial.print(tempAmbiente);
      //Serial.print("*C\tObject = "); 
      Serial.println(tempObjeto); //Serial.println("*C");
      //  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF());
      //  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");
      //Serial.println();
    delay(5000);
    }
  }
}
}