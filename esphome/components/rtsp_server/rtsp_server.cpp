#include "rtsp_server.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include <StreamString.h>
#include "esphome/core/util.h"
#include "esphome/components/esp32_camera/esp32_camera.h"


namespace esphome {
namespace rtsp_server {

static const char *TAG = "rtsp_server";

void RTSPServer::setup() {
  ESP_LOGCONFIG(TAG, "Beginning to set up RTSP server listener");
  
  this->server = new AsyncRTSPServer(this->port_);

  this->server->onClient([](void *s) {
    ESP_LOGCONFIG(TAG, "Received RTSP connection");
  }, this);


  this->server->setLogFunction([](String s) {
    ESP_LOGCONFIG(TAG, s.c_str());
  }, this);

	
  ESP_LOGCONFIG(TAG, "Set up RTSP server listener, starting");
  try {
    this->server->begin();
    ESP_LOGCONFIG(TAG, "Started RTSP server listener");
  }
  catch (...) {
    ESP_LOGCONFIG(TAG, "Failed to start RTSP server listener");
    this->mark_failed();
  }


  if (esp32_camera::global_esp32_camera != nullptr) {
    esp32_camera::global_esp32_camera->add_image_callback([this](std::shared_ptr<esp32_camera::CameraImage> image) {
      ESP_LOGCONFIG(TAG, "RTSP server received camera frame");
      uint8_t* data = image->get_data_buffer();
      size_t length = image->get_data_length();
      //this->server->pushFrame(data, length);
    });
  }


  this->dump_config();
}


void RTSPServer::set_port(uint16_t port) { this->port_ = port; }
void RTSPServer::set_camera(void* camera) { this->camera__ = camera; }

void RTSPServer::dump_config() {
  ESP_LOGCONFIG(TAG, "RTSP Server:");
  ESP_LOGCONFIG(TAG, "  Address: %s:%u", network_get_address().c_str(), this->port_);
  ESP_LOGCONFIG(TAG, "  Camera Object: %p", this->camera__ );
 
}

float RTSPServer::get_setup_priority() const {

  return setup_priority::LATE;
}


}  // namespace rtsp_server
}  // namespace esphome
