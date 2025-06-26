#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>

const char* ssid = "Binary Base";
const char* password = "________@";
const int debounceDelay = 200; // Debouncing time in milliseconds

// Relay Pin
const int relayPin = 0;


// Timezone offset for Uzbekistan (UTC+5)
const long utcOffsetInSeconds = 5 * 3600;

// Create NTPClient instance
WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", utcOffsetInSeconds);

ESP8266WebServer server(80);

// Define the maximum number of ring times
const int maxRingTimes = 14;
String ringTimes[maxRingTimes] = {"08:00", "08:45", "08:50", "09:35", "09:40", "10:25", "10:45", "11:30", "11:35", "12:20", "12:25", "13:10", "13:15", "14:00"};

// Default bell duration
int bellDuration = 10000; // Default 10 seconds

void handleRoot() {
    timeClient.update();
    String currentTime = timeClient.getFormattedTime();
    String dayOfWeekStr = getDayOfWeek();

    String html = "<!DOCTYPE html>"
                  "<html lang='en'>"
                  "<head>"
                  "<meta charset='UTF-8'>"
                  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                  "<title>School Bell Automation</title>"
                  "<style>"
                  "body {"
                  "    font-family: Arial, sans-serif;"
                  "    background-color: #f0f8ff;"
                  "    color: #333;"
                  "    padding: 20px;"
                  "    text-align: center;"
                  "    margin: 0;"
                  "}"
                  "h1 {"
                  "    color: #0047ab;"
                  "    margin-bottom: 30px;"
                  "    font-size: 2.5em;"
                  "}"
                  "h2 {"
                  "    color: #008b8b;"
                  "    margin-top: 20px;"
                  "    font-size: 1.5em;"
                  "}"
                  "form {"
                  "    margin: 20px auto;"
                  "    padding: 20px;"
                  "    border: 1px solid #ccc;"
                  "    border-radius: 10px;"
                  "    background-color: #fff;"
                  "    box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);"
                  "    max-width: 400px;"
                  "    display: inline-block;"
                  "    text-align: left;"
                  "}"
                  "input[type='text'],"
                  "input[type='number'] {"
                  "    padding: 10px;"
                  "    margin: 10px 0;"
                  "    border: 1px solid #ccc;"
                  "    border-radius: 5px;"
                  "    width: calc(100% - 22px);"
                  "}"
                  "input[type='submit'] {"
                  "    background-color: #0047ab;"
                  "    color: white;"
                  "    padding: 10px 15px;"
                  "    border: none;"
                  "    border-radius: 5px;"
                  "    cursor: pointer;"
                  "    font-size: 1em;"
                  "}"
                  "input[type='submit']:hover {"
                  "    background-color: #003366;"
                  "}"
                  "a {"
                  "    text-decoration: none;"
                  "    color: black;"
                  "    font-weight: bold;"
                  "}"
                  ".manual-ring {"
                  "    background-color: #008b8b;"
                  "    padding: 10px 15px;"
                  "    border: none;"
                  "    border-radius: 5px;"
                  "    cursor: pointer;"
                  "    font-size: 1em;"
                  "    position: absolute;"
                  "    top: 20px;"
                  "    right: 20px;"
                  "}"
                  "#time, #dayOfWeek {"
                  "    font-size: 1.2em;"
                  "    font-weight: bold;"
                  "    margin-bottom: 10px;"
                  "    text-align: left;"
                  "}"
                  ".scrollable {"
                  "    max-height: 200px;"  // Set the height for the scrollable area
                  "    overflow-y: auto;"   // Enable vertical scrolling
                  "}"
                  "footer {"
                  "    margin-top: 40px;"
                  "    font-size: 0.9em;"
                  "    color: #666;"
                  "}"
                  "</style>"
                  "</head>"
                  "<body>"
                  "<button class='manual-ring' onclick=\"window.location.href='/manualring'\">Manual</button>"
                  "<h1>School Bell Automation</h1>"
                  "<div style='text-align: left;'>"
                  "<p id='time'>" + currentTime + "</p>"
                  "</div>"
                  "<form action='/set_time' method='POST'>"
                  "<h2>Set Ring Times</h2>"
                  "<div class='scrollable'>";  // Start scrollable area

    for (int i = 0; i < maxRingTimes; i++) {
        html += "<label for='ringtime" + String(i + 1) + "'>Ring Time " + String(i + 1) + " (HH:MM):</label>";
        html += "<input type='text' name='ringtime" + String(i + 1) + "' value='" + ringTimes[i] + "' required pattern='[0-9]{2}:[0-9]{2}'>";
    }

    html += "</div>"  // Close scrollable area
            "<input type='submit' value='Set Times'></form>"
            "<footer>"
            "<p>&copy; 2024 School Bell Automation. All rights reserved.</p>"
            "<p>Developer: <a href='https://t.me/alien457' target='_blank'>alien457</a></p>"
            "</footer>"
            "<script>"
            "setInterval(function() {"
            "    fetch('/time').then(response => response.text()).then(text => {"
            "        document.getElementById('time').innerText = text;"
            "    });"
            "    fetch('/day').then(response => response.text()).then(text => {"
            "        document.getElementById('dayOfWeek').innerText = text;"
            "    });"
            "}, 2000);"
            "</script>"
            "</body>"
            "</html>";

    server.send(200, "text/html", html);
}



