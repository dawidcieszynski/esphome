import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID
from esphome.core import coroutine_with_priority, CORE

CODEOWNERS = ["@crossan007"]
DEPENDENCIES = ["network"]
AUTO_LOAD = ["async_tcp"]

rtsp_server_ns = cg.esphome_ns.namespace("rtsp_server")
RTSPServer = rtsp_server_ns.class_("RTSPServer", cg.Component)

CONF_RTSP_SERVER_ID = "rtsp_server_id"
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(RTSPServer),
    }
)


@coroutine_with_priority(65.0)
def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    # https://github.com/geeksville/Micro-RTSP
    cg.add_library("Micro-RTSP", "0.1.6")
