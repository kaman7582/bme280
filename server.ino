

// Load Wi-Fi library
#include <WiFi.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Replace with your network credentials
const char* ssid     = "qiao qiao house";
const char* password = "198422hk";

// Set web server port number to 80
WiFiServer server(80);
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
#define Addr 0x76

// Variable to store the HTTP request
//String header;



String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
 
String html_1 = R"=====(
<!DOCTYPE html>
<html>

<script>
  var mytimer=setInterval(clock,1000);
  function clock() 
  {
      ajaxLoad("sensor");
  }
 
var ajaxRequest = null;
if (window.XMLHttpRequest)  { ajaxRequest =new XMLHttpRequest(); }
else                        { ajaxRequest =new ActiveXObject("Microsoft.XMLHTTP"); }
 
 
function ajaxLoad(ajaxURL)
{
  if(!ajaxRequest){ alert("AJAX is not supported."); return; }
 
  ajaxRequest.open("GET",ajaxURL,true);
  ajaxRequest.onreadystatechange = function()
  {
    if(ajaxRequest.readyState == 4 && ajaxRequest.status==200)
    {
      var ajaxResult = ajaxRequest.responseText;
      document.getElementById("bme").innerHTML = ajaxResult;
    }
  }
  ajaxRequest.send();
}
 
</script>
 <p id = "bme">Sensor values</p>
<body>

</body>
</html>
 
)=====";

int LED_Pin = 10;

void setup() {
      Serial.begin(115200);
     bool status;

  // Initialize the output variables as outputs
    status = bme.begin(Addr);
    if (!status) {
        Serial.println("Can not find the Bme280 sensor, check the right I2C address");
        while (1);
    }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
String request = "";
int count = 0;
void loop() 
{
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client)  {  return;  }
    StaticJsonDocument<256> doc;
    
    JsonObject object = doc.to<JsonObject>();    
    object["temperature"] = bme.readTemperature();
    object["Humidity"] = bme.readHumidity();
    object["Pressure"] = bme.readPressure() / 100.0F;
    object["Altitude"] = bme.readAltitude(SEALEVELPRESSURE_HPA);
    // Read the first line of the request
    request = client.readStringUntil('\r');
 
    Serial.print("request: "); Serial.println(request); 
 
    if ( request.indexOf("sensor") > 0 )  
    {
          client.print( header );
          //client.print( count++);
          serializeJson(object,client);
          serializeJson(object,Serial);
    }
    else
    {
        //client.flush();
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
        client.print( html_1 );    
        delay(5);
    }
 
  // The client will actually be disconnected when the function returns and 'client' object is detroyed
} // void loop()

 