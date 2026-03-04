#include "task_http.h"
#include "../peripherals/peripheral_init.h"
#include "../rtos_config.h"
#include <Ethernet3.h>

TaskHandle_t httpTaskHandle = NULL;

void httpServerTask(void *pvParameters) {
  Serial1.println("[HTTP Task] Started");
  
  while (1) {
    // Listen for incoming clients
    EthernetClient client = server.available();
    
    if (client) {
      Serial1.println("[HTTP Task] New client connected");
      
      bool currentLineIsBlank = true;
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          
          // If you've gotten to the end of the line (received a newline
          // character) and the line is blank, the HTTP request has ended
          if (c == '\n' && currentLineIsBlank) {
            // Send a standard HTTP response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println("Refresh: 5");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<h1>ProjectDMX Controller</h1>");
            client.print("<p>Hostname: ");
            client.print(MDNS_NAME);
            client.println("</p>");
            client.print("<p>Uptime: ");
            client.print(millis() / 1000);
            client.println(" seconds</p>");
            
            // Output analog input pins
            for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
              int sensorReading = analogRead(analogChannel);
              client.print("<p>Analog input ");
              client.print(analogChannel);
              client.print(" is ");
              client.print(sensorReading);
              client.println("</p>");
            }
            
            client.println("</html>");
            break;
          }
          
          if (c == '\n') {
            currentLineIsBlank = true;
          } else if (c != '\r') {
            currentLineIsBlank = false;
          }
        }
      }
      
      // Give the web browser time to receive the data
      vTaskDelay(pdMS_TO_TICKS(1));
      
      // Close the connection
      client.stop();
      Serial1.println("[HTTP Task] Client disconnected");
    }
    
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
