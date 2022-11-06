// Signal K application template file.
//
// This application demonstrates core SensESP concepts in a very
// concise manner. You can build and upload the application as is
// and observe the value changes on the serial port monitor.
//
// You can use this source file as a basis for your own projects.
// Remove the parts that are not relevant to you, and add your own code
// for external hardware libraries.

#include "sensesp/sensors/analog_input.h"
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
                    ->set_hostname("my-sensesp")
                    // Optionally, hard-code the WiFi and Signal K server
                    // settings. This is normally not needed.
                    ->set_wifi("HillGrownup_IoT", "CuckouCuckou")
                    //->set_sk_server("192.168.10.3", 80)
                    ->get_app();

  // GPIO number to use for the analog input
  const uint8_t kAnalogInputPin1 = 23;
  const uint8_t kAnalogInputPin2 = 22;
  const uint8_t kAnalogInputPin3 = 21;
  // Define how often (in milliseconds) new samples are acquired
  const unsigned int kAnalogInputReadInterval = 500;
  // Define the produced value at the maximum input voltage (3.3V).
  // A value of 3.3 gives output equal to the input voltage.
  const float kAnalogInputScale = 3.3;

  // Create a new Analog Input Sensor that reads an analog input pin
  // periodically.
  auto* analog_input1 = new AnalogInput(
      kAnalogInputPin1, kAnalogInputReadInterval, "", kAnalogInputScale);
  auto* analog_input2 = new AnalogInput(
      kAnalogInputPin2, kAnalogInputReadInterval, "", kAnalogInputScale);
  auto* analog_input3 = new AnalogInput(
      kAnalogInputPin3, kAnalogInputReadInterval, "", kAnalogInputScale);

  // Add an observer that prints out the current value of the analog input
  // every time it changes.
  analog_input1->attach([analog_input1]() {
    debugD("Analog input 1 value: %f", analog_input1->get());
  });
  analog_input2->attach([analog_input2]() {
    debugD("Analog input 2 value: %f", analog_input2->get());
  });
  analog_input3->attach([analog_input3]() {
    debugD("Analog input 3 value: %f", analog_input3->get());
  });
  
  // Connect the analog input to Signal K output. This will publish the
  // analog input value to the Signal K server every time it changes.
  analog_input1->connect_to(new SKOutputFloat(
      "sensors.analog_input1.voltage",         // Signal K path
      "/sensors/analog_input1/voltage",        // configuration path, used in the
                                              // web UI and for storing the
                                              // configuration
      new SKMetadata("V",                     // Define output units
                     "Analog input 1 voltage")  // Value description
      ));
  analog_input2->connect_to(new SKOutputFloat(
      "sensors.analog_input2.voltage",         // Signal K path
      "/sensors/analog_input2/voltage",        // configuration path, used in the
                                              // web UI and for storing the
                                              // configuration
      new SKMetadata("V",                     // Define output units
                     "Analog input 2 voltage")  // Value description
      ));
  analog_input3->connect_to(new SKOutputFloat(
      "sensors.analog_input3.voltage",         // Signal K path
      "/sensors/analog_input3/voltage",        // configuration path, used in the
                                              // web UI and for storing the
                                              // configuration
      new SKMetadata("V",                     // Define output units
                     "Analog input 3 voltage")  // Value description
      ));


  // Start networking, SK server connections and other SensESP internals
  sensesp_app->start();
}

void loop() { app.tick(); }
