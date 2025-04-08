#include "MyWIFI.h"

const char* ssid = "Test WiFi";
const char* password = "12345678";
IPAddress local_ip(192, 168, 10, 1);  
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 10, 1);

WiFiServer server(80);

void WifiStart()
{

  WiFi.config(local_ip, gateway, subnet);
  WiFi.beginAP(ssid, password);
  Serial.println("WiFi AP Started");
  Serial.print("AP IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void RunHTML()
{

  WiFiClient client = server.available();

    if (client) {
      String request = client.readStringUntil('\r');
      Serial.println(request);
      client.flush();

      if (request.indexOf("GET /data") >= 0)
      {
        String jsonData = "{";
        jsonData += "\"soil\":" + String(isnan(soilMoisture) ? -1 : soilMoisture) + ",";
        jsonData += "\"air\":" + String(isnan(airHumidity) ? -1 : airHumidity) + ",";
        jsonData += "\"light\":" + String(isnan(lightIntensity) ? -1 : lightIntensity) + ",";
        jsonData += "\"tvoc\":" + String(isnan(tvoc) ? -1 : tvoc) + ",";
        jsonData += "\"co2\":" + String(isnan(co2) ? -1 : co2) + ",";
        jsonData += "\"temp\":" + String(isnan(temperature) ? -1 : temperature) + ",";

        jsonData += "\"soiltar\":" + String(isnan(targetSoil) ? -1 : targetSoil) + ",";
        jsonData += "\"airtar\":" + String(isnan(targetAir) ? -1 : targetAir) + ",";
        jsonData += "\"lighttar\":" + String(isnan(targetLight) ? -1 : targetLight) + ",";
        jsonData += "\"tvoctar\":" + String(isnan(targetTVOC) ? -1 : targetTVOC) + ",";
        jsonData += "\"co2tar\":" + String(isnan(targetCO2) ? -1 : targetCO2) + ",";
        jsonData += "\"temptar\":" + String(isnan(targetTemp) ? -1 : targetTemp) + "}";


        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json");
        client.println("Connection: close");
        client.println();
        client.println(jsonData);
        Serial.println(jsonData);
      } 

      else if (request.indexOf("POST /set") >= 0) 
      {
        if (request.indexOf("soil=") >= 0) targetSoil = extractValue(request, "soil");
        if (request.indexOf("air=") >= 0) targetAir = extractValue(request, "air");
        if (request.indexOf("light=") >= 0) targetLight = extractValue(request, "light");
        if (request.indexOf("tvoc=") >= 0) targetTVOC = extractValue(request, "tvoc");
        if (request.indexOf("co2=") >= 0) targetCO2 = extractValue(request, "co2");
        if (request.indexOf("temp=") >= 0) targetTemp = extractValue(request, "temp");

        
      

        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println("Connection: close");
        client.println();
        client.println("Settings Updated");
      } else
      {
        sendHTML(client);
      }

      client.stop();

    }
  
}


float extractValue(String request, String key) {
  int start = request.indexOf(key + "=") + key.length() + 1;
  int end = request.indexOf("&", start);
  if (end == -1) end = request.length();
  return request.substring(start, end).toFloat();
}

void sendHTML(WiFiClient client) {
  client.println(html_page);
}

