#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QMap>

class Json_config {
private:

public:
    static QString IP;
    static int PORT;

    Json_config();

    Json_config(QJsonObject jObj);

    static QString getIP();
    static int getPORT();

    void firmware_init(QJsonObject jObj);
    struct firmware_struct {
        QString version;
        QString sysid;
        QString clock;
    };
    QMap<int, firmware_struct> firmware_map;
    QJsonObject toJsonObject(QJsonObject jObj,  QMap<int, firmware_struct> firmware);


    void scenario_init(QJsonObject jObj);
    struct scenario_struct {
        QString name;
        QString min_firmware_version;
        QJsonArray states;
        QJsonArray parameters;
    };
    QMap<int, scenario_struct> scenario_map;
    QJsonObject toJsonObject(QJsonObject jObj,  QMap<int, scenario_struct> scenario);


    void parameters_init(QJsonObject jObj);
    struct parameters_struct {
        QString name;
        QString description;
        QString address;
        QString default_val;
    };
    QMap<int, parameters_struct> parameters_map;
    QJsonObject toJsonObject(QJsonObject jObj, QMap<int, parameters_struct> parameter);


    void measurment_init(QJsonObject jObj);
    struct measurment_struct {
        QString name;
        QString description;
        QString interval;
        QString address;
    };
    QMap<int, measurment_struct> measurment_map;
    QJsonObject toJsonObject(QJsonObject jObj, QMap<int, measurment_struct> measurement);
};

#endif // JSON_CONFIG_H
