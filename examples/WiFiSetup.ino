#include <ESP_WifiConfig.h>
#include <ESP8266WebServer.h>

ESP_WifiConfig wifi;
ESP8266WebServer server(80);

//ESP connected to the router successfully
void onSuccess(){
  Serial.println("ESP connected successfully");
  Serial.println(wifi.getIPAddressString());
}

//ESP failed to connect to the router
void onFailure(){
  Serial.println("Starting access point");
  wifi.startAccessPoint(); //default accessPoint credentials WifiConfig/123456789
  server.on("/", loginUI);
  server.on("/config", configHandler);
  server.begin();
}

void setup(){
  //Trying to connect to the router
  wifi.connectWifi(onSuccess, onFailure);
}

void loop(){
  server.handleClient();
}

//Get user's login info and save it in ROM
void configHandler(){
  String ssid;
  String password;

  for(uint8_t i = 0; i < server.args(); i++){
    if(server.argName(i) == "ssid"){
      ssid = server.arg(i);
    }
    if(server.argName(i) == "password"){
      password = server.arg(i);
    }
  }

  server.send(200, "application/json", "{\"success\":\"true\",\"message\":\"Information saved\"}");
  wifi.setWifi(ssid, password);
}

//Create WIFI login UI
void loginUI() {
  char page[] = "<html>"
                  "<head>"
                    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                    "<title>Basic WiFi Setup</title>"
                  "</head>"
                  "<body>"
                    "<style>.line{display:block;width:100%;}</style>"
                    "<form method=\"post\" action=\"config\">"
                      "<select class=\"line\" name=\"ssid\"><option>Select SSID</option></select>"
                      "<input class=\"line\" type=\"password\" name=\"password\">"
                      "<button class=\"line\" type=\"submit\">Enter</button>"
                    "</form>"
                    "<script>"
                      "var list = %LIST%;"
                      "function add(item, index){"
                        "var option = document.createElement(\"option\");"
                        "option.text = item;"
                        "document.querySelector(\"select\").add(option, index);"
                       "}"
                       "list.networks.forEach(function(item, index){add(item, index+1);});"
                      "</script>"
                  "</body>"
                "</html>";
    page.replace("%LIST%", wifi.nearAccessPoints());
    server.send(200, "text/html", page);
}