void handleSetTime() {
  for (int i = 0; i < maxRingTimes; i++) {
    String paramName = "ringtime" + String(i + 1);
    if (server.hasArg(paramName)) {
      String newTime = server.arg(paramName);
      // Validate input format
      if (newTime.length() == 5 && newTime[2] == ':' && isDigit(newTime[0]) && isDigit(newTime[1]) && isDigit(newTime[3]) && isDigit(newTime[4])) {
        ringTimes[i] = newTime;
      } else {
        server.send(400, "text/html", "Invalid time format. Please use HH:MM.");
        return;
      }
    }
  }
  server.send(200, "text/html", "Ring times updated.");
}

void handleRingBell() {
  if (server.hasArg("duration")) {
    bellDuration = server.arg("duration").toInt() * 1000; // Convert seconds to milliseconds
    ringBell(bellDuration);
    server.send(200, "text/html", "Bell rung for " + String(bellDuration / 1000) + " seconds.");
  } else {
    server.send(400, "text/html", "Duration parameter missing.");
  }
}

void handleManualRing() {
  String html = "<html><body>";
  html += "<h1>Manual Bell Ring</h1>";
  html += "<form action='/ring_bell' method='POST'>";
  html += "Duration (seconds): <input type='number' name='duration' min='1' max='60' value='" + String(bellDuration / 1000) + "' required><br>";
  html += "<input type='submit' value='Ring Bell'></form>";
  html += "<a href='/'>Back to Home</a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleCurrentTime() {
  timeClient.update();
  String currentTime = timeClient.getFormattedTime();
  server.send(200, "text/plain", currentTime);
}

String getDayOfWeek() {
    timeClient.update();
    int dayOfWeek = timeClient.getDay();  // Assuming this function returns the day (0-6)
    String days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    return days[dayOfWeek];
}



void setup() {
  Serial.begin(115200);
  

  // Set up Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  
  int maxAttempts = 20; // Max attempts for connection
  int attempt = 0;

  while (WiFi.status() != WL_CONNECTED && attempt < maxAttempts) {
    delay(500);
    Serial.print(".");
    attempt++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect. Restarting...");
    ESP.restart(); // Restart the ESP if connection fails
  }

  Serial.println("\nConnected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Set up NTP client
  timeClient.begin();
  timeClient.forceUpdate(); // Force an immediate update after setup

  // Set up relay pin
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Relay off initially

  // Set up seismometer pin

  // Set up web server
  server.on("/", handleRoot);
  server.on("/set_time", HTTP_POST, handleSetTime);
  server.on("/ring_bell", HTTP_POST, handleRingBell);
  server.on("/manualring", HTTP_GET, handleManualRing);
  server.on("/time", HTTP_GET, handleCurrentTime);
  server.begin();
}

void ringBell(int duration) {
  digitalWrite(relayPin, LOW); // Turn on relay
  delay(duration);              // Keep it on for specified duration
  digitalWrite(relayPin, HIGH);  // Turn off relay
}


void loop() {
  server.handleClient(); // Handle web client requests

  timeClient.update();
  String currentTime = timeClient.getFormattedTime();
  
  int dayOfWeek = timeClient.getDay();

  // Check if today is Sunday (0 = Sunday)
  if (dayOfWeek != 0) {
    // Check if the current time matches any of the ring times
    for (int i = 0; i < maxRingTimes; i++) {
      if (currentTime.substring(0, 5) == ringTimes[i]) {
        Serial.println("Ringing"); // Print "ringing" to the serial port
        ringBell(10000); // Regular bell rings for 10 seconds
        Serial.println("stopped"); // Print "ringing stopped" to the serial port
        delay(60000); // Wait for 1 minute before checking again
        break; // Exit loop once bell rings
      }
    }
  } else {
    Serial.println("No ringing on Sundays"); // Print to serial if it's Sunday
  }

  delay(1000); // Check every second
}
