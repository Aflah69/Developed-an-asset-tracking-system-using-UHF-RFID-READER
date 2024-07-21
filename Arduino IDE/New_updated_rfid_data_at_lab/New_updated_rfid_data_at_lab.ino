#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <FS.h> // Include the SPIFFS library

#ifndef STASSID
#define STASSID "YOUR_SSID"
#define STAPSK "YOUR_PASSWORD"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);


struct ObjectDetails {
  String tagNumber;
  String objectName;
  String location;
  String homeland;
  bool permission;
};

ObjectDetails objects[10];
int numObjects = 0;

String page; // Declare the page variable at a global scope

void loadAssetDetails() {
  if (SPIFFS.begin()) {
    File file = SPIFFS.open("/asset_details.txt", "r");
    if (file) {
      while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.length() > 0) {
          String parts[5];
          int index = 0;
          int pos = 0;
          while ((pos = line.indexOf(',')) != -1) {
            parts[index++] = line.substring(0, pos);
            line = line.substring(pos + 1);
          }
          parts[index] = line;

          if (numObjects < 10) {
            objects[numObjects].tagNumber = parts[0];
            objects[numObjects].objectName = parts[1];
            objects[numObjects].location = parts[2];
            objects[numObjects].homeland = parts[3];
            objects[numObjects].permission = (parts[4] == "true");
            numObjects++;
          }
        }
      }
      file.close();
    }
    SPIFFS.end();
  }
}

void saveAssetDetails() {
  if (SPIFFS.begin()) {
    File file = SPIFFS.open("/asset_details.txt", "w");
    if (file) {
      for (int i = 0; i < numObjects; i++) {
        file.print(objects[i].tagNumber);
        file.print(',');
        file.print(objects[i].objectName);
        file.print(',');
        file.print(objects[i].location);
        file.print(',');
        file.print(objects[i].homeland);
        file.print(',');
        file.println(objects[i].permission ? "true" : "false");
      }
      file.close();
    }
    SPIFFS.end();
  }
}

void handleRoot() {
  page = "<!DOCTYPE html><html><head><title>Add Asset</title></head><body>";
  page += "<h1>Add Asset</h1>";
  page += "<form method='post' action='/addasset'>";
  page += "Tag Number: <input type='text' name='tagNumber'><br>";
  page += "Object Name: <input type='text' name='objectName'><br>";
  page += "Location: <input type='text' name='location'><br>";
  page += "Homeland: <input type='text' name='homeland'><br>";
  page += "Permission: <input type='checkbox' name='permission' value='true'> Allowed<br>";
  page += "<input type='submit' value='Submit'>";
  page += "</form>";
  page += "<hr>"; // Add a horizontal line for visual separation
  page += "<h2>Asset Details</h2>";
  page += "<table border='1'><tr><th>Tag Number</th><th>Object Name</th><th>Location</th><th>Homeland</th><th>Permission</th></tr>";

  for (int i = 0; i < numObjects; i++) {
    page += "<tr><td>" + objects[i].tagNumber + "</td><td>" + objects[i].objectName + "</td><td>" + objects[i].location + "</td><td>" + objects[i].homeland + "</td><td>" + (objects[i].permission ? "Allowed" : "Not Allowed") + "</td></tr>";
  }

  page += "</table></body></html>";

  server.send(200, "text/html", page);
}

void handleAddAsset() {
  String tagNumber = server.arg("tagNumber");
  String objectName = server.arg("objectName");
  String location = server.arg("location");
  String homeland = server.arg("homeland");
  bool permission = (server.arg("permission") == "true");

  if (numObjects < 10) {
    objects[numObjects].tagNumber = tagNumber;
    objects[numObjects].objectName = objectName;
    objects[numObjects].location = location;
    objects[numObjects].homeland = homeland;
    objects[numObjects].permission = permission;
    numObjects++;
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
  } else {
    server.send(200, "text/plain", "Maximum number of assets reached");
  }

  saveAssetDetails(); // Save the asset details after adding
}

void setup() {
  Serial.begin(9600);
  delay(10);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  loadAssetDetails(); // Load asset details when the device starts

  server.on("/", handleRoot);
  server.on("/addasset", HTTP_POST, handleAddAsset);
  server.begin();

  Serial.println("HTTP server started");

  
}

String receivedString; // Declare receivedString at a global scope

void loop() {
  // Handle HTTP requests
  server.handleClient();

  // Check if data is available to read from UART1 (SoftwareSerial)
  if (Serial.available() > 0) {
    // Read the data from the serial port into the global string variable
    receivedString = Serial.readStringUntil('\n');
    
    // Print the received string to the serial monitor
    Serial.print("Received: ");
    Serial.println(receivedString);
    
    // Transmit the received data to the ESP8266
    //espSerial.println(hexData);
    
    // Check if received data matches the RFID tag number
    for (int i = 0; i < numObjects; i++) {
      static int count = 2; // Declare count as static to retain its value across iterations
      if (receivedString == objects[i].tagNumber) {
        if (count % 2 == 0) {
          // Even count, item returns to original location
          objects[i].location = "lab1";
        } else {
          // Odd count, item exits from current location
          objects[i].location = "exit from " + objects[i].location;
        }
        count++;
        
        Serial.println("Location updated"); // Print debug message
        
        // Call handleRoot to update the server with the new location
        handleRoot();
        break; // Exit loop once the tag is found
      }
    }
  }
}
