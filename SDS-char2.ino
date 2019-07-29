#include <ArduinoJson.h>
//#define DEBUG 1
const int charBuffer = 512;

String inputString = "";  // a String to hold incoming data

long inputCount;
bool stringComplete = false;     // whether the string is complete

char inputTopic[] = "device/X9Gg88B4pdfdmEtXX1fFphzC/asset/relay/command";
char inputPayload[charBuffer];

void setup() {
  //
  Serial.begin(9600);
  char json[50];
  stringComplete = false;
  inputCount = 0;
  //
  // char SIMpayload[] = "\{\"at\"\:\"2019-07-26T06:13:12.779169Z\",\"value\":false,\"meta\":null}";
  // payload2 = '{"at":"2019-07-26T06:13:22.155859Z","value":true,"meta":null}'
  // payload3 = '{"at":"2019-07-26T06:13:42.796807Z","value":900,"meta":null}'
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // ***
  Serial.println(F("** Send me some inputPayload in JSON **"));
}

void loop() {
  // put your main code here, to run repeatedly:
  // print the string when a newline arrives:
  if (stringComplete) {
    inputString.toCharArray(inputPayload, charBuffer);
    stringComplete = false;
    inputCount = 0;
    inputString = "";
    //
#ifdef DEBUG
    Serial.println(F("## loop ##"));
    Serial.print(F("inputTopic= "));
    Serial.println(inputTopic);
    Serial.print(F("inputPayload= "));
    Serial.println(inputPayload);
#endif
    //
    mycallback(inputTopic, inputPayload, charBuffer);
  }
}

void mycallback(char* topic, byte* payload, unsigned int length) {
  //
#ifdef DEBUG
  Serial.println(F("## TOPIC ##"));
  Serial.println(topic);
#endif
  //
  char ATTjson[charBuffer];
  for (int i = 0; i < length; i++) {
    ATTjson[i] = (char)payload[i];
  }
  // ATTjson[length] = '\0';
  //
#ifdef DEBUG
  Serial.println(F("## PAYLOAD ##"));
  Serial.print(F("ATTjson= "));
  Serial.println(ATTjson);
  Serial.print(F("Payload ["));
  Serial.print(ATTjson);
  Serial.println(F("] "));
#endif
  //
  // Here we go with JSON
  //
  // Allocate the JSON document
  StaticJsonDocument<charBuffer> jsonDoc;
  // Parse JSON object
  DeserializationError error = deserializeJson(jsonDoc, ATTjson);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  //
  // extract the data
  //
  JsonObject object = jsonDoc.as<JsonObject>();
  int value = jsonDoc["value"];
  // Extract values
  Serial.print(F("## Resulting value: "));
  Serial.print(value);
  Serial.println(F(" ##"));

}


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
#ifdef DEBUG
      Serial.println(F("## SerialEvent ##"));
      Serial.println(inputString);
#endif
      return ;
    }
    else {
      // add it to the inputString:
      inputString += inChar;
      //
    }
  }
}
