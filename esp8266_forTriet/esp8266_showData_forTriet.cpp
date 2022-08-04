#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid     = "BaoThe0302";
const char* password = "09022020";
const char* serverName = "http://k3eryn93.chiasenhe.com/";

int notificLed = 4;
int location1;
int location2;
int httpResponseCode = 0;
String soLuongHangConTrongKho = "";
String reportCode = "";
String payloadFS = "";
String valueUART = "";
String httpRequestData = "";
String apiKeyValue = "r2i5dPRqC7p2J";

void setup() {
  Serial.begin(115200);
  pinMode(OUTPUT, notificLed);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(notificLed, LOW);
  delay(500);
}

void loop() {
  getValueUART();
}
void getValueUART() {
  if (Serial.available() > 0) {
    valueUART = Serial.readString();
    location1 = valueUART.indexOf("@");
    location2 = valueUART.indexOf("#");
    valueUART = valueUART.substring(location1+1, location2);
    if (valueUART != "") {
      getDatabase();
    }
  }
  return;
}

void getDatabase() {
//  value = "nhaphang|loaihang|tenhang|07/03/2022|07/03/2055"; //loại nhập/xuất|loại hàng|tên hàng|hạn sử dụng|ngày nhập/xuất
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  Serial.println(valueUART);
  httpRequestData = "api_key=" + apiKeyValue + "&value=" + valueUART;
  Serial.print("httpRequestData: ");
  Serial.println(httpRequestData);
  httpResponseCode = http.POST(httpRequestData);
  payloadFS = http.getString();
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode); 
    Serial.print("Listen: ");
    Serial.println(payloadFS); //
    soLuongHangConTrongKho = payloadFS;
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}