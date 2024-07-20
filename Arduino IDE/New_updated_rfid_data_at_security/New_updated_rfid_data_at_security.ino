# include <ArduinoJson.h>
# include <ESP8266WiFi.h>
# include <ESP8266WebServer.h>

# include <FS.h> // Include the SPIFFS library
# ifndef STASSID
# define STASSID "yo"
# define STAPSK " 12345678 "
# endif
# include <Arduino.h>
#if defined ( ESP32 )
# include <WiFi.h>
# elif defined (ESP8266)
# include <ESP8266WiFi.h>
# endif
# include <ESP_Mail_Client.h>
# define BUZZER_PIN 16
# define BUZZER_PIN 16
# define SMTP_HOST "smtp - mail . outlook .com"
# define SMTP_PORT 587
# define AUTHOR_EMAIL " asset_tracking@outlook . com "
# define AUTHOR_PASSWORD " 123456 "
# define RECIPIENT_EMAIL " aflah . kareem9123@gmail .com"

SMTPSession smtp ;
const char * ssid = STASSID ;
const char * password = STAPSK ;


ESP8266WebServer server(80);

struct ObjectDetails {
  String tagNumber;
  String objectName;
  String location;
  bool permission;
};

ObjectDetails objects[10]; // Assuming max 10 objects for simplicity
int numObjects = 0;

