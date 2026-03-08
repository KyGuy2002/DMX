#include "task_http.h"
#include "../peripherals/peripheral_init.h"
#include "../rtos_config.h"
#include <Ethernet3.h>

TaskHandle_t httpTaskHandle = NULL;

void httpServerTask(void *pvParameters) {
  Serial1.println("[HTTP Task] Started");
  
  while (1) {



    // Skip if mutex not available
    if (xSemaphoreTake(xSPIMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(10));
      continue;
    }



    // Listen for incoming clients
    EthernetClient client = server.available();
    
    if (client) {
      Serial1.println("[HTTP Task] New client connected");
      
      if (client) {
        String currentLine = "";
        while (client.connected()) {
          if (client.available()) {
            char c = client.read();
            if (c == '\n') {
              if (currentLine.length() == 0) {
                // End of request, send response
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/html");
                client.println();
                client.stop();
                break;
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
                    client.println();
                    client.println("<html><body><h1>ProjectDMX Controller</h1><p>Welcome to the ProjectDMX Controller HTTP Interface!</p><br/><br/><br/><button onclick=\"window.location.href='/api/volume/up'\">Volume Up</button><button onclick=\"window.location.href='/api/volume/down'\">Volume Down</button><button onclick=\"window.location.href='/api/music/pause'\">Pause</button></body></html>");
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
                  }
                   else if (path == "/api/volume/down") {
                    // Trigger volume down action
                    Serial1.println("========= Volume down button pressed");


                    // Actually increase volume (volume range is 0.0 to 1.0)
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
                  }
                  else if (path == "/api/music/pause") {
                    // Trigger pause action
                    Serial1.println("========= Pause button pressed");


                    // Actually increase volume (volume range is 0.0 to 1.0)
                    decoder.end();
                    Serial1.println("Music paused");


                    // Redirect back to main page
                    client.println("HTTP/1.1 302 Found");
                    client.println("Location: /");
                  }
                }
                currentLine = "";
              }
            } else if (c != '\r') {
              currentLine += c; // Build line
            }
          }
        }
        client.stop();
      }
      
      // Give the web browser time to receive the data
      vTaskDelay(pdMS_TO_TICKS(1));
      
      // Close the connection
      client.stop();
      Serial1.println("[HTTP Task] Client disconnected");
    }

    // Give mutex back
    xSemaphoreGive(xSPIMutex);
    
    // Yield to other tasks
    vTaskDelay(pdMS_TO_TICKS(HTTP_TASK_DELAY_MS));
  }
}

void createHttpTask() {
  xTaskCreate(
    httpServerTask,           // Task function
    "HTTP Server",            // Task name
    HTTP_TASK_STACK_SIZE,     // Stack size (bytes)
    NULL,                     // Parameters
    HTTP_TASK_PRIORITY,       // Priority
    &httpTaskHandle           // Task handle
  );
}
