#include "json_config.h"

#include <QJsonValue>

#define KEY_FW "FIRMWARE"
#define KEY_FW_VERSION "version"
#define KEY_FW_SYSID "sysid"

#define KEY_SCEN "SCENARIO"
#define KEY_SCEN_MIN_FW "min_fw_version"
#define KEY_SCEN_STATES "states"
#define KEY_SCEN_PARAMETERS "scen_parameters"

#define KEY_PARAM "PARAMETERS"
#define KEY_MEAS "MEASUREMENTS"
#define KEY_NAME "name"
#define KEY_DESCRIPTION "description"
#define KEY_ADDR "address"
#define KEY_DEFAULT "default"

Json_config::Json_config() {}
Firmware::Firmware() {}
Scenario::Scenario() {}
Parameters::Parameters() {}
Measurement::Measurement() {}

void Firmware::firmware_init(QJsonObject jObj) {
    QJsonArray array = jObj.value(KEY_FW).toArray();
    for (int i = 0; i < array.size(); i++) {
        QJsonObject firmware_obj = array[i].toObject();
        firmware_map[i].version = firmware_obj.value(KEY_FW_VERSION).toString();
        firmware_map[i].sysid = firmware_obj.value(KEY_FW_SYSID).toString();

        QJsonArray default_val_js = firmware_obj.value(KEY_DEFAULT).toArray();
        for (int j = 0; j < default_val_js.size(); j++) {
            QJsonObject def_obj = default_val_js[j].toObject();
             firmware_map[i].default_values[def_obj.value(KEY_NAME).toString()] = def_obj.value(KEY_DEFAULT).toString();
        }
    }
}

QJsonObject Firmware::toJsonObject(QJsonObject jObj, QMap<int, firmware_struct> firmware) {
    for (int i = 0; i < firmware.size(); i++) {
        QJsonObject obj;
        obj.insert(KEY_FW_VERSION, firmware[i].version);
        obj.insert(KEY_FW_SYSID, firmware[i].sysid);
        for (int j = 0; j < firmware_map[i].default_values.size(); j++) {
            QJsonObject def_obj;
//            def_obj.insert(KEY_NAME, firmware_map[j].default_values.value());
            obj.insert(KEY_DEFAULT, def_obj);
        }
        jObj.insert(KEY_FW, obj);
    }
    return jObj;
}

void Scenario::scenario_init(QJsonObject jObj) {
    QJsonArray array = jObj.value(KEY_SCEN).toArray();
    for (int i = 0; i < array.size(); i++) {
        QJsonObject scenario_obj = array[i].toObject();

        scenario_map[i].name = scenario_obj.value(KEY_NAME).toString();
        scenario_map[i].min_firmware_version = scenario_obj.value(KEY_SCEN_MIN_FW).toString();
        scenario_map[i].states = scenario_obj.value(KEY_SCEN_STATES).toArray();
        scenario_map[i].parameters = scenario_obj.value(KEY_SCEN_PARAMETERS).toArray();
    }
}

QJsonObject Scenario::toJsonObject(QJsonObject jObj, QMap<int, scenario_struct> scenario) {
    for (int i = 0; i < scenario.size(); i++) {
        QJsonObject obj;
        obj.insert(KEY_NAME, scenario[i].name);
        obj.insert(KEY_SCEN_MIN_FW, scenario[i].min_firmware_version);
        obj.insert(KEY_SCEN_STATES, scenario[i].states);
        obj.insert(KEY_SCEN_PARAMETERS, scenario[i].parameters);
        jObj.insert(KEY_SCEN, obj);

    }
    return jObj;
}

void Parameters::parameters_init(QJsonObject jObj) {
    QJsonArray array = jObj.value(KEY_PARAM).toArray();
    for (int i = 0; i < array.size(); i++) {
        QJsonObject parameter_obj = array[i].toObject();

        parameters_map[i].name = parameter_obj.value(KEY_NAME).toString();
        parameters_map[i].description = parameter_obj.value(KEY_DESCRIPTION).toString();
        parameters_map[i].address = parameter_obj.value(KEY_ADDR).toString();
        parameters_map[i].default_val = parameter_obj.value(KEY_DEFAULT).toString();
    }
}

QJsonObject Parameters::toJsonObject(QJsonObject jObj, QMap<int, parameters_struct> parameter) {
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

void Measurement::measurment_init(QJsonObject jObj) {
    QJsonArray array = jObj.value(KEY_MEAS).toArray();
    for (int i = 0; i < array.size(); i++) {
        QJsonObject obj = array[i].toObject();

        measurment_map[i].name = obj.value(KEY_NAME).toString();
        measurment_map[i].description = obj.value(KEY_DESCRIPTION).toString();
        measurment_map[i].address = obj.value(KEY_ADDR).toString();
    }
}

QJsonObject Measurement::toJsonObject(QJsonObject jObj, QMap<int, measurment_struct> measurment) {
    for (int i = 0; i < measurment.size(); i++) {
        QJsonObject obj;
        obj.insert(KEY_NAME, measurment[i].name);
        obj.insert(KEY_DESCRIPTION, measurment[i].description);
        obj.insert(KEY_ADDR, measurment[i].address);
        jObj.insert(KEY_MEAS, obj);
    }
    return jObj;
}