void smtpCallback(SMTP_Status status) 
{
  Serial.println(status.info());

  if (status.success()) 
    {
         Serial.println("----------------");
         Serial.printf("Message sent success: %d\n", status.completedCount());
         Serial.printf("Message sent failed: %d\n", status.failedCount());
         Serial.println("----------------\n");

        for (size_t i = 0; i < smtp.sendingResult.size(); i++) 
         {
           SMTP_Result result = smtp.sendingResult.getItem(i);
           Serial.println("Message No: " + String(i + 1));
           Serial.println("Status: " + String(result.completed ? "success" : "failed"));
           Serial.println("Date/Time: " + MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S"));
           // Serial.println("Recipient: " + result.recipients);
          //Serial.println("Subject: " + result.subject);
         }

      Serial.println("----------------\n");
      smtp.sendingResult.clear();
   }
}

void handleRoot() 
{
  String page = "<!DOCTYPE html><html><head><title>Add Asset</title></head><body>";
  page += "<h1>Add Asset</h1>";
  page += "<form method='post' action='/addasset'>";
  page += "Tag Number: <input type='text' name='tagNumber'><br>";
  page += "Object Name: <input type='text' name='objectName'><br>";
  page += "Location: <input type='text' name='location'><br>";
  page += "Permission: <input type='checkbox' name='permission' value='true'> Allowed<br>";
  page += "<input type='submit' value='Submit'>";
  page += "</form>";
  page += "<hr>"; // Add a horizontal line for visual separation
  page += "<h2>Add Another Asset</h2>"; // Add a header for the second asset
  page += "<form method='post' action='/addasset'>";
  page += "Tag Number: <input type='text' name='tagNumber'><br>";
  page += "Object Name: <input type='text' name='objectName'><br>";
  page += "Location: <input type='text' name='location'><br>";
  page += "Permission: <input type='checkbox' name='permission' value='true'> Allowed<br>";
  page += "<input type='submit' value='Submit'>";
  page += "</form>";
  page += "<hr>"; // Add a horizontal line for visual separation
  page += "<form method='post' action='/deleteall'>";
  page += "<input type='submit' value='Delete All Assets'>";
  page += "</form>";
  page += "</body></html>";

  server.send(200, "text/html", page);
}


void handleAddAsset() 
{
   String tagNumber = server.arg("tagNumber");
   String objectName = server.arg("objectName");
   String location = server.arg("location");
   bool permission = (server.arg("permission") == "true");

   if (numObjects < 10) 
    {
      objects[numObjects].tagNumber = tagNumber;
      objects[numObjects].objectName = objectName;
      objects[numObjects].location = location;
      objects[numObjects].permission = permission;
      numObjects++;

    // Save object details to SPIFFS
      saveObjectDetails();

    // Redirect to the page showing the asset details
      server.sendHeader("Location", "/assetdetails", true);
      server.send(302, "text/plain", ""); // Empty content to close the connection
    } 
    else 
    {
     server.send(200, "text/plain", "Maximum number of assets reached");
    }
}

void handleAssetDetails() 
{
  String page = "<!DOCTYPE html><html><head><title>Asset Details</title></head><body>";
  page += "<h1>Asset Details</h1>";
  page += "<table border='1'><tr><th>Tag Number</th><th>Object Name</th><th>Location</th><th>Permission</th></tr>";

   for (int i = 0; i < numObjects; i++) 
     {
       page += "<tr><td>" + objects[i].tagNumber + "</td><td>" + objects[i].objectName + "</td><td>" + objects[i].location + "</td><td>";

       if (objects[i].permission) 
        {
          page += "Allowed";
        }  
        else 
        {
           page += "Not Allowed";
        }
      page += "</td></tr>";
     }

  page += "</table></body></html>";
  server.send(200, "text/html", page);
}

void handleDeleteAll() 
{
   numObjects = 0; // Reset the number of objects to delete all items
   saveObjectDetails(); // Save the changes to SPIFFS
   server.sendHeader("Location", "/", true); // Redirect back to the main page
   server.send(302, "text/plain", ""); // Empty content to close the connection
}

void setup() 
{
  pinMode(BUZZER_PIN, OUTPUT); // Set BUZZER_PIN as output for buzzer
  digitalWrite(BUZZER_PIN, LOW);
  delay(1000);
  Serial.begin(9600);

   if (!SPIFFS.begin()) 
    {
      Serial.println("Failed to mount file system");
      return;
    }

  // Load object details from SPIFFS
  loadObjectDetails();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) 
     {
       delay(500);
       Serial.print(".");
     }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/addasset", HTTP_POST, handleAddAsset);
  server.on("/assetdetails", handleAssetDetails); // New route for asset details
  server.on("/deleteall", handleDeleteAll);       // Handler for deleting all assets
  server.begin();

  Serial.println("HTTP server started");
  server.begin();

  Serial.println("HTTP server started");
  MailClient.networkReconnect(true);
  smtp.debug(1);
  smtp.callback(smtpCallback);

  Session_Config config;
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  config.login.user_domain = "";

  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 3;
  config.time.day_light_offset = 0;

  SMTP_Message message;
  message.sender.name = F("ESP");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("Theft Alert");
  message.addRecipient(F("Recipient Name"), RECIPIENT_EMAIL);
  message.text.content = F("Alert! Theft detected!"); // Modify email content here

     if (!smtp.connect(&config)) 
      {
         Serial.printf("Connection error, Status Code: %d, Error Code: %d, Reason: %s\n", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
         return;
      }

     if (!smtp.isLoggedIn()) 
      {
        Serial.println("\nNot yet logged in.");
      }
      else 
      {
          if (smtp.isAuthenticated())
             Serial.println("\nSuccessfully logged in.");
         else
             Serial.println("\nConnected with no Auth.");
       }

    if (!MailClient.sendMail(&smtp, &message))
        Serial.printf("Error, Status Code: %d, Error Code: %d, Reason: %s\n", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
  }
}

 //espSerial.begin(9600); // Change baud rate if needed


 String receivedString;
void loop() 
 {
     // Handle HTTP requests
     server.handleClient();

     // Check if data is available to read from UART1 (SoftwareSerial)
  
      if (Serial.available() > 0) 
        {
             // Read the data from the serial port into the global string variable
             receivedString = Serial.readStringUntil('\n');

            // Print the received string to the serial monitor
             Serial.print("Received: ");
             Serial.println(receivedString);

           // Check permission and trigger buzzer if not allowed
             for (int i = 0; i < numObjects; i++) 
             {
                // Trim the received string to remove leading and trailing whitespace
                   receivedString.trim();

               // Trim the stored tag number to remove leading and trailing whitespace
                  String storedTagNumber = objects[i].tagNumber;
                  storedTagNumber.trim();

               // Compare the trimmed strings
             if (receivedString.equals(objects[i].tagNumber)) 
             {
               if (!objects[i].permission) 
                {
                    // Permission not allowed , trigger buzzer
                      digitalWrite ( BUZZER_PIN , HIGH ); // Turn on buzzer
                      sendTheftAlertEmail (); // Send theft alert email
                      delay (1000); // Buzzer on for 1 second
                      digitalWrite ( BUZZER_PIN , LOW ); // Turn off buzzer
                }
                  break ; // Exit the loop after finding a match
              }
            }
       }
  }

void saveObjectDetails() 
 {
    File file = SPIFFS.open("/object_details.txt", "w");
    if (!file) 
     {
       Serial.println("Failed to open file for writing");
       return;
     }

  StaticJsonDocument<1024> doc;
  doc["numObjects"] = numObjects;
  JsonArray objectsArray = doc.createNestedArray("objects");
    for (int i = 0; i < numObjects; i++) 
     {
       JsonObject object = objectsArray.createNestedObject();
       object["tagNumber"] = objects[i].tagNumber;
       object["objectName"] = objects[i].objectName;
       object["location"] = objects[i].location;
       object["permission"] = objects[i].permission;
     }

   if (serializeJson(doc, file) == 0) 
    {
      Serial.println("Failed to write to file");
    }

   file.close();
}

void loadObjectDetails() 
{
   File file = SPIFFS.open("/object_details.txt", "r");
   if (!file) 
    {
      Serial.println("Failed to open file for reading");
      return;
    }

  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, file);
   if (error) 
    {
      Serial.println("Failed to parse file");
      return;
    }

  numObjects = doc["numObjects"];
  JsonArray objectsArray = doc["objects"];
    for (int i = 0; i < numObjects; i++) 
    {
      JsonObject object = objectsArray[i];
      objects[i].tagNumber = object["tagNumber"].as<String>();
      objects[i].objectName = object["objectName"].as<String>();
      objects[i].location = object["location"].as<String>();
      objects[i].permission = object["permission"].as<bool>();
    }

  file.close();
}
