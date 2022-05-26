#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QMap>

class Json_config {
public:
    Json_config();
};

class Firmware {
public:
    Firmware();
    virtual ~Firmware();

    void firmware_init(QJsonObject jObj);

    QJsonArray array;
    int array_size = 0;

    struct firmware_struct {
        QString version;
        QString sysid;
        QMap<QString, QString> default_values;
    };
    firmware_struct* struct_array = 0;
    firmware_struct* add_struct(firmware_struct* parameter, const int number);

    QJsonObject toJsonObject(QJsonObject jObj, firmware_struct firmware);

private:

};

class Scenario {
public:
    Scenario();
    virtual ~Scenario();

    void scenario_init(QJsonObject jObj);

    QJsonArray array;
    int array_size = 0;

    struct scenario_struct {
        QString name;
        QString min_firmware_version;
        QJsonArray states;
        QJsonArray parameters;
    };
    scenario_struct* struct_array = 0;
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

    QJsonArray array;
    int array_size = 0;

    struct parameters_struct {
        QString name;
        QString description;
        QString address;
        QString default_val;
    };
    parameters_struct* struct_array = 0;
    parameters_struct* add_struct(parameters_struct* parameter, const int number);

    QJsonObject toJsonObject(QJsonObject jObj, parameters_struct parameter);
};

class Register {
private:

public:
    Register();
    virtual ~Register();

    void register_init(QJsonObject jObj);

    QJsonArray array;
    int array_size = 0;

    struct register_struct {
        QString name;
        QString description;
        QString address;
    };
    register_struct* struct_array = 0;
    register_struct* add_struct(register_struct* reg, const int number);

    QJsonObject toJsonObject(QJsonObject jObj, register_struct reg);
};

#endif // JSON_CONFIG_H
