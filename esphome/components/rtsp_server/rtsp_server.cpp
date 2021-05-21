#include "rtsp_server.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include <StreamString.h>
#ifdef ARDUINO_ARCH_ESP32
#include <AsyncTCP.h>
#endif
#ifdef ARDUINO_ARCH_ESP8266
#include <ESPAsyncTCP.h>
#endif

namespace esphome {
namespace rtsp_server {

static const char *TAG = "rtsp_server";

void RTSPServer::setup() {
  
  
  AsyncServer* server = new AsyncServer(8554); // start listening on tcp port 8554
	server->onClient([](void *s, AsyncClient* c){
    
    if(c == NULL)
      return;
    ESP_LOGCONFIG(TAG, "Handling new RTSP server connection request");
    c->setRxTimeout(3);
  }, this);
	server->begin();

}


}  // namespace rtsp_server
}  // namespace esphome
