#ifdef ARDUINO_ARCH_ESP32

#pragma once

#include "esphome/core/component.h"
#include <AsyncRTSP.h>
#include "esphome/components/esp32_camera/esp32_camera.h"


namespace esphome {
namespace rtsp_server {

class RTSPServer : public Component {

  public:
    void setup() override;
    float get_setup_priority() const override;
    void dump_config() override;

    void loop();
    void set_port(uint16_t port);
    void set_camera(void* camera);
  
  private:
    uint16_t port_;
    esp32_camera::ESP32Camera* camera__;
    AsyncRTSPServer* server;
    dimensions parse_camera_dimensions_(camera_config_t config);
    dimensions dim;
};
}
}
#endif