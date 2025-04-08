#include <dht11.h>

dht11 DHT;
#define DHT11_PIN 5  // DHT-11 连接到 D5
unsigned long startTime;  
unsigned long duration = 300 * 1000;  // 300秒（5分钟）

void setup() {
  Serial.begin(9600);
  Serial.println("DHT TEST PROGRAM");
  Serial.println("Time (s),\tStatus,\tHumidity (%),\tTemperature (°C)");
  
  startTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - startTime > duration) {
    Serial.println("Data collection completed.");
    while (1); // 停止运行
  }

  int elapsedTime = (currentTime - startTime) / 1000; // 计算运行时间（秒）
  Serial.print(elapsedTime);
  Serial.print(",\t");

  int chk = DHT.read(DHT11_PIN);  // 读取 DHT-11 传感器数据
  switch (chk) {
    case DHTLIB_OK:
      Serial.print("OK,\t");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.print("Checksum error,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.print("Timeout error,\t");
      break;
    default:
      Serial.print("Unknown error,\t");
      break;
  }
  
  // 显示湿度和温度数据
  Serial.print(DHT.humidity, 1);  
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);
  
  delay(2000); // 每 2 秒采集一次
}
