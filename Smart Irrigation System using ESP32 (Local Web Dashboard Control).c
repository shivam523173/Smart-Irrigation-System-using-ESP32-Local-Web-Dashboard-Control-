#include <WiFi.h>
#include <WebServer.h>

// ---------- Wi-Fi ----------
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ---------- Relay ----------
const int RELAY_PIN = 26;  
const int RELAY_ACTIVE_LEVEL = LOW;     
const int RELAY_INACTIVE_LEVEL = HIGH;

// ---------- Moisture Sensor ----------
const int MOISTURE_PIN = 34; 
int MOISTURE_DRY_RAW = 3600; 
int MOISTURE_WET_RAW = 1600; 

bool relayOn = false;
WebServer server(80);

// Relay control helper
void relayWrite(bool on) {
  relayOn = on;
  digitalWrite(RELAY_PIN, on ? RELAY_ACTIVE_LEVEL : RELAY_INACTIVE_LEVEL);
}

int moisturePercent(int raw) {
  long pct = map(raw, MOISTURE_DRY_RAW, MOISTURE_WET_RAW, 0, 100);
  return constrain(pct, 0, 100);
}

// Web Dashboard Page
const char* page = R"HTML(
<!DOCTYPE html><html><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Smart Irrigation</title>
<style>body{font-family:Arial;padding:20px;max-width:500px;margin:auto;}
button{padding:12px;margin:5px;font-size:18px;border-radius:8px;border:none;}
.on{background:#4CAF50;color:#fff;} .off{background:#f44336;color:#fff;}
</style></head><body>
<h2>Smart Irrigation Dashboard</h2>
<p>Moisture: <span id="moisture">--</span>%</p>
<p>Relay: <span id="relay">--</span></p>
<button class="on" onclick="fetch('/on')">ON</button>
<button class="off" onclick="fetch('/off')">OFF</button>
<script>
setInterval(async()=>{let r=await fetch('/status');r=await r.json();
document.getElementById('moisture').innerText=r.moisture_pct;
document.getElementById('relay').innerText=r.relay?"ON":"OFF";},2000);
</script></body></html>
)HTML";

// Handlers
void handleRoot(){ server.send(200,"text/html",page); }

void handleStatus(){
  int raw = analogRead(MOISTURE_PIN);
  int pct = moisturePercent(raw);
  String json = "{\"relay\":" + String(relayOn) +
                ",\"moisture_pct\":" + String(pct) + "}";
  server.send(200,"application/json",json);
}

void handleOn(){ relayWrite(true); handleStatus(); }
void handleOff(){ relayWrite(false); handleStatus(); }

void setup(){
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  relayWrite(false);

  analogReadResolution(12);
  analogSetPinAttenuation(MOISTURE_PIN, ADC_11db);

  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){delay(300);}
  Serial.println(WiFi.localIP());

  server.on("/",handleRoot);
  server.on("/status",handleStatus);
  server.on("/on",handleOn);
  server.on("/off",handleOff);
  server.begin();
}

void loop(){ server.handleClient(); }
