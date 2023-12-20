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
const int NUM_DESC = 4; // Adjust this based on your requirements

NexText desc[NUM_DESC] = {
  NexText(0, 32, "desc1"),
  NexText(0, 30, "desc2"), // Add more as needed
  NexText(0,31,"desc3"),
  NexText(0,32,"desc4")
};

NexButton v1 = NexButton(0,6,"v1");
NexButton m1 = NexButton(0,7,"m1");
NexButton x1 = NexButton(0,8,"x1");
NexButton v2 = NexButton(0,25,"v2");
NexButton m2 = NexButton(0,26,"m2");
NexButton x2 = NexButton(0,24,"x2");
NexButton v3 = NexButton(0,13,"v3");
NexButton m3 = NexButton(0,12,"m3");
NexButton x3 = NexButton(0,11,"x3");
NexButton v4 = NexButton(0,21,"v4");
NexButton m4 = NexButton(0,22,"m4");
NexButton x4 = NexButton(0,23,"x4");
NexButton bUpdate = NexButton(0,2,"bUpdate");

NexTouch *nex_listen_list[] = {
  &v1, &v2, &v3, &v4,
  &m1, &m2, &m3, &m4,
  &x1, &x2, &x3, &x4,
  &bUpdate,
  NULL
};

void x1_Release(void *ptr) {
  updateOrderStatus ("complete", 0);
  desc[0].Set_background_color_bco(31);
}
void x2_Release(void *ptr) {
  updateOrderStatus ("complete", 1);
  desc[1].Set_background_color_bco(31);
}
void x3_Release(void *ptr) {
  updateOrderStatus ("complete", 2);
  desc[2].Set_background_color_bco(31);
}
void x4_Release(void *ptr) {
  updateOrderStatus ("complete", 3);
  desc[3].Set_background_color_bco(31);
}

void v1_Release(void *ptr) {
  updateOrderStatus ("pickup", 0);
  desc[0].Set_background_color_bco(34784);
}
void v2_Release(void *ptr) {
  updateOrderStatus ("pickup", 1);
  desc[1].Set_background_color_bco(34784);
}
void v3_Release(void *ptr) {
  updateOrderStatus ("pickup", 2);
  desc[2].Set_background_color_bco(34784);
}
void v4_Release(void *ptr) {
  updateOrderStatus ("pickup", 3);
  desc[3].Set_background_color_bco(34784);
}

void m1_Release(void *ptr) {
  updateOrderStatus("process", 0);
  desc[0].Set_background_color_bco(64520);
}
void m2_Release(void *ptr) {
  updateOrderStatus("process", 1);
  desc[1].Set_background_color_bco(64520);
}
void m3_Release(void *ptr) {
  updateOrderStatus("process", 2);
  desc[2].Set_background_color_bco(64520);
}
void m4_Release(void *ptr) {
  updateOrderStatus("process", 3);
  desc[3].Set_background_color_bco(64520);
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
  m1.attachPop(m1_Release, &m1);
  m2.attachPop(m2_Release, &m2);
  m3.attachPop(m3_Release, &m3);
  m4.attachPop(m4_Release, &m4);
  x1.attachPop(x1_Release, &x1);
  x2.attachPop(x2_Release, &x2);
  x3.attachPop(x3_Release, &x3);
  x4.attachPop(x4_Release, &x4);
  bUpdate.attachPop(bUpdatePressCallback, &bUpdate);
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
            Serial.print("Menu Name ");
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.println(menuName);
            desc[i].setText(menuName);
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