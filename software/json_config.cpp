#include "json_config.h"

#include <QJsonValue>

#define KEY_CONNECTION "CONNECTION"
#define KEY_IP "IP"
#define KEY_PORT "PORT"

#define KEY_FW "FIRMWARE"
#define KEY_FW_VERSION "version"
#define KEY_FW_SYSID "sysid"
#define KEY_FW_CLOCK "clock"

#define KEY_SCEN "SCENARIO"
#define KEY_MIN_FW "min_fw_version"
#define KEY_STATES "states"
#define KEY_PARAMETERS "parameters"

#define KEY_PARAM "PARAMETERS"
#define KEY_MEAS "MEASUREMENTS"
#define KEY_NAME "name"
#define KEY_DESCRIPTION "description"
#define KEY_ADDR "address"
#define KEY_INTERVAL "interval"
#define KEY_DEFAULT "default"

QString Json_config::IP = "0";
int Json_config::PORT = 0;

Json_config::Json_config() {}

Json_config::Json_config(QJsonObject jObj) {
    QJsonObject obj = jObj.value(KEY_CONNECTION).toObject();
    IP = obj.value(KEY_IP).toString();
    PORT = obj.value(KEY_PORT).toInt();

    firmware_init(jObj);
    scenario_init(jObj);
    parameters_init(jObj);
    measurment_init(jObj);
}

QString Json_config::getIP() {
    return IP;
}

int Json_config::getPORT() {
    return PORT;
}

void Json_config::firmware_init(QJsonObject jObj) {
    QJsonArray array = jObj.value(KEY_FW).toArray();
    for (int i = 0; i < array.size(); i++) {
        QJsonObject obj = array[i].toObject();
        firmware_map[i].version = obj.value(KEY_FW_VERSION).toString();
        firmware_map[i].sysid = obj.value(KEY_FW_SYSID).toString();
        firmware_map[i].clock = obj.value(KEY_FW_CLOCK).toString();
    }
}

QJsonObject Json_config::toJsonObject(QJsonObject jObj, QMap<int, firmware_struct> firmware) {
    for (int i = 0; i < firmware.size(); i++) {
        QJsonObject obj;
        obj.insert(KEY_FW_VERSION, firmware[i].version);
        obj.insert(KEY_FW_SYSID, firmware[i].sysid);
        obj.insert(KEY_FW_CLOCK, firmware[i].clock);
        jObj.insert(KEY_FW, obj);
    }
    return jObj;
}

void Json_config::scenario_init(QJsonObject jObj) {
    QJsonArray array = jObj.value(KEY_SCEN).toArray();
    for (int i = 0; i < array.size(); i++) {
        QJsonObject obj = array[i].toObject();
        scenario_map[i].name = obj.value(KEY_NAME).toString();
        scenario_map[i].min_firmware_version = obj.value(KEY_MIN_FW).toString();
        scenario_map[i].states = obj.value(KEY_STATES).toArray();
        scenario_map[i].parameters = obj.value(KEY_PARAMETERS).toArray();
    }
}

QJsonObject Json_config::toJsonObject(QJsonObject jObj, QMap<int, scenario_struct> scenario) {
    for (int i = 0; i < scenario.size(); i++) {
        QJsonObject obj;
        obj.insert(KEY_NAME, scenario[i].name);
        obj.insert(KEY_MIN_FW, scenario[i].min_firmware_version);
        obj.insert(KEY_STATES, scenario[i].states);
        obj.insert(KEY_PARAMETERS, scenario[i].parameters);
        jObj.insert(KEY_SCEN, obj);

    }
    return jObj;
}

void Json_config::parameters_init(QJsonObject jObj) {
    QJsonArray array = jObj.value(KEY_PARAM).toArray();
    for (int i = 0; i < array.size(); i++) {
        QJsonObject obj = array[i].toObject();
        parameters_map[i].name = obj.value(KEY_NAME).toString();
        parameters_map[i].description = obj.value(KEY_DESCRIPTION).toString();
        parameters_map[i].address = obj.value(KEY_ADDR).toString();
        parameters_map[i].default_val = obj.value(KEY_DEFAULT).toString();
    }
}

QJsonObject Json_config::toJsonObject(QJsonObject jObj, QMap<int, parameters_struct> parameter) {
    for (int i = 0; i < parameter.size(); i++) {
        QJsonObject obj;
        obj.insert(KEY_NAME, parameter[i].name);
        obj.insert(KEY_DESCRIPTION, parameter[i].description);
        obj.insert(KEY_ADDR, parameter[i].address);
        obj.insert(KEY_DEFAULT, parameter[i].default_val);
        jObj.insert(KEY_PARAM, obj);
    }
    return jObj;
}

void Json_config::measurment_init(QJsonObject jObj) {
    QJsonArray array = jObj.value(KEY_MEAS).toArray();
    for (int i = 0; i < array.size(); i++) {
        QJsonObject obj = array[i].toObject();
        measurment_map[i].name = obj.value(KEY_NAME).toString();
        measurment_map[i].description = obj.value(KEY_DESCRIPTION).toString();
        measurment_map[i].interval = obj.value(KEY_INTERVAL).toString();
        measurment_map[i].address = obj.value(KEY_ADDR).toString();
    }
}

QJsonObject Json_config::toJsonObject(QJsonObject jObj, QMap<int, measurment_struct> measurment) {
    for (int i = 0; i < measurment.size(); i++) {
        QJsonObject obj;
        obj.insert(KEY_NAME, measurment[i].name);
        obj.insert(KEY_DESCRIPTION, measurment[i].description);
        obj.insert(KEY_ADDR, measurment[i].address);
        jObj.insert(KEY_MEAS, obj);
    }
    return jObj;
}
