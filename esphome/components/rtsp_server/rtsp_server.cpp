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
  ESP_LOGCONFIG(TAG, "Beginning to set up RTSP server listener");
  
  AsyncServer* server = new AsyncServer(8554); // start listening on tcp port 8554
	server->onClient([](void *s, AsyncClient* c){
    
    if(c == NULL)
      return;
    ESP_LOGCONFIG(TAG, "Handling new RTSP server connection request");
    c->setRxTimeout(3);
  }, this);
  ESP_LOGCONFIG(TAG, "Set up RTSP server listener, starting");
  try {
    server->begin();
    ESP_LOGCONFIG(TAG, "Started RTSP server listener");
  }
  catch (...) {
    ESP_LOGCONFIG(TAG, "Failed to start RTSP server listener");
    this->mark_failed();
  }

}

float RTSPServer::get_setup_priority() const {

  return setup_priority::LATE;
}


}  // namespace rtsp_server
}  // namespace esphome
