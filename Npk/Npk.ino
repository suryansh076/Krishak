#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* WIFINAME = "suryansh_Andriod";
const char* PASS = "1931177@suryansh";

//int led = D0;
WiFiServer server(80);

// RE and DE Pins set the RS485 module
// to Receiver or Transmitter mode
#define RE 15 //D8
#define DE 13 //D7

#define DHTPIN 12 // D6
#define DHTTYPE DHT11

bool Status = false;

// Connect Soil moisture analog sensor pin to A0 of NodeMCU
const int moisture_sensor = A0;

// Modbus RTU requests for reading NPK values
const byte query[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x03, 0x05, 0xCB};

// A variable used to store NPK values
byte response[11];

// Initializing DHT sensor with dht pin
DHT dht(DHTPIN, DHTTYPE);

// Sets up a new SoftwareSerial object
SoftwareSerial mod(0, 2); //(RO,DI)-(D3,D4)

void setup() {
  // Set the baud rate for the Serial port
  Serial.begin(9600);
  Serial.print("Connecting To ");
  Serial.print(WIFINAME);
  WiFi.begin(WIFINAME, PASS);

  // Set the baud rate for the SerialSoftware object
  mod.begin(9600);

  // Initializing DHT Sensor
  dht.begin();

  pinMode(D0, INPUT_PULLUP);

  // Define pin modes for RE and DE
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  //  Serial.print("IP address: ");
  //  Serial.println(WiFi.localIP());
  delay(1000);
}

void loop() {

  byte nitro, phos, potas;
  float moisture_read;
  float moisture_int;
  float moisture_percentage;

  NPKFunc();
  moisture_read = analogRead(moisture_sensor);
  moisture_int = moisture_read / 1024.00;
  moisture_percentage = 100.00 - (moisture_int * 100);

  nitro = response[4];
  phos = response[6];
  potas = response[8];

  float hum = dht.readHumidity();
  float tempC = dht.readTemperature();

  // Print values to the serial monitor
  //  display(nitro, phos, potas, hum, tempC, moisture_percentage);
  int Signal = digitalRead(D1);
  Serial.print("Digital value: ");
  Serial.println(Signal);
  // as by default digital pin reads high
  //  if (Signal == 0)
  //  {
  //    Status = !Status;
  //  }
  if (Signal == 1)
  {
    Serial.println("Button Pressed");
    if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
      WiFiClient client;
      HTTPClient http;
      String query_api = "http://192.168.136.43:8000/api/data/?n=" + String(nitro) + "&p=" + String(phos) + "&k=" + String(potas) + "&t=" + String(tempC) + "&h=" + String(hum);
      Serial.println(query_api);
      http.begin(client, query_api); //Specify the URL
      int httpCode = http.GET();                                        //Make the request

      if (httpCode > 0) { //Check for the returning code

        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload.substring(2, payload.length() - 2));
      }
      else {
        Serial.println("Error on HTTP request");
      }
      http.end(); //Free the resources
    }
    else {
      Serial.println("Wifi Not Connected");
    }
    //    Status = false;
    Signal = 0;
  }
  else {
    Serial.println("Button Not Pressed");
  }
  delay(2000);
}

void NPKFunc() {

  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(query, sizeof(query)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 11; i++) {
      response[i] = mod.read();
      //      Serial.print(response[i], DEC);
      //      Serial.print(" ");
    }
    //    Serial.println();
  }
}
void display(byte nitro , byte phos, byte potas, float hum, float tempC, float moisture_percentage) {
  Serial.print(F("Nitrogen: "));
  Serial.print(nitro);
  Serial.println(F(" mg/Kg"));
  Serial.print(F("Phosphorous: "));
  Serial.print(phos);
  Serial.println(F(" mg/Kg"));
  Serial.print(F("Potassium: "));
  Serial.print(potas);
  Serial.println(F("mg/Kg"));

  Serial.print(F("Soil Moisture(in Percentage) = {phoas}"));
  Serial.print(moisture_percentage);
  Serial.println(F("%"));

  Serial.print(F(" Humidity: "));
  Serial.print(hum);
  Serial.print(F("%  Temperature: "));
  Serial.print(tempC);
  Serial.println(F("°C "));
}
