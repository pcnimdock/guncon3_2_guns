#ifndef GUNCON3_H
#define GUNCON3_H

#include <QObject>
#include "qusbdevice.h"
#include "qusbendpoint.h"

#define BTN_TRIGGER_G3 1
#define BTN_A1      2
#define BTN_A2      4
#define BTN_B1      8
#define BTN_B2      16
#define BTN_C1      32
#define BTN_C2      64
#define BTN_A_STICK 128
#define BTN_B_STICK 256
#define OUT_OF_RANGE 512
#define ONE_LED_REFERENCE 1024

#define BTN_DPAD_UP_G3      2048
#define BTN_DPAD_DOWN_G3    4096
#define BTN_DPAD_LEFT_G3    8192
#define BTN_DPAD_RIGHT_G3   16384

//quint8 abs_hat0y,abs_hat0x,rx,ry;
#define ABS_HAT0Y


const quint8 USB_ENDPOINT_IN = 0x82; /* Bulk output endpoint for responses */
const quint8 USB_ENDPOINT_OUT = 0x02; /* Bulk input endpoint for commands */
const quint16 USB_TIMEOUT_MSEC = 8;


class GUNCON3 : public QObject
{
Q_OBJECT
public:
    GUNCON3(QObject *parent = nullptr);
    ~GUNCON3(void);
    void setBus(qint8 b);
    void setupDevice();
    bool openDevice(void);
    void closeDevice(void);
    bool openHandle(void);
    void closeHandle(void);
    void read(QByteArray *buf);
    void write(QByteArray *buf);
    QByteArray decode(QByteArray data2);
    bool isButtonActive(quint16 button);
    QByteArray getSticksValue();
    qint16 getAbsX();
    qint16 getAbsY();
    void init_send_read();
    void poll();

signals:
    void new_cmd();

public slots:
    void onReadyRead(void);
    void onWriteComplete(qint64 bytes);

private:
    QUsbDevice *m_usb_dev;
    QUsbEndpoint *m_read_ep, *m_write_ep;

    QByteArray m_send, m_recv;

    QUsbDevice::Id m_filter;
    QUsbDevice::Config m_config;
    qint8 bus;
    quint16 buttons;
    qint16 abs_x,abs_y,abs_z;
    quint8 abs_stickA_y,abs_stickA_x,abs_stickB_x,abs_stickB_y;

};

#endif // GUNCON3_H
