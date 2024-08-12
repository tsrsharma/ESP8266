#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Network credentials (default)
char ssid[] = "ESP8266-Network";
char password[] = "123456789";

// Set web server port number to 80
ESP8266WebServer server(80);

// Output pin
const int ledPin = 2; // GPIO2 is D4 on most ESP8266 boards

// LED state
bool ledState = LOW;

// Time tracking
unsigned long startTime;
bool hotspotActive = true;

// HTML for the web page
String webpage = "";

// Default credentials for login
String loginUsername = "admin";
String loginPassword = "admin";

// Flag to check if the user is logged in
bool loggedIn = false;

// Function to handle login
void handleLogin() {
  if (server.hasArg("username") && server.hasArg("password")) {
    if (server.arg("username") == loginUsername && server.arg("password") == loginPassword) {
      loggedIn = true;
      server.send(200, "text/html", "Login successful! <a href='/'>Go to the control page</a>");
    } else {
      server.send(401, "text/html", "Invalid credentials! <a href='/login'>Try again</a>");
    }
  } else {
    server.send(200, "text/html",
                "<html><head>"
                "<meta name='viewport' content='width=device-width, initial-scale=1'>"
                "<style>"
                "body { font-family: Arial, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }"
                ".login-container { text-align: center; border: 1px solid #ccc; padding: 20px; border-radius: 10px; background-color: #f9f9f9; }"
                "input[type='text'], input[type='password'] { width: 100%; padding: 12px 20px; margin: 8px 0; box-sizing: border-box; font-size: 18px; }"
                "input[type='submit'] { width: 100%; padding: 14px 20px; background-color: #4CAF50; color: white; border: none; border-radius: 4px; cursor: pointer; font-size: 18px; }"
                "input[type='submit']:hover { background-color: #45a049; }"
                "</style>"
                "</head><body>"
                "<div class='login-container'>"
                "<form action='/login' method='POST'>"
                "Username: <input type='text' name='username'><br>"
                "Password: <input type='password' name='password'><br>"
                "<input type='submit' value='Login'></form>"
                "</div>"
                "</body></html>");
  }
}

// Function to handle LED control
void handleRoot() {
  if (!loggedIn) {
    server.sendHeader("Location", "/login", true);
    server.send(302, "text/plain", "");
    return;
  }

  if (server.hasArg("led")) {
    String ledStateStr = server.arg("led");
    if (ledStateStr == "on") {
      digitalWrite(ledPin, HIGH);
      ledState = HIGH;
    } else if (ledStateStr == "off") {
      digitalWrite(ledPin, LOW);
      ledState = LOW;
    }
  }

  String ledStateStr = ledState ? "on" : "off";

  webpage = "<!DOCTYPE html><html>";
  webpage += "<head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  webpage += "<style>body { font-family: monospace; text-align: center; }";
  webpage += ".button { padding: 16px; font-size: 24px; margin: 4px; }";
  webpage += "</style></head>";
  webpage += "<body><h1>ESP8266 Control</h1>";
  webpage += "<p>LED is " + ledStateStr + "</p>";
  webpage += "<p><a href='/?led=on'><button class='button'>Turn ON</button></a></p>";
  webpage += "<p><a href='/?led=off'><button class='button'>Turn OFF</button></a></p>";
  webpage += "<h3>Settings</h3>";
  webpage += "<p><a href='/changeCredentials'><button class='button'>Change Login Credentials</button></a></p>";
  webpage += "<p><a href='/changeSSID'><button class='button'>Change SSID and Password</button></a></p>";
  webpage += "</body></html>";

  server.send(200, "text/html", webpage);
}

// Function to handle changing credentials
void handleChangeCredentials() {
  if (!loggedIn) {
    server.sendHeader("Location", "/login", true);
    server.send(302, "text/plain", "");
    return;
  }

  if (server.hasArg("newUsername") && server.hasArg("newPassword")) {
    loginUsername = server.arg("newUsername");
    loginPassword = server.arg("newPassword");
    server.send(200, "text/html", "Credentials updated! <a href='/'>Go back</a>");
  } else {
    server.send(200, "text/html",
                "<form action='/changeCredentials' method='POST'>"
                "New Username: <input type='text' name='newUsername'><br>"
                "New Password: <input type='password' name='newPassword'><br>"
                "<input type='submit' value='Update'></form>");
  }
}

// Function to handle changing SSID and password
void handleChangeSSID() {
  if (!loggedIn) {
    server.sendHeader("Location", "/login", true);
    server.send(302, "text/plain", "");
    return;
  }

  if (server.hasArg("newSSID") && server.hasArg("newPassword")) {
    strncpy(ssid, server.arg("newSSID").c_str(), sizeof(ssid) - 1);
    strncpy(password, server.arg("newPassword").c_str(), sizeof(password) - 1);
    server.send(200, "text/html", "SSID and password updated! Please reboot the device.");
  } else {
    server.send(200, "text/html",
                "<form action='/changeSSID' method='POST'>"
                "New SSID: <input type='text' name='newSSID'><br>"
                "New Password: <input type='password' name='newPassword'><br>"
                "<input type='submit' value='Update'></form>");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Start the ESP8266 in Access Point mode
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Start the web server
  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/changeCredentials", handleChangeCredentials);
  server.on("/changeSSID", handleChangeSSID);
  server.begin();

  // Start time for hotspot
  startTime = millis();
}

void loop() {
  unsigned long currentTime = millis();

  // Hotspot mode active for 5 seconds with blinking LED
  if (currentTime - startTime < 5000) {
    hotspotActive = true;
    digitalWrite(ledPin, millis() / 500 % 2);  // Blink LED every 500ms
  } else if (hotspotActive) {
    // Stop LED blinking after 5 seconds
    digitalWrite(ledPin, LOW);
    hotspotActive = false;
  }

  // Handle web server
  server.handleClient();
}
