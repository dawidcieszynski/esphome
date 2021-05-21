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
  
  private:
    uint16_t port_;
};
}
}