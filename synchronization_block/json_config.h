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
    Firmware(QJsonObject jObj);
    QJsonObject toJsonObject(QJsonObject jObj);
    QJsonArray _firmware_array;
private:
    int _firmware_id;
    QString _firmware_version;
    QString _firmware_addr;
};

class Scenario {
public:
    Scenario(QJsonObject jObj);
    QJsonObject toJsonObject(QJsonObject jObj);
private:
    int _scenario_id;
    QString _scenario_name;
    QString _min_firmware_version;
    QJsonArray _scenario_states;
    QJsonArray _scenario_parameters;
};

class Parameters {
private:

public:
    Parameters(QJsonObject jObj);
    virtual ~Parameters();

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

#endif // JSON_CONFIG_H
