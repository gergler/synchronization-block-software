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

    void firmware_init(QJsonObject jObj);

    struct firmware_struct {
        QString version;
        QString sysid;
        QMap<QString, QString> default_values;
    };
    QMap<int, firmware_struct> firmware_map;

    QJsonObject toJsonObject(QJsonObject jObj,  QMap<int, firmware_struct> firmware);

private:

};

class Scenario {
public:
    Scenario();

    void scenario_init(QJsonObject jObj);

    struct scenario_struct {
        QString name;
        QString min_firmware_version;
        QJsonArray states;
        QJsonArray parameters;
    };
    QMap<int, scenario_struct> scenario_map;

    QJsonObject toJsonObject(QJsonObject jObj,  QMap<int, scenario_struct> scenario);

private:

};

class Parameters {
private:

public:
    Parameters();

    void parameters_init(QJsonObject jObj);

    struct parameters_struct {
        QString name;
        QString description;
        QString address;
        QString default_val;
    };
    QMap<int, parameters_struct> parameters_map;

    QJsonObject toJsonObject(QJsonObject jObj, QMap<int, parameters_struct> parameter);
};

class Measurement {
private:

public:
    Measurement();

    void measurment_init(QJsonObject jObj);

    struct measurment_struct {
        QString name;
        QString description;
        QString address;
    };
    QMap<int, measurment_struct> measurment_map;

    QJsonObject toJsonObject(QJsonObject jObj, QMap<int, measurment_struct> measurement);
};

#endif // JSON_CONFIG_H
