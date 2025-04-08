#ifndef _MyWiFi_H_
#define _MyWiFi_H_

#include <WiFiS3.h>
#include "MyParameters.h"

extern const char html_page[] PROGMEM;

extern const char* ssid;
extern const char* password;
extern IPAddress local_ip;  
extern IPAddress subnet;
extern IPAddress gateway;

extern WiFiServer server;

void WifiStart();

void RunHTML();

float extractValue(String request, String key);

void sendHTML(WiFiClient client);

#endif