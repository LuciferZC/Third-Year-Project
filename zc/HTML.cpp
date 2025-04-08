#include "MyWIFI.h"

const char html_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>Sensor Monitoring</title> 
  <script>
    function updateData() {

      fetch("/data")
        .then(response => response.json())
        .then(data => {
          document.getElementById("soil").innerText = data.soil;
          document.getElementById("air").innerText = data.air;
          document.getElementById("light").innerText = data.light;
          document.getElementById("tvoc").innerText = data.tvoc;
          document.getElementById("co2").innerText = data.co2;
          document.getElementById("temp").innerText = data.temp;


          document.getElementById("targetSoil").innerText = data.soiltar;
          document.getElementById("targetAir").innerText = data.airtar;
          document.getElementById("targetLight").innerText = data.lighttar;
          document.getElementById("targetTVOC").innerText = data.tvoctar;
          document.getElementById("targetCO2").innerText = data.co2tar;
          document.getElementById("targetTemp").innerText = data.temptar;
        });
    }

    function sendSettings() {

      let params = new URLSearchParams();
      if (document.getElementById("targetSoilset").value !== "") {
        params.append("soil", document.getElementById("targetSoilset").value);
      }
      if (document.getElementById("targetAirset").value !== "") {
        params.append("air", document.getElementById("targetAirset").value);
      }
      if (document.getElementById("targetLightset").value !== "") {
        params.append("light", document.getElementById("targetLightset").value);
      }
      if (document.getElementById("targetTVOCset").value !== "") {
        params.append("tvoc", document.getElementById("targetTVOCset").value);
      }
      if (document.getElementById("targetCO2set").value !== "") {
        params.append("co2", document.getElementById("targetCO2set").value);
      }
      if (document.getElementById("targetTempset").value !== "") {
        params.append("temp", document.getElementById("targetTempset").value);
      }
    
      
      fetch("/set?" + params.toString(), { method: "POST" });

      updateData();
    }

    setInterval("updateData()", 1000);
  </script>
</head>
<body>
  <h1>Sensor Monitoring</h1>
  <p>Soil Moisture Now: <span id="soil">--</span>%                  Target: <span id="targetSoil">--</span>%</p>
  <p>Air Humidity Now: <span id="air">--</span>%                    Target: <span id="targetAir">--</span>%</p>
  <p>Light Intensity Now: <span id="light">--</span> lux            Target: <span id="targetLight">--</span>lux</p>
  <p>TVOC Now: <span id="tvoc">--</span> ppb                        Target: <span id="targetTVOC">--</span>ppb</p>
  <p>CO2 Now: <span id="co2">--</span> ppm                          Target: <span id="targetCO2">--</span>ppm</p>
  <p>Temperature Now: <span id="temp">--</span>°C                   Target: <span id="targetTemp">--</span>°C</p>

  <button onclick="updateData()"; style="font-size:2vw; padding:5vh 10vw;"> Update</button>

  <h2>Target Value Settings</h2>
  <p>Soil Moisture: <input type="number" id="targetSoilset" ></p>
  <p>Air Humidity: <input type="number" id="targetAirset" ></p>
  <p>Light Intensity: <input type="number" id="targetLightset" ></p>
  <p>TVOC: <input type="number" id="targetTVOCset" ></p>
  <p>CO₂: <input type="number" id="targetCO2set" ></p>
  <p>Temperature: <input type="number" id="targetTempset" ></p>

  <button onclick="sendSettings()"; style="font-size:2vw; padding:5vh 10vw;"> SetTarget</button>
</body>
</html>
)rawliteral";