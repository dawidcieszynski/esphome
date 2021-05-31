#include "rtsp_server.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include <StreamString.h>
#include "esphome/core/util.h"


namespace esphome {
namespace rtsp_server {

static const char *TAG = "rtsp_server";

void RTSPServer::setup() {

  if ( this->camera__ == nullptr) {
     ESP_LOGCONFIG(TAG, "Error: Unable to obtain reference to camera");
  }

  camera_config_t cameraConfig =  this->camera__->get_camera_config();

  dimensions dim = this->parseCameraDimensions(cameraConfig);

  ESP_LOGCONFIG(TAG, "Beginning to set up RTSP server listener");
  
  this->server = new AsyncRTSPServer(this->port_, dim);

  this->server->onClient([this](void *s) {
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


 
  this->camera__->add_image_callback([this](std::shared_ptr<esp32_camera::CameraImage> image) {
    uint8_t* data = image->get_data_buffer();
    size_t length = image->get_data_length();
    this->server->pushFrame(data, length);
  }); 

  this->dump_config();

}

void RTSPServer::loop() {
    if(this->server->hasClients()) {
      this->camera__->request_stream();
    }
}
dimensions RTSPServer::parseCameraDimensions(camera_config_t config){
  struct dimensions dim = {0, 0};
  switch (config.frame_size ) {
    case FRAMESIZE_QQVGA:
      dim.width = 160;
      dim.height = 120;
      break;
    case FRAMESIZE_QQVGA2:
      dim.width = 160;
      dim.height = 128;
      break;
    case FRAMESIZE_QCIF:
      dim.width = 176;
      dim.height = 144;
      break;
    /*case ESP32_CAMERA_SIZE_240X176:
      this->config_.frame_size = FRAMESIZE_HQVGA;
      break;
    case ESP32_CAMERA_SIZE_320X240:
      this->config_.frame_size = FRAMESIZE_QVGA;
      break;
    case ESP32_CAMERA_SIZE_400X296:
      this->config_.frame_size = FRAMESIZE_CIF;
      break;
    case ESP32_CAMERA_SIZE_640X480:
      this->config_.frame_size = FRAMESIZE_VGA;
      break;
    case ESP32_CAMERA_SIZE_800X600:
      this->config_.frame_size = FRAMESIZE_SVGA;
      break;
    case ESP32_CAMERA_SIZE_1024X768:
      this->config_.frame_size = FRAMESIZE_XGA;
      break;
    case ESP32_CAMERA_SIZE_1280X1024:
      this->config_.frame_size = FRAMESIZE_SXGA;
      break;
    case ESP32_CAMERA_SIZE_1600X1200:
      this->config_.frame_size = FRAMESIZE_UXGA;
      break;*/
  }
  return dim;
}

void RTSPServer::set_port(uint16_t port) { this->port_ = port; }
void RTSPServer::set_camera(void* camera) { this->camera__ = static_cast<esp32_camera::ESP32Camera* >(camera); }

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
