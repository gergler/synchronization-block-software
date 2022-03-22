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
    QJsonObject toJsonObject();
private:
    int _firmware_id;
    QString _firmware_version;
    QString _firmware_addr;
};

class Scenario {
public:
    Scenario(QJsonObject jObj);
    QJsonObject toJsonObject();
private:
    int _scenario_id;
    QString _scenario_name;
    QString _min_firmware_version;
    QJsonArray _scenario_states;
    QJsonArray _scenario_parameters;
};

class Parameters {
public:
    Parameters(QJsonObject jObj);
    QJsonObject toJsonObject();

    void set(QString parameter_name, QString parameter_description, QString parameter_addr, int parameter_default_val);
    void get();

    QString parameter_name;
    QString parameter_description;
    QString parameter_addr;
    int parameter_default_val;

private:
    QString _parameter_name;
    QString _parameter_description;
    QString _parameter_addr;
    int _parameter_default_val;
};

#endif // JSON_CONFIG_H
