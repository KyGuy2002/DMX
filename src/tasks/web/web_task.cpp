#include "web_task.h"




void createWebTask() {
  

  if(xSemaphoreTake(xEthernetMutex, portMAX_DELAY) != pdTRUE) {
    Serial1.println("[Web Task] Failed to take Ethernet mutex during initialization!");
    vTaskDelete(NULL);
    return;
  }

  
  // Init
  Serial1.println("[Web Task] Starting web server initialization...");
  server.begin();
  Serial1.println("[Web Task] HTTP server started on port 80");
  

  xSemaphoreGive(xEthernetMutex);
  

  xTaskCreate(
    webTask,        // Task function
    "Web",         // Task name
    WEB_TASK_STACK_SIZE,    // Stack size (bytes) - larger for web processing
    NULL,                     // Parameters
    WEB_TASK_PRIORITY,      // Priority (higher for web)
    NULL
  );
  
}


void webTask(void *pvParameters) {
  while (1) {

    // Skip if mutex not available
    if (xSemaphoreTake(xEthernetMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(10));
      continue;
    }
    
    // Listen for incoming clients
    EthernetClient client = server.available();

    // Avoid bool conversion on EthernetClient; only proceed when data is ready.
    int initialAvailable = client.available();
    if (initialAvailable > 0) {
      Serial1.println("[HTTP Task] New client connected");

      String currentLine = "";
      bool responseSent = false;
      bool sawBlankLine = false;
      uint32_t requestStartMs = millis();

      // Keep request parsing bounded so this task cannot monopolize the SPI mutex.
      // Do not rely on client.connected(), which can stall on some socket states.
      while (!responseSent) {
        if ((millis() - requestStartMs) > 250) {
          Serial1.println("[HTTP Task] Request timeout");
          break;
        }

        if (client.available()) {
          char c = client.read();
          if (c == '\n') {
            if (currentLine.length() == 0) {
              // End of headers: send a default response if nothing matched yet.
              if (!responseSent) {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/html");
                client.println("Connection: close");
                client.println();
                responseSent = true;
              }
              sawBlankLine = true;
            } else {
              // Parse first line for path: "GET /path HTTP/1.1"
              if (currentLine.startsWith("GET")) {
                int firstSpace = currentLine.indexOf(' ');
                int secondSpace = currentLine.indexOf(' ', firstSpace + 1);
                String path = currentLine.substring(firstSpace + 1, secondSpace);
                Serial1.println("Requested Path: " + path);

                if (path == "/") {
                  // Serve main page
                  client.println("HTTP/1.1 200 OK");
                  client.println("Content-Type: text/html");
                  client.println("Connection: close");
                  client.println();
                  client.println("<html><body><h1>ProjectDMX Controller</h1><p>Welcome to the ProjectDMX Controller HTTP Interface!</p><br/><br/><br/><button onclick=\"window.location.href='/api/volume/up'\">Volume Up</button><button onclick=\"window.location.href='/api/volume/down'\">Volume Down</button><button onclick=\"window.location.href='/api/music/pause'\">Pause</button></body></html>");
                  responseSent = true;
                } else if (path == "/api/volume/up") {
                  // Trigger volume up action
                  Serial1.println("========= Volume up button pressed");

                  // Actually increase volume (volume range is 0.0 to 1.0)
                  float currentVol = volume.volume();
                  float newVol = min(currentVol + 0.1f, 1.0f); // Increment by 0.1, cap at 1.0
                  volume.setVolume(newVol);
                  Serial1.print("Volume: ");
                  Serial1.print(currentVol);
                  Serial1.print(" -> ");
                  Serial1.println(newVol);

                  // Redirect back to main page
                  client.println("HTTP/1.1 302 Found");
                  client.println("Location: /");
                  client.println("Connection: close");
                  client.println();
                  responseSent = true;
                } else if (path == "/api/volume/down") {
                  // Trigger volume down action
                  Serial1.println("========= Volume down button pressed");

                  // Actually decrease volume (volume range is 0.0 to 1.0)
                  float currentVol = volume.volume();
                  float newVol = max(currentVol - 0.1f, 0.0f); // Decrement by 0.1, cap at 0.0
                  volume.setVolume(newVol);
                  Serial1.print("Volume: ");
                  Serial1.print(currentVol);
                  Serial1.print(" -> ");
                  Serial1.println(newVol);

                  // Redirect back to main page
                  client.println("HTTP/1.1 302 Found");
                  client.println("Location: /");
                  client.println("Connection: close");
                  client.println();
                  responseSent = true;
                } else if (path == "/api/music/pause") {
                  // Trigger pause action
                  Serial1.println("========= Pause button pressed");

                  decoder.end();
                  Serial1.println("Music paused");

                  // Redirect back to main page
                  client.println("HTTP/1.1 302 Found");
                  client.println("Location: /");
                  client.println("Connection: close");
                  client.println();
                  responseSent = true;
                }
              }
              currentLine = "";
            }
            if (sawBlankLine) {
              break;
            }
          } else if (c != '\r') {
            currentLine += c; // Build line
          }
        } else {
          // Yield briefly while waiting for bytes so we do not spin forever.
          vTaskDelay(pdMS_TO_TICKS(1));
        }
      }

      // Give the browser time to receive data, then close the connection once.
      vTaskDelay(pdMS_TO_TICKS(1));
      client.stop();
      Serial1.println("[HTTP Task] Client disconnected");
    }
    



    // Give mutex back
    xSemaphoreGive(xEthernetMutex);

    // Briefly block to allow lower-priority networking tasks to run
    vTaskDelay(pdMS_TO_TICKS(50));
    
  }
}