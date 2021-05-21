#include "rtsp_server.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include <StreamString.h>

#ifdef ARDUINO_ARCH_ESP32
#include <Update.h>
#endif
#ifdef ARDUINO_ARCH_ESP8266
#include <Updater.h>
#endif

namespace esphome {
namespace rtsp_server {

static const char *TAG = "rtsp_server";



}  // namespace rtsp_server
}  // namespace esphome
