#include "Nextion.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "TP-Link_2B43";
const char* password = "371A82447F";
String updtUrl = "https://kantech.vercel.app/api/vendor/order/get";
String pstUrl = "https://kantech.vercel.app/api/vendor/order/update";
String vendorId = "4a981015-7c76-469a-baf2-02c227f2dafc";
String orderIds[7];
unsigned long lastButtonPressTime = 0;  // Variable to store the last execution time

// Number of desc objects
const int NUM_DESC = 8; // Adjust this based on your requirements

NexText desc[NUM_DESC] = {
  NexText(0, 28, "desc1"),
  NexText(0, 29, "desc2"), // Add more as needed
  NexText(0, 30, "desc3"),
  NexText(0, 31, "desc4"),
  NexText(1, 26, "desc5"),
  NexText(1, 27, "desc6"), 
  NexText(1, 28, "desc7"),
  NexText(1, 29, "desc8")
};

NexButton v1 = NexButton(0,5,"v1");
NexButton m1 = NexButton(0,6,"m1");
NexButton x1 = NexButton(0,7,"x1");
NexButton v2 = NexButton(0,24,"v2");
NexButton m2 = NexButton(0,25,"m2");
NexButton x2 = NexButton(0,23,"x2");
NexButton v3 = NexButton(0,12,"v3");
NexButton m3 = NexButton(0,11,"m3");
NexButton x3 = NexButton(0,10,"x3");
NexButton v4 = NexButton(0,20,"v4");
NexButton m4 = NexButton(0,21,"m4");
NexButton x4 = NexButton(0,22,"x4");
NexButton v5 = NexButton(1,5,"v5");
NexButton m5 = NexButton(1,6,"m5");
NexButton x5 = NexButton(1,7,"x5");
NexButton v6 = NexButton(1,22,"v6");
NexButton m6 = NexButton(1,23,"m6");
NexButton x6 = NexButton(1,21,"x6");
NexButton v7 = NexButton(1,12,"v7");
NexButton m7 = NexButton(1,11,"m7");
NexButton x7 = NexButton(1,10,"x7");
NexButton v8 = NexButton(1,18,"v8");
NexButton m8 = NexButton(1,19,"m8");
NexButton x8 = NexButton(1,20,"x8");
NexButton bNext = NexButton(0,2,"bNext");
NexButton bBackt = NexButton(1,1,"bBackt");
NexButton bUpdate = NexButton(0,1,"bUpdate");
NexButton bUpdate1 = NexButton(1,2,"bUpdate1");

NexTouch *nex_listen_list[] = {
  &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8,
  &m1, &m2, &m3, &m4, &m5, &m6, &m7, &m8,
  &x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8,
  &bNext, &bBackt,
  &bUpdate, &bUpdate1,
  NULL
};

void x1_Release(void *ptr) {
  updateOrderStatus ("complete", 0);
  delay(1000);
  buttonPressCallback(bUpdate);  
}
void x2_Release(void *ptr) {
  updateOrderStatus ("complete", 1);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void x3_Release(void *ptr) {
  updateOrderStatus ("complete", 2);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void x4_Release(void *ptr) {
  updateOrderStatus ("complete", 3);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void x5_Release(void *ptr) {
  updateOrderStatus ("complete", 4);
  delay(1000);
  buttonPressCallback(bUpdate);  
}
void x6_Release(void *ptr) {
  updateOrderStatus ("complete", 5);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void x7_Release(void *ptr) {
  updateOrderStatus ("complete", 6);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void x8_Release(void *ptr) {
  updateOrderStatus ("complete", 7);
  delay(1000);
  buttonPressCallback(bUpdate);
}

void v1_Release(void *ptr) {
  updateOrderStatus ("pickup", 0);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void v2_Release(void *ptr) {
  updateOrderStatus ("pickup", 1);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void v3_Release(void *ptr) {
  updateOrderStatus ("pickup", 2);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void v4_Release(void *ptr) {
  updateOrderStatus ("pickup", 3);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void v5_Release(void *ptr) {
  updateOrderStatus ("pickup", 4);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void v6_Release(void *ptr) {
  updateOrderStatus ("pickup", 5);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void v7_Release(void *ptr) {
  updateOrderStatus ("pickup", 6);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void v8_Release(void *ptr) {
  updateOrderStatus ("pickup", 7);
  delay(1000);
  buttonPressCallback(bUpdate);
}

void m1_Release(void *ptr) {
  updateOrderStatus("process", 0);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void m2_Release(void *ptr) {
  updateOrderStatus("process", 1);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void m3_Release(void *ptr) {
  updateOrderStatus("process", 2);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void m4_Release(void *ptr) {
  updateOrderStatus("process", 3);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void m5_Release(void *ptr) {
  updateOrderStatus("process", 4);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void m6_Release(void *ptr) {
  updateOrderStatus("process", 5);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void m7_Release(void *ptr) {
  updateOrderStatus("process", 6);
  delay(1000);
  buttonPressCallback(bUpdate);
}
void m8_Release(void *ptr) {
  updateOrderStatus("process", 7);
  delay(1000);
  buttonPressCallback(bUpdate);
}

void bNextRelease(void *ptr) {
  buttonPressCallback(bNext);
  Serial.println("bNext button pressed");
}

void bBacktRelease(void *ptr) {
  buttonPressCallback(bBackt);
  Serial.println("bBackt button pressed");
}

void bUpdate1Release(void *ptr) {
  buttonPressCallback(bUpdate1);
}

void bUpdatePressCallback(void *ptr) {
  Serial.println("bUpdate button pressed");

  // Trigger update
  buttonPressCallback(bUpdate);
}

void updateOrderStatus(const char *newStatus, int descIndex) {
  // Check if the index is within the array bounds
  if (descIndex >= 0 && descIndex < NUM_DESC) {
    // Send the API request to update order status
    String requestBody = "{\"vendor_id\":\"" + vendorId + "\",\"order_id\":\"" + orderIds[descIndex] + "\",\"status\":\"" + newStatus + "\"}";

    // Send the POST request
    HTTPClient http;
    http.begin(pstUrl);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(requestBody);

    if (httpResponseCode > 0) {
      String response = http.getString();

      // Parse JSON response (optional)
      // You can parse and handle the response if needed

      Serial.print("Order status updated to ");
      Serial.println(newStatus);
    } else {
      Serial.print("HTTP error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Invalid index for updating order status.");
  }
}

void setup() {
  // put your setup code here, to run once:
  nexInit();

  // Connect to Wi-Fi
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  v1.attachPop(v1_Release, &v1);
  v2.attachPop(v2_Release, &v2);
  v3.attachPop(v3_Release, &v3);
  v4.attachPop(v4_Release, &v4);
  v5.attachPop(v5_Release, &v5);
  v6.attachPop(v6_Release, &v6);
  v7.attachPop(v7_Release, &v7);
  v8.attachPop(v8_Release, &v8);
  m1.attachPop(m1_Release, &m1);
  m2.attachPop(m2_Release, &m2);
  m3.attachPop(m3_Release, &m3);
  m4.attachPop(m4_Release, &m4);
  m5.attachPop(m5_Release, &m5);
  m6.attachPop(m6_Release, &m6);
  m7.attachPop(m7_Release, &m7);
  m8.attachPop(m8_Release, &m8);
  x1.attachPop(x1_Release, &x1);
  x2.attachPop(x2_Release, &x2);
  x3.attachPop(x3_Release, &x3);
  x4.attachPop(x4_Release, &x4);
  x5.attachPop(x5_Release, &x5);
  x6.attachPop(x6_Release, &x6);
  x7.attachPop(x7_Release, &x7);
  x8.attachPop(x8_Release, &x8);
  bUpdate.attachPop(bUpdatePressCallback, &bUpdate);
  bNext.attachPop(bNextRelease, &bNext);
  bBackt.attachPop(bBacktRelease, &bBackt);
  bUpdate1.attachPop(bUpdate1Release, &bUpdate1);
}

void loop() {
  // put your main code here, to run repeatedly:
  nexLoop(nex_listen_list);
  // Get the current time
  unsigned long currentTime = millis();

  // Check if 1 minute (60,000 milliseconds) has passed since the last execution
  if (currentTime - lastButtonPressTime >= 60000) {
    // Update the last execution time
    lastButtonPressTime = currentTime;

    // Call the function
    buttonPressCallback(bUpdate);
  }
}

void buttonPressCallback(NexButton& button) {
  String requestBody = "{\"vendor_id\":\"" + vendorId + "\"}";

  // Send the POST request
  HTTPClient http;
  http.begin(updtUrl);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(requestBody);

  if (httpResponseCode > 0) {
    String response = http.getString();

    // Parse JSON response
    DynamicJsonDocument doc(2048); // Adjust the buffer size based on your response size
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
      Serial.print("JSON parsing error: ");
      Serial.println(error.c_str());
    } else {
      // Check if the response is an array
      if (doc.is<JsonArray>()) {
        JsonArray orders = doc.as<JsonArray>();

        // Clear the existing order IDs before storing new ones
        memset(orderIds, 0, sizeof(orderIds));

        // Iterate over the received orders
        for (int i = 0; i < min((size_t)7, orders.size()); i++) {
          orderIds[i] = orders[i]["id"].as<String>();
          Serial.print("Order ID ");
          Serial.print(i + 1);
          Serial.print(": ");
          Serial.println(orderIds[i]);
        }

        // Iterate over the desc objects and update their content
        for (int i = 0; i < NUM_DESC; i++) {
          if (i < orders.size()) {
            // Data available, update the desc text
            JsonObject order = orders[i];
            const char* menuName = order["menu"]["name"];
            const char* status = order["status"];
            Serial.print("Menu Name ");
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.println(menuName);
            Serial.print("Status ");
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.println(status);
            
            desc[i].setText(menuName);

            // Set background color based on status
            if (strcmp(status, "process") == 0) {
              desc[i].Set_background_color_bco(64520);
            } else if (strcmp(status, "pickup") == 0) {
              desc[i].Set_background_color_bco(34784);
            } else {
              // Handle other status values if needed
              // For now, set a default background color
              desc[i].Set_background_color_bco(50712);
            }
          } else {
            // No data available, set desc text to blank
            desc[i].setText("");
            desc[i].Set_background_color_bco(50712);
          } 
        }
      } else {
        Serial.println("Invalid JSON format - Expected an array.");
      }
    }
  } else {
    Serial.print("HTTP error: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}