#pragma once

#include "esphome/core/component.h"


namespace esphome {
namespace rtsp_server {

class RTSPServer : public Component {

  public:
    void setup() override;
    float get_setup_priority() const override;
    void dump_config() override;

  void set_port(uint16_t port);
  void set_camera(void* camera);
  
  private:
    uint16_t port_;
    void* camera__;
};
}
}