#ifdef ARDUINO_ARCH_ESP32
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

  this->dim = this->parse_camera_dimensions_(cameraConfig);

  ESP_LOGCONFIG(TAG, "Beginning to set up RTSP server listener");
  
  this->server = new AsyncRTSPServer(this->port_, this->dim);

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
      // this asks the camera to fetch images and invoke our callback
      // when it has images ready
      this->camera__->request_stream(); 
    }
}
dimensions RTSPServer::parse_camera_dimensions_(camera_config_t config){
  struct dimensions dim = {0, 0};
  switch (config.frame_size ) {
    case FRAMESIZE_QQVGA:
      dim.width = 160;
      dim.height = 120;
      break;
    case FRAMESIZE_QCIF:
      dim.width = 176;
      dim.height = 144;
      break;
    case FRAMESIZE_HQVGA:
      dim.width = 240;
      dim.height = 176;
      break;
    case FRAMESIZE_QVGA:
      dim.width = 320;
      dim.height = 240;
      break;
    case FRAMESIZE_CIF:
      dim.width = 400;
      dim.height = 296;
      break;
    case FRAMESIZE_VGA:
      dim.width = 640;
      dim.height = 480;
      break;
    case FRAMESIZE_SVGA:
      dim.width = 800;
      dim.height = 600;
      break;
    case FRAMESIZE_XGA:
      dim.width = 1024;
      dim.height = 768;
      break;
    case FRAMESIZE_SXGA:
      dim.width = 1280;
      dim.height = 1024;
      break;
    case FRAMESIZE_UXGA:
      dim.width = 1600;
      dim.height = 1200;
      break;
  }
  return dim;
}

void RTSPServer::set_port(uint16_t port) { this->port_ = port; }
void RTSPServer::set_camera(void* camera) { this->camera__ = static_cast<esp32_camera::ESP32Camera* >(camera); }

void RTSPServer::dump_config() {
  ESP_LOGCONFIG(TAG, "RTSP Server:");
  ESP_LOGCONFIG(TAG, "  Address: %s:%u", network_get_address().c_str(), this->port_);
  ESP_LOGCONFIG(TAG, "  Camera Object: %p", this->camera__ );
  ESP_LOGCONFIG(TAG, "  width: %d height: %d ", this->dim.width, this->dim.height  );
 
}

float RTSPServer::get_setup_priority() const {

  return setup_priority::LATE;
}


}  // namespace rtsp_server
}  // namespace esphome
#endif