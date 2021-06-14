import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import (
    CONF_ID,
    CONF_PORT
);
from esphome.core import coroutine_with_priority, CORE
from esphome.components import esp32_camera


CODEOWNERS = ["@crossan007"]
DEPENDENCIES = ["network", "esp32_camera"]
AUTO_LOAD = ["async_tcp"]

rtsp_server_ns = cg.esphome_ns.namespace("rtsp_server")
RTSPServer = rtsp_server_ns.class_("RTSPServer", cg.Component)

CONF_RTSP_SERVER_ID = "rtsp_server_id"
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(RTSPServer),
        cv.Optional(CONF_PORT, default=8555): cv.port,
        cv.Required("camera"): cv.use_id(esp32_camera.ESP32Camera),
    }
)

@coroutine_with_priority(65.0)
def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    cg.add_library("ESPAsyncRTSPServer-esphome", "0.0.1")
    
    
    cg.add(var.set_port(config[CONF_PORT]))
    camera = yield cg.get_variable(config["camera"])
    cg.add(var.set_camera(camera))
