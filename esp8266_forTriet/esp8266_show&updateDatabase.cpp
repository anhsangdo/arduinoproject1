#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4); 

const char* ssid     = "Dinh Van Tri 24G";
const char* password = "0937037858";

//const char* ssid     = "WareHouseIOT";
//const char* password = "Triet17018191";

const char* serverName = "https://k3eryn93.chiasenhe.com/getData.php";

int location1;
int location2;
int location3;
int httpResponseCode = 0;

const byte touchPin1 = D5;
int lastButtonState1 = LOW;
unsigned long lastDebounceTime1 = 0;  // the last time the output pin was toggled
int buttonState1;  

const byte touchPin2 = D5;
int lastButtonState2 = LOW;
unsigned long lastDebounceTime2 = 0;  // the last time the output pin was toggled
int buttonState2;             // the current reading from the input pin

String tenHang;
String thongbao = "";
String soLuongHangConTrongKho = "";
String reportCode = "";
String payloadFS = "";
String valueUART = "";
String httpRequestData = "";
String apiKeyValue = "r2i5dPRqC7p2J";

unsigned long timeBeforeUart = 0;

//@|dientu|hangloai9|2022-03-19|#
void setup() 
{
  Serial.begin(9600);

  lcd.init();                    
  lcd.backlight();
  WiFi.begin(ssid, password);
  if(WiFi.status() != WL_CONNECTED) 
  {
    lcd.setCursor(0,0); lcd.print("DangTruyCapWifi...");
  }
  while(WiFi.status() != WL_CONNECTED) 
  { 
    Serial.println(".");
  }
  lcd.setCursor(0,0); lcd.print("KetNoiThanhCong!!!");
  lcd.setCursor(0,1); lcd.print(WiFi.localIP());
  delay(3000);
  lcd.clear();
  Serial.println(WiFi.localIP());
  delay(250);
  lcd.setCursor(0,0); lcd.print("TenHang:");
  lcd.setCursor(0,2); lcd.print("SoLuongTonKho:");
  
}

void loop() 
{
  getValueUART();
}
void getValueUART() 
{
  if (Serial.available() > 0) 
  {
    valueUART = Serial.readString();
    location1 = valueUART.indexOf("@");
    location2 = valueUART.indexOf("#");
    valueUART = valueUART.substring(location1+1, location2);
    //Serial.print(valueUART);
    tachchuoi();
    if (valueUART != "") 
    {
      uploadDatabase();
    }
  }
  return;
}

void uploadDatabase() 
{
  payloadFS = insertDatabase(valueUART);
  if (payloadFS != "Error") 
  {
    soLuongHangConTrongKho = payloadFS.substring(0, payloadFS.indexOf("|"));
    trangThaiKiemKho = payloadFS.substring(payloadFS.indexOf("|")+1, payloadFS.length());
    Serial.print("soLuongHangConTrongKho: ");
    Serial.println(soLuongHangConTrongKho);
    Serial.print("trang thai kiem kho: ");
    Serial.println(trangThaiKiemKho);
    Serial.println("=============");
    // soLuongHangConTrongKho.trim();
    lcd.setCursor(15,2); 
    lcd.print(soLuongHangConTrongKho);
    if(payloadFS == "Null") {
        while(thongbao == "done") {
            checkButtonSetEnough();
            checkButtonSetNoneEnough();
            Serial.print("thong bao cap nhat trang thai kiem kho: ");
            Serial.println(thongbao);
        }
        thongbao = "";
    }
  }
}
void tachchuoi() {
  tenHang = valueUART;
  tenHang.remove(0, 1);
  location3 = tenHang.indexOf("|");
  tenHang.remove(0, location3 + 1);
  location3 = tenHang.indexOf("|");
  tenHang.remove(location3, tenHang.length());
  Serial.println(tenHang);
}
void checkButtonSetNoneEnough() {
    int reading2 = digitalRead(touchPin2); //gia tri nut nhan
    if (reading2 != lastButtonState2) { //neu gia tri nut nhan hien tai ko bang gtri trc do
        lastDebounceTime2 = millis(); // thi set thoi gian bang tgian htai
    }
    if ((millis() - lastDebounceTime2) > 3000) {
        if (reading2 != buttonState2) {
            buttonState2 = reading2;
            if (buttonState2 == HIGH) {
                thongbao = insertDatabase(valueUART + "&check=noneEnough");     
            }
        }
    }
    lastButtonState2 = reading2;
}
void checkButtonSetEnough() {
   int reading1 = digitalRead(touchPin1); //gia tri nut nhan
    if (reading1 != lastButtonState1) { //neu gia tri nut nhan hien tai ko bang gtri trc do
        lastDebounceTime1 = millis(); // thi set thoi gian bang tgian htai
    }
    if ((millis() - lastDebounceTime1) > 3000) {
        if (reading1 != buttonState1) {
            buttonState1 = reading1;
            if (buttonState1 == HIGH) {
                thongbao = insertDatabase("&check=Enough");     
            }
        }
    }
    lastButtonState1 = reading1;
}

String insertDatabase(String Stringvalue) {
    String result;
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    //Serial.println(valueUART);
    httpRequestData = "api_key=" + apiKeyValue + "&value=" + Stringvalue;
    //Serial.print("httpRequestData: ");
    //Serial.println(httpRequestData);
    httpResponseCode = http.POST(httpRequestData);
    if (httpResponseCode > 0) {
        result = http.getString();
    }
    else {
        result = "Error"
    }
    http.end();
    return result;
}