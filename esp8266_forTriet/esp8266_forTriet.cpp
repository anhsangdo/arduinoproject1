#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid     = "BaoThe0302";
const char* password = "09022020";
const char* serverName = "http://k3eryn93.chiasenhe.com/";

int location1;
int location2;
int httpResponseCode = 0;
int rule = LOW;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int notificLed = 4;
int ledBaoBan = 0;
int statusButton = 2; 
const byte touchPin = 15;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 5000;    // the debounce time; increase if the output flickers
unsigned long timeBeforeGetUART = 0;

String formattedDate;
String dateNow;

String payloadFS = "";
String valueUART = "";
String dataUART = "";
String httpRequestData = "";
String apiKeyValue = "r2i5dPRqC7p2J";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup() {
  Serial.begin(115200);
  pinMode(OUTPUT, ledBaoBan);
  pinMode(OUTPUT, notificLed);
  pinMode(OUTPUT, statusButton);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  timeClient.setTimeOffset(25200);
  digitalWrite(notificLed, LOW);
  delay(500);
}

void loop() {
  checkButtonReset();
  checkButtonSetRule();
  getValueUART();
}
void getValueUART() {
  if (Serial.available() > 0) {
    valueUART = Serial.readString();
    location1 = valueUART.indexOf("@");
    location2 = valueUART.indexOf("#");
    valueUART = valueUART.substring(location1+1, location2);
    if (valueUART != "") {
      getTime();
      uploadDatabase();
      Serial.print("dateNow: ")
      Serial.println(dateNow);
      Serial.print("valueUART: ")
      Serial.println(valueUART);
      Serial.print("IDcard: ")
      Serial.println(IDcard);
    }
    
  }
  else {
    digitalWrite(ledBaoBan, LOW); //led b??o r???nh
  }
  return;
}
void getTime() {
  while(!timeClient.update()) {
      timeClient.forceUpdate();
  }
  formattedDate = timeClient.getFormattedDate();
  int splitT = formattedDate.indexOf("T");
  dateNow = formattedDate.substring(0, splitT);
  return;
}
void uploadDatabase() {
//  value = "nhaphang|loaihang|tenhang|07/03/2022|07/03/2055"; //lo???i nh???p/xu???t|lo???i h??ng|t??n h??ng|h???n s??? d???ng|ng??y nh???p/xu???t
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  Serial.println(rule);
  Serial.println(valueUART);
  Serial.println(dateNow);
  if (rule == 0) {
    httpRequestData = "api_key=" + apiKeyValue + "&value=" + "nhaphang" + valueUART + IDcard + dateNow ;
  }
  else if (rule == 1) {
    httpRequestData = "api_key=" + apiKeyValue + "&value=" + "xuathang" + valueUART + IDcard + dateNow ;
  }
  Serial.print("httpRequestData: ");
  Serial.println(httpRequestData);
  httpResponseCode = http.POST(httpRequestData);
  payloadFS = http.getString();
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode); 
    Serial.print("Listen: ");
    Serial.println(payloadFS); 
    if (payloadFS.substring(0, 3) == "264") {
      Serial.println("C???p nh???t CSDL th??nh c??ng!"); 
    } 
    else if (payloadFS.substring(0, 3) == "203") {
      Serial.println("C???p nh???t CSDL kh??ng th??nh c??ng!"); 
    } 
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

void checkButtonSetRule() {
  int reading = digitalRead(touchPin); //gia tri nut nhan
  if (reading == HIGH) {
    digitalWrite(statusButton, !reading);
  }
  if (reading != lastButtonState) { //neu gia tri nut nhan hien tai ko bang gtri trc do
    lastDebounceTime = millis(); // thi set thoi gian bang tgian htai
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        digitalWrite(statusButton, HIGH);
        rule = !rule;
        Serial.println("Rule changed!");
        digitalWrite(notificLed, rule);
      }
    }
  }
  lastButtonState = reading;
  return;
}


void checkButtonReset() {
  int reading1 = digitalRead(touchPin1); //gia tri nut nhan
  if (reading1 == HIGH) {
    digitalWrite(statusButton1, !reading1);
  }
  if (reading1 != lastButtonState1) { //neu gia tri nut nhan hien tai ko bang gtri trc do
    lastDebounceTime1 = millis(); // thi set thoi gian bang tgian htai
  }
  if ((millis() - lastDebounceTime1) > debounceDelay1) {
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
      if (buttonState1 == HIGH) {
        digitalWrite(statusButton1, HIGH);
        rule = !rule;
        IDcard = "";
        Serial.println("Reset done!");
        digitalWrite(notificLed1, rule);
      }
    }
  }
  lastButtonState1 = reading1;
  return;
}

void checkID() {
  if(mfrc522.PICC_IsNewCardPresent()) 
  {
    if(mfrc522.PICC_ReadCardSerial()) 
    {
      for (byte i = 0; i < mfrc522.uid.size; i++) 
      {
          IDcardbyte = mfrc522.uid.uidByte[i];
          IDcard = IDcard + String(IDcardbyte);  
      }  
      mfrc522.PICC_HaltA();    
    } 
  }
  if (IDcard == keySecurity) 
  {
    access = 1;
  }
  return;  
}