// Signal K application template file.
//
// This application demonstrates core SensESP concepts in a very
// concise manner. You can build and upload the application as is
// and observe the value changes on the serial port monitor.
//
// You can use this source file as a basis for your own projects.
// Remove the parts that are not relevant to you, and add your own code
// for external hardware libraries.

#include "sensesp/sensors/digital_input.h"
#include "sensesp/sensors/sensor.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp_app_builder.h"

using namespace sensesp;

reactesp::ReactESP app;

// The setup function performs one-time application initialization.
void setup() {
#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  // Construct the global SensESPApp() object
  SensESPAppBuilder builder;
  sensesp_app = (&builder)
                    // Set a custom hostname for the app.
                    ->set_hostname("my-sensesp-digital")
                    // Optionally, hard-code the WiFi and Signal K server
                    // settings. This is normally not needed.
                    ->set_wifi("Hill_Grownups_IoT", "CuckouCuckou")
                    //->set_sk_server("192.168.10.3", 80)
                    ->get_app();


  // Create another digital input, this time with RepeatSensor. This approach
  // can be used to connect external sensor library to SensESP!
  const uint8_t kDigitalInput1Pin = 12;
  const uint8_t kDigitalInput2Pin = 13;
  const unsigned int kDigitalInputInterval = 1000;

  // Configure the pin. Replace this with your custom library initialization
  // code!
  pinMode(kDigitalInput1Pin, INPUT_PULLUP);

  // Define a new RepeatSensor that reads the pin every 100 ms.
  // Replace the lambda function internals with the input routine of your custom
  // library.
//ReapeatSensor 1
  auto* digital_input1 = new RepeatSensor<bool>(
      kDigitalInputInterval,
      [kDigitalInput1Pin]() { return digitalRead(kDigitalInput1Pin); });


  // Connect digital input 1 to Signal K output.
  digital_input1->connect_to(new SKOutputBool(
      "sensors.digital_input1.value",          // Signal K path
      "/sensors/digital_input1/value",         // configuration path
      new SKMetadata("",                       // No units for boolean values
                     "Digital input 1 value")  // Value description
      ));
//ReapeatSensor 2
  auto* digital_input2 = new RepeatSensor<bool>(
      kDigitalInput2Interval,
      [kDigitalInput2Pin]() { return digitalRead(kDigitalInput2Pin); });


  // Connect digital input 2 to Signal K output.
  digital_input2->connect_to(new SKOutputBool(
      "sensors.digital_input2.value",          // Signal K path
      "/sensors/digital_input2/value",         // configuration path
      new SKMetadata("",                       // No units for boolean values
                     "Digital input 2 value")  // Value description
      ));

  // Start networking, SK server connections and other SensESP internals
  sensesp_app->start();
}

void loop() { app.tick(); }
