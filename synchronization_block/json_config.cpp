#include "json_config.h"

#include <QJsonValue>

#define KEY_FW_ID "id"
#define KEY_FW_VERSION "version"
#define KEY_FW_ADDR "address"

#define KEY_SCEN_ID "id"
#define KEY_SCEN_NAME "name"
#define KEY_SCEN_MIN_FW "min_fw_version"
#define KEY_SCEN_STATES "states"
#define KEY_SCEN_PARAMETERS "parameters"

#define KEY_PARAM_NAME "name"
#define KEY_PARAM_DESCRIPTION "description"
#define KEY_PARAM_ADDR "address"
#define KEY_PARAM_DEFAULT "default"

Json_config::Json_config() {}

Firmware::Firmware(QJsonObject jObj) {
    _firmware_id = jObj.value(KEY_FW_ID).toInt();
    _firmware_version = jObj.value(KEY_FW_VERSION).toString();
    _firmware_addr = jObj.value(KEY_FW_ADDR).toString();
}

QJsonObject Firmware::toJsonObject(){
    QJsonObject jObj;
    jObj.insert(KEY_FW_ID, _firmware_id);
    jObj.insert(KEY_FW_VERSION, _firmware_version);
    jObj.insert(KEY_FW_ADDR, _firmware_addr);
    return jObj;
}

Scenario::Scenario(QJsonObject jObj) {
    _scenario_id = jObj.value(KEY_SCEN_ID).toInt();
    _scenario_name = jObj.value(KEY_SCEN_NAME).toString();
    _min_firmware_version = jObj.value(KEY_SCEN_MIN_FW).toString();
    _scenario_states = jObj.value(KEY_SCEN_STATES).toArray();
    _scenario_parameters = jObj.value(KEY_SCEN_PARAMETERS).toArray();
}

QJsonObject Scenario::toJsonObject() {
    QJsonObject jObj;
    jObj.insert(KEY_SCEN_ID, _scenario_id);
    jObj.insert(KEY_SCEN_NAME, _scenario_name);
    jObj.insert(KEY_SCEN_MIN_FW, _min_firmware_version);
    jObj.insert(KEY_SCEN_STATES, _scenario_states);
    jObj.insert(KEY_SCEN_PARAMETERS, _scenario_parameters);
    return jObj;
}

Parameters::Parameters(QJsonObject jObj) {
    _parameter_name = jObj.value(KEY_PARAM_NAME).toString();
    _parameter_description = jObj.value(KEY_PARAM_DESCRIPTION).toString();
    _parameter_addr = jObj.value(KEY_PARAM_ADDR).toString();
    _parameter_default_val = jObj.value(KEY_PARAM_DEFAULT).toInt();
}

void Parameters::get() {
    parameter_name = _parameter_name;
    parameter_description = _parameter_description;
    parameter_addr = _parameter_addr;
    parameter_default_val = _parameter_default_val;
}

void Parameters::set(QString parameter_name, QString parameter_description, QString parameter_addr, int parameter_default_val) {
    _parameter_name = parameter_name;
    _parameter_description = parameter_description;
    _parameter_addr = parameter_addr;
    _parameter_default_val = parameter_default_val;
}

QJsonObject Parameters::toJsonObject() {
    QJsonObject jObj;
    jObj.insert(KEY_PARAM_NAME, _parameter_name);
    jObj.insert(KEY_PARAM_DESCRIPTION, _parameter_description);
    jObj.insert(KEY_PARAM_ADDR, _parameter_addr);
    jObj.insert(KEY_PARAM_DEFAULT, _parameter_default_val);
    return jObj;
}
