#include <WiFiS3.h>
#include "CBTimer.h"
#include "MyWIFI.h"
#include "MyParameters.h"
#include "MyControl.h"
//=======
#include <Wire.h>
#include <Adafruit_SGP30.h>      // SGP-30 传感器库
#include <Adafruit_GFX.h>        // OLED 显示屏 GFX 库
#include <Adafruit_SSD1306.h>    // OLED 显示屏库
#include <DHT.h>                 // 使用 DHT.h 库替代 dht11.h

#define SCREEN_WIDTH 128         // OLED 屏幕宽度
#define SCREEN_HEIGHT 64         // OLED 屏幕高度
#define OLED_RESET -1            // OLED 重置引脚
#define OLED_I2C_ADDRESS 0x3C    // OLED I2C 地址
#define DHT11_PIN 5              // DHT-11 连接到 D5
#define SOIL_MOISTURE_PIN A1     // 土壤湿度传感器连接到 A1
#define FanPin1 9              // DHT-11 连接到 D5
#define FanPin2 6              // DHT-11 连接到 D5

#define ReadLEDPin  A0             // LEDREAD
#define LEDPin 11     // 土壤湿度传感器连接到 A1

Adafruit_SGP30 sgp;              // SGP-30 传感器实例
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // OLED 实例
DHT dht(DHT11_PIN, DHT11);      // 使用 DHT 库替代 dht11.h 库，创建 DHT 传感器实例

bool manualMode = false;         // 是否处于手动输入模式
unsigned long manualModeStart = 0; // 记录手动模式开始时间

float manualTemp, manualHumidity, manualSoil, manualCO2, manualTVOC, manualLight;
bool tempSet = false, humiditySet = false, soilSet = false, co2Set = false, tvocSet = false, lightSet = false;  // 变量修改标志

void readSensors();
void updateDisplay();
void checkSerialInput();
void parseInput(String input);
float extractMyValue(String input, String key) ;
void displayError(String message) ;
void displayMessage(String message);
//=======


void setup() {
  Serial.begin(9600);
  pinMode(5, INPUT_PULLUP);
  FanStart(FanPin1,FanPin2);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("OLED initialization failed!"));
    displayError("OLED Error!");  // 显示 OLED 错误
    for(;;);
  }

  if (!sgp.begin()) {
    Serial.println("SGP30 sensor not found. Check wiring!");
    displayError("SGP30 Error!");  // 显示 SGP-30 错误\
    for(;;);
  }

  LEDStart(LEDPin);


  WifiStart();

  Serial.println("System initialized!");
  displayMessage("System Ready");  // 显示系统初始化完成
  delay(2000);
}

void loop() {
  RunHTML();

  delay(25);

  LEDControl(LEDPin);

  FanControl(FanPin1,FanPin2);


  if (manualMode && millis() - manualModeStart > 5000) {

    Serial.println("Returning to automatic mode...");

    manualMode = false;

    tempSet = humiditySet = soilSet = co2Set = tvocSet = lightSet = false;

  }

 

  // **读取传感器数据**

  readSensors();

 

  // **OLED & 串口同步显示**

  updateDisplay();

  checkSerialInput();

  delay(1);
}

// **读取传感器数据**
void readSensors() {
  // 使用 DHT 库的 readHumidity 和 readTemperature 函数替代 dht11 库的 read 方法
  int checkairHumidity = dht.readHumidity();
  int checktemperature = dht.readTemperature();

  airHumidity = isnan(checkairHumidity)?airHumidity:checkairHumidity;

  temperature = isnan(checktemperature)?temperature:checktemperature;
  if (!soilSet) {
    soilMoisture = analogRead(SOIL_MOISTURE_PIN);
    soilMoisture = map(soilMoisture, 0, 1023, 0, 100);
  }

  if (sgp.IAQmeasure()) {
    if (!co2Set) co2 = sgp.eCO2;
    if (!tvocSet) tvoc = sgp.TVOC;
  }


  lightIntensity = (lightSet ? manualLight : 1023 - analogRead(ReadLEDPin));

  soilMoisture = soilSet ? manualSoil : soilMoisture;
  airHumidity = humiditySet ? manualHumidity : airHumidity;
  tvoc = tvocSet ? manualTVOC : tvoc;
  co2 = co2Set ? manualCO2 : co2;
  temperature = tempSet ? manualTemp : temperature;
  temperature = temperature<0 ? 0 : temperature;



}

