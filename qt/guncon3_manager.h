#ifndef GUNCON3_MANAGER_H
#define GUNCON3_MANAGER_H

#include <QObject>
#include "qusbdevice.h"
#include "qusbinfo.h"
#include <QList>
#include <QByteArray>
#include <QTimer>

#include "guncon3.h"
#include "calibration.h"
#include "events.h"

class GUNCON3_MANAGER : public QObject
{
    Q_OBJECT
public:
    explicit GUNCON3_MANAGER(QObject *parent = nullptr);
    void connect_guncon3_controllers();
    void init_calibration(quint8 controller);
    void set_event(quint8 controller,qint16 x,qint16 y);
    QByteArray get_guncon2_transform_controller(quint8 controller,quint16 x,quint16 y);
    GUNCON3 controller1;
    Calibration cal_controller1;
    Events controller1_event;
    GUNCON3 controller2;
    Calibration cal_controller2;
    Events controller2_event;

signals:
    void controller1_cmd();
    void controller2_cmd();

private slots:
    void new_cmd_controller1();
    void new_cmd_controller2();
    void new_read();

private:
    QUsbInfo m_usb_info;
    QTimer timer;


    QUsbDevice::IdList lista_usb;
    QUsbDevice::IdList lista_guncon3;

    void filtrar_usb_guncon3();


};

#endif // GUNCON3_MANAGER_H
