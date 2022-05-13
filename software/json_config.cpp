#include "json_config.h"

#include <QJsonValue>

#define KEY_FW "FIRMWARE"
#define KEY_FW_ID "id"
#define KEY_FW_VERSION "version"
#define KEY_FW_ADDR "address"

#define KEY_SCEN "SCENARIO"
#define KEY_SCEN_ID "id"
#define KEY_SCEN_MIN_FW "min_fw_version"
#define KEY_SCEN_STATES "states"
#define KEY_SCEN_PARAMETERS "scen_parameters"

#define KEY_PARAM "PARAMETERS"
#define KEY_REG "MEASUREMENTS"
#define KEY_NAME "name"
#define KEY_DESCRIPTION "description"
#define KEY_ADDR "address"
#define KEY_DEFAULT "default"

Json_config::Json_config() {}
Firmware::Firmware() {}
Scenario::Scenario() {}
Parameters::Parameters() {}
Register::Register() {}

void Firmware::firmware_init(QJsonObject jObj) {
    array = jObj.value(KEY_FW).toArray();
    array_size = array.size();
    for (int i = 0; i < array_size; i++) {
        struct_array = add_struct(struct_array, i);
        QJsonObject firmware_obj = array[i].toObject();

        struct_array[i].version = firmware_obj.value(KEY_FW_VERSION).toString();
        struct_array[i].address = firmware_obj.value(KEY_FW_ADDR).toString();
        QJsonArray default_val_js = firmware_obj.value(KEY_DEFAULT).toArray();
        for (int j = 0; j < default_val_js.size(); j++) {
            QJsonObject def_obj = default_val_js[j].toObject();
            struct_array[i].default_values[def_obj.value(KEY_NAME).toString()] = def_obj.value(KEY_DEFAULT).toString();
        }
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
    delete [] struct_array;
}

QJsonObject Firmware::toJsonObject(QJsonObject jObj, Firmware::firmware_struct firmware) {
    jObj.insert(KEY_FW_VERSION, firmware.version);
    jObj.insert(KEY_FW_ADDR, firmware.address);
    return jObj;
}

void Scenario::scenario_init(QJsonObject jObj) {
    array = jObj.value(KEY_SCEN).toArray();
    array_size = array.size();
    for (int i = 0; i < array_size; i++) {
        struct_array = add_struct(struct_array, i);
        QJsonObject scenario_obj = array[i].toObject();

        struct_array[i].name = scenario_obj.value(KEY_NAME).toString();
        struct_array[i].min_firmware_version = scenario_obj.value(KEY_SCEN_MIN_FW).toString();
        struct_array[i].states = scenario_obj.value(KEY_SCEN_STATES).toArray();
        struct_array[i].parameters = scenario_obj.value(KEY_SCEN_PARAMETERS).toArray();
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
    jObj.insert(KEY_NAME, scenario.name);
    jObj.insert(KEY_SCEN_MIN_FW, scenario.min_firmware_version);
    jObj.insert(KEY_SCEN_STATES, scenario.states);
    jObj.insert(KEY_SCEN_PARAMETERS, scenario.parameters);
    return jObj;
}

Scenario::~Scenario() {
    delete [] struct_array;
}

void Parameters::parameters_init(QJsonObject jObj) {
    array = jObj.value(KEY_PARAM).toArray();
    array_size = array.size();
    for (int i = 0; i < array_size; i++) {
        struct_array = add_struct(struct_array, i);
        QJsonObject parameter_obj = array[i].toObject();

        struct_array[i].name = parameter_obj.value(KEY_NAME).toString();
        struct_array[i].description = parameter_obj.value(KEY_DESCRIPTION).toString();
        struct_array[i].address = parameter_obj.value(KEY_ADDR).toString();
        struct_array[i].default_val = parameter_obj.value(KEY_DEFAULT).toString();
    }
}

Parameters::~Parameters() {
    delete [] struct_array;
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
    jObj.insert(KEY_NAME, parameter.name);
    jObj.insert(KEY_DESCRIPTION, parameter.description);
    jObj.insert(KEY_ADDR, parameter.address);
    jObj.insert(KEY_DEFAULT, parameter.default_val);
    return jObj;
}

void Register::register_init(QJsonObject jObj) {
    array = jObj.value(KEY_REG).toArray();
    array_size = array.size();
    for (int i = 0; i < array_size; i++) {
        struct_array = add_struct(struct_array, i);
        QJsonObject reg_obj = array[i].toObject();

        struct_array[i].name = reg_obj.value(KEY_NAME).toString();
        struct_array[i].description = reg_obj.value(KEY_DESCRIPTION).toString();
        struct_array[i].address = reg_obj.value(KEY_ADDR).toString();
    }
}

Register::~Register() {
    delete [] struct_array;
}

Register::register_struct* Register::add_struct(Register::register_struct* parameter, const int number) {
    if (number == 0) {
        parameter = new Register::register_struct[number + 1];
    } else {
        Register::register_struct* temp_param = new Register::register_struct[number + 1];
        for (int i = 0; i < number; i++) {
            temp_param[i] = parameter[i];
        }
        delete [] parameter;
        parameter = temp_param;
    }
    return parameter;
}

QJsonObject Register::toJsonObject(QJsonObject jObj, Register::register_struct reg) {
    jObj.insert(KEY_NAME, reg.name);
    jObj.insert(KEY_DESCRIPTION, reg.description);
    jObj.insert(KEY_ADDR, reg.address);
    return jObj;
}