// **OLED 显示传感器数据**
void updateDisplay() {
  display.clearDisplay();

  // **黄色区域：标题**
  display.setTextColor(SSD1306_WHITE);  // 设置文本颜色（白色，OLED 黄区会显示黄色）
  display.setCursor(0, 0);             // 设置光标位置（左上角）
  display.setTextSize(1);               // 设置字体大小
  display.println("Greenhouse Monitor"); // 在同一行显示 "Greenhouse Monitor"

  // **蓝色区域（传感器数据）**
  display.setTextColor(SSD1306_WHITE);  
  display.setTextSize(1);
  display.setCursor(0, 16);

  display.print("Temp: "); display.print(tempSet ? manualTemp : temperature); display.println(" C");
  display.print("Humidity: "); display.print(humiditySet ? manualHumidity : airHumidity); display.println(" %");
  display.print("Soil: "); display.print(soilSet ? manualSoil : soilMoisture); display.println(" %");
  display.print("CO2: "); display.print(co2Set ? manualCO2 : co2); display.println(" ppm");
  display.print("TVOC: "); display.print(tvocSet ? manualTVOC : tvoc); display.println(" ppb");
  display.print("Light: "); display.print(lightSet ? manualLight : lightIntensity); display.println(" lux");

  display.display();

  // **同步输出到串口**
  Serial.print("Temp: "); Serial.print(tempSet ? manualTemp : temperature); Serial.println(" C");
  Serial.print("Humidity: "); Serial.print(humiditySet ? manualHumidity : airHumidity); Serial.println(" %");
  Serial.print("Soil: "); Serial.print(soilSet ? manualSoil : soilMoisture); Serial.println(" %");
  Serial.print("CO2: "); Serial.print(co2Set ? manualCO2 : co2); Serial.println(" ppm");
  Serial.print("TVOC: "); Serial.print(tvocSet ? manualTVOC : tvoc); Serial.println(" ppb");
  Serial.print("Light: "); Serial.print(lightSet ? manualLight : lightIntensity); Serial.println(" lux");
  Serial.println("--------------------------------");
}


// **检查串口输入**
void checkSerialInput() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() == 0) return;

    manualMode = true;
    manualModeStart = millis();
    
    // **解析输入内容**
    parseInput(input);
  }
}

// **解析用户输入**
void parseInput(String input) {
  Serial.print("Input received: ");
  Serial.println(input);

  if (input.indexOf("temp=") != -1) {
    manualTemp = extractMyValue(input, "temp=");
    tempSet = true;
  }
  if (input.indexOf("humidity=") != -1) {
    manualHumidity = extractMyValue(input, "humidity=");
    humiditySet = true;
  }
  if (input.indexOf("soil=") != -1) {
    manualSoil = extractMyValue(input, "soil=");
    soilSet = true;
  }
  if (input.indexOf("co2=") != -1) {
    manualCO2 = extractMyValue(input, "co2=");
    co2Set = true;
  }
  if (input.indexOf("tvoc=") != -1) {
    manualTVOC = extractMyValue(input, "tvoc=");
    tvocSet = true;
  }
  if (input.indexOf("light=") != -1) {
    manualLight = extractMyValue(input, "light=");
    lightSet = true;
  }
  // **OLED 立即更新**
  updateDisplay();
}

// **提取数值**
float extractMyValue(String input, String key) {
  int startIndex = input.indexOf(key);
  if (startIndex == -1) return 0;
  startIndex += key.length();
  int endIndex = input.indexOf(" ", startIndex);
  if (endIndex == -1) endIndex = input.length();
  return input.substring(startIndex, endIndex).toFloat();
}

// **显示错误信息**
void displayError(String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println(message);
  display.display();
}

// **显示普通信息**
void displayMessage(String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println(message);
  display.display();
}


