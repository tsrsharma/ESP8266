# ESP8266 Hotspot with Web-Based LED Control and Login Authentication
Overview
This project demonstrates how to set up an ESP8266 microcontroller as a Wi-Fi hotspot, which hosts a web server. The web server allows users to connect to the ESP8266, log in with credentials, control an onboard LED, and modify the Wi-Fi SSID, password, and login credentials. 

Features
1. Hotspot Mode with Blinking LED:

i)   The ESP8266 starts in hotspot mode for 5 seconds after power-up.
ii)  During this period, the onboard LED blinks to indicate the hotspot is active.
iii) After 5 seconds, the LED stops blinking, but the hotspot remains active.

2. Webpage with Login Authentication:

i)  A login page secures access to the control panel.
ii) Default credentials are:
   - Username: admin
   - Password: admin
     
3. LED Control via Webpage:

 - The web interface allows users to turn the onboard LED on or off.
   
4. Change Login Credentials:

 - Users can change the login username and password through the web interface.
   
5. Change SSID and Password:

 - The SSID and password for the ESP8266 hotspot can be updated via the web interface.
   
Prerequisites
Hardware:

- ESP8266 development board (e.g., NodeMCU)
- USB cable for programming
- Optional: LED connected to GPIO2 (built-in LED on many boards)
  
Software:

- Visual Studio Code
- PlatformIO IDE extension for VS Code
