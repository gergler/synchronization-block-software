#include "json_config.h"

#include <QJsonValue>

#define KEY_FW "FIRMWARE"
#define KEY_FW_ID "id"
#define KEY_FW_VERSION "version"
#define KEY_FW_ADDR "address"

#define KEY_SCEN "SCENARIO"
#define KEY_SCEN_ID "id"
#define KEY_SCEN_NAME "name"
#define KEY_SCEN_MIN_FW "min_fw_version"
#define KEY_SCEN_STATES "states"
#define KEY_SCEN_PARAMETERS "scen_parameters"

#define KEY_PARAM "PARAMETERS"
#define KEY_PARAM_NAME "name"
#define KEY_PARAM_DESCRIPTION "description"
#define KEY_PARAM_ADDR "address"
#define KEY_PARAM_DEFAULT "default"

Json_config::Json_config() {}

Firmware::Firmware(QJsonObject jObj) {
    firmware_array = jObj.value(KEY_FW).toArray();
    firmware_array_size = firmware_array.size();
    for (int i = 0; i < firmware_array_size; i++) {
        firmware_struct_array = add_struct(firmware_struct_array, i);
        QJsonObject firmware_obj = firmware_array[i].toObject();

        firmware_struct_array[i].firmware_id = firmware_obj.value(KEY_FW_ID).toInt();
        firmware_struct_array[i].firmware_version = firmware_obj.value(KEY_FW_VERSION).toString();
        firmware_struct_array[i].firmware_addr = firmware_obj.value(KEY_FW_ADDR).toString();
    }
}

Firmware::firmware_struct* Firmware::add_struct(Firmware::firmware_struct* parameter, const int number) {
    if (number == 0) {
        parameter = new Firmware::firmware_struct[number + 1];
    } else {
        Firmware::firmware_struct* temp_param = new Firmware::firmware_struct[number + 1];
        for (int i = 0; i < number; i++) {
            temp_param[i] = parameter[i];
        }
        delete [] parameter;
        parameter = temp_param;
    }
    return parameter;
}

Firmware::~Firmware() {
    delete [] firmware_struct_array;
}

QJsonObject Firmware::toJsonObject(QJsonObject jObj, Firmware::firmware_struct firmware) {
    jObj.insert(KEY_FW_ID, firmware.firmware_id);
    jObj.insert(KEY_FW_VERSION, firmware.firmware_version);
    jObj.insert(KEY_FW_ADDR, firmware.firmware_addr);
    return jObj;
}

Scenario::Scenario(QJsonObject jObj) {
    scenario_array = jObj.value(KEY_SCEN).toArray();
    scenario_array_size = scenario_array.size();
    for (int i = 0; i < scenario_array_size; i++) {
        scenario_struct_array = add_struct(scenario_struct_array, i);
        QJsonObject scenario_obj = scenario_array[i].toObject();

        scenario_struct_array[i].scenario_id  = scenario_obj.value(KEY_SCEN_ID).toInt();
        scenario_struct_array[i].scenario_name = scenario_obj.value(KEY_SCEN_NAME).toString();
        scenario_struct_array[i].min_firmware_version = scenario_obj.value(KEY_SCEN_MIN_FW).toString();\
        scenario_struct_array[i].scenario_states = scenario_obj.value(KEY_SCEN_STATES).toArray();
        scenario_struct_array[i].scenario_parameters = scenario_obj.value(KEY_SCEN_PARAMETERS).toArray();
    }
}

Scenario::scenario_struct* Scenario::add_struct(Scenario::scenario_struct* parameter, const int number) {
    if (number == 0) {
        parameter = new Scenario::scenario_struct[number + 1];
    } else {
        Scenario::scenario_struct* temp_param = new Scenario::scenario_struct[number + 1];
        for (int i = 0; i < number; i++) {
            temp_param[i] = parameter[i];
        }
        delete [] parameter;
        parameter = temp_param;
    }
    return parameter;
}

QJsonObject Scenario::toJsonObject(QJsonObject jObj, Scenario::scenario_struct scenario) {
    jObj.insert(KEY_SCEN_ID, scenario.scenario_id);
    jObj.insert(KEY_SCEN_NAME, scenario.scenario_name);
    jObj.insert(KEY_SCEN_MIN_FW, scenario.min_firmware_version);
    jObj.insert(KEY_SCEN_STATES, scenario.scenario_states);
    jObj.insert(KEY_SCEN_PARAMETERS, scenario.scenario_parameters);
    return jObj;
}

Scenario::~Scenario() {
    delete [] scenario_struct_array;
}

Parameters::Parameters(QJsonObject jObj) {
    parameters_array = jObj.value(KEY_PARAM).toArray();
    parameters_array_size = parameters_array.size();
    for (int i = 0; i < parameters_array_size; i++) {
        parameters_struct_array = add_struct(parameters_struct_array, i);
        QJsonObject parameter_obj = parameters_array[i].toObject();

        parameters_struct_array[i].parameter_name = parameter_obj.value(KEY_PARAM_NAME).toString();
        parameters_struct_array[i].parameter_description = parameter_obj.value(KEY_PARAM_DESCRIPTION).toString();
        parameters_struct_array[i].parameter_addr = parameter_obj.value(KEY_PARAM_ADDR).toString();
        parameters_struct_array[i].parameter_default_val = parameter_obj.value(KEY_PARAM_DEFAULT).toInt();
    }
}

Parameters::~Parameters() {
    delete [] parameters_struct_array;
}

Parameters::parameters_struct* Parameters::add_struct(Parameters::parameters_struct* parameter, const int number) {
    if (number == 0) {
        parameter = new Parameters::parameters_struct[number + 1];
    } else {
        Parameters::parameters_struct* temp_param = new Parameters::parameters_struct[number + 1];
        for (int i = 0; i < number; i++) {
            temp_param[i] = parameter[i];
        }
        delete [] parameter;
        parameter = temp_param;
    }
    return parameter;
}

QJsonObject Parameters::toJsonObject(QJsonObject jObj, Parameters::parameters_struct parameter) {
    jObj.insert(KEY_PARAM_NAME, parameter.parameter_name);
    jObj.insert(KEY_PARAM_DESCRIPTION, parameter.parameter_description);
    jObj.insert(KEY_PARAM_ADDR, parameter.parameter_addr);
    jObj.insert(KEY_PARAM_DEFAULT, parameter.parameter_default_val);
    return jObj;
}
