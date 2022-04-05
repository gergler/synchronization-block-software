#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>

class Json_config {
public:
    Json_config();
};

class Firmware {
public:
    Firmware();
    virtual ~Firmware();

    void firmware_init(QJsonObject jObj);

    QJsonArray firmware_array;
    int firmware_array_size = 0;

    struct firmware_struct {
        int firmware_id;
        QString firmware_version;
        QString firmware_addr;
    };
    firmware_struct* firmware_struct_array = 0;
    firmware_struct* add_struct(firmware_struct* parameter, const int number);

    QJsonObject toJsonObject(QJsonObject jObj, firmware_struct firmware);

private:

};

class Scenario {
public:
    Scenario();
    virtual ~Scenario();

    void scenario_init(QJsonObject jObj);

    QJsonArray scenario_array;
    int scenario_array_size = 0;

    struct scenario_struct {
        int scenario_id;
        QString scenario_name;
        QString min_firmware_version;
        QJsonArray scenario_states;
        QJsonArray scenario_parameters;
    };
    scenario_struct* scenario_struct_array = 0;
    scenario_struct* add_struct(scenario_struct* parameter, const int number);

    QJsonObject toJsonObject(QJsonObject jObj, scenario_struct firmware);

private:

};

class Parameters {
private:

public:
    Parameters();
    virtual ~Parameters();

    void parameters_init(QJsonObject jObj);

    QJsonArray parameters_array;
    int parameters_array_size = 0;

    struct parameters_struct {
        QString parameter_name;
        QString parameter_description;
        QString parameter_addr;
        int parameter_default_val;
    };
    parameters_struct* parameters_struct_array = 0;
    parameters_struct* add_struct(parameters_struct* parameter, const int number);

    QJsonObject toJsonObject(QJsonObject jObj, parameters_struct parameter);
};

class Register {
private:

public:
    Register();
    virtual ~Register();

    void register_init(QJsonObject jObj);

    QJsonArray register_array;
    int register_array_size = 0;

    struct register_struct {
        QString register_name;
        QString register_description;
        QString register_addr;
        int register_default_val;
    };
    register_struct* register_struct_array = 0;
    register_struct* add_struct(register_struct* reg, const int number);

    QJsonObject toJsonObject(QJsonObject jObj, register_struct reg);
};

#endif // JSON_CONFIG_H
