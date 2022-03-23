#include "json_config.h"

#include <QJsonValue>

#define KEY_FW "FIRMWARE"
#define KEY_FW_ID "id"
#define KEY_FW_VERSION "version"
#define KEY_FW_ADDR "address"

#define KEY_SCEN "SCENARIOS"
#define KEY_SCEN_ID "id"
#define KEY_SCEN_NAME "name"
#define KEY_SCEN_MIN_FW "min_fw_version"
#define KEY_SCEN_STATES "states"
#define KEY_SCEN_PARAMETERS "parameters"

#define KEY_PARAM "PARAMETERS"
#define KEY_PARAM_NAME "name"
#define KEY_PARAM_DESCRIPTION "description"
#define KEY_PARAM_ADDR "address"
#define KEY_PARAM_DEFAULT "default"

Json_config::Json_config() {}

Firmware::Firmware(QJsonObject jObj) {
    _firmware_array = jObj.value(KEY_FW).toArray();
}

QJsonObject Firmware::toJsonObject(QJsonObject jObj){
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

QJsonObject Scenario::toJsonObject(QJsonObject jObj) {
    jObj.insert(KEY_SCEN_ID, _scenario_id);
    jObj.insert(KEY_SCEN_NAME, _scenario_name);
    jObj.insert(KEY_SCEN_MIN_FW, _min_firmware_version);
    jObj.insert(KEY_SCEN_STATES, _scenario_states);
    jObj.insert(KEY_SCEN_PARAMETERS, _scenario_parameters);
    return jObj;
}

Parameters::Parameters(QJsonObject jObj) {
    _parameters_array = jObj.value(KEY_PARAM).toArray();
    parameters_array_size = _parameters_array.size();
    for (int i = 0; i < parameters_array_size; i++) {
        QJsonObject parameter_obj = _parameters_array[i].toObject();
        parameters_structs[i].parameter_name = parameter_obj.value(KEY_PARAM_NAME).toString();
        parameters_structs[i].parameter_description = parameter_obj.value(KEY_PARAM_DESCRIPTION).toString();
        parameters_structs[i].parameter_addr = parameter_obj.value(KEY_PARAM_ADDR).toString();
        parameters_structs[i].parameter_default_val = parameter_obj.value(KEY_PARAM_DEFAULT).toInt();
    }
}

QJsonObject Parameters::toJsonObject(QJsonObject jObj, Parameters::parameters_struct parameter) {
    jObj.insert(KEY_PARAM_NAME, parameter.parameter_name);
    jObj.insert(KEY_PARAM_DESCRIPTION, parameter.parameter_description);
    jObj.insert(KEY_PARAM_ADDR, parameter.parameter_addr);
    jObj.insert(KEY_PARAM_DEFAULT, parameter.parameter_default_val);
    return jObj;
}
