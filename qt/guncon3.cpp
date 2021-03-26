#include "guncon3.h"
#include <QDebug>

const unsigned char key[8]={0x01,0x12,0x6f,0x32,0x24,0x60,0x17,0x21};

static const unsigned char KEY_TABLE[320] = {
    0x75, 0xC3, 0x10, 0x31, 0xB5, 0xD3, 0x69, 0x84, 0x89, 0xBA, 0xD6, 0x89, 0xBD, 0x70, 0x19, 0x8E, 0x58, 0xA8,
    0x3D, 0x9B, 0x5D, 0xF0, 0x49, 0xE8, 0xAD, 0x9D, 0x7A, 0xD , 0x7E, 0x24, 0xDA, 0xFC, 0xD , 0x14, 0xC5, 0x23,
    0x91, 0x11, 0xF5, 0xC0, 0x4B, 0xCD, 0x44, 0x1C, 0xC5, 0x21, 0xDF, 0x61, 0x54, 0xED, 0xA2, 0x81, 0xB7, 0xE5,
    0x74, 0x94, 0xB0, 0x47, 0xEE, 0xF1, 0xA5, 0xBB, 0x21, 0xC8, 0x91, 0xFD, 0x4C, 0x8B, 0x20, 0xC1, 0x7C, 9, 0x58,
    0x14, 0xF6, 0   , 0x52, 0x55, 0xBF, 0x41, 0x75, 0xC0, 0x13, 0x30, 0xB5, 0xD0, 0x69, 0x85, 0x89, 0xBB, 0xD6, 0x88,
    0xBC, 0x73, 0x18, 0x8D, 0x58, 0xAB, 0x3D, 0x98, 0x5C, 0xF2, 0x48, 0xE9, 0xAC, 0x9F, 0x7A, 0xC, 0x7C, 0x25, 0xD8,
    0xFF, 0xDC, 0x7D, 8, 0xDB, 0xBC, 0x18, 0x8C, 0x1D, 0xD6, 0x3C, 0x35, 0xE1, 0x2C, 0x14, 0x8E, 0x64, 0x83, 0x39,
    0xB0, 0xE4, 0x4E, 0xF7, 0x51, 0x7B, 0xA8, 0x13, 0xAC, 0xE9, 0x43, 0xC0, 8, 0x25, 0xE, 0x15, 0xC4, 0x20, 0x93,
    0x13, 0xF5, 0xC3, 0x48, 0xCC, 0x47, 0x1C, 0xC5, 0x20, 0xDE, 0x60, 0x55, 0xEE, 0xA0, 0x40, 0xB4, 0xE7, 0x74,
    0x95, 0xB0, 0x46, 0xEC, 0xF0, 0xA5, 0xB8, 0x23, 0xC8, 4, 6, 0xFC, 0x28, 0xCB, 0xF8, 0x17, 0x2C, 0x25, 0x1C,
    0xCB, 0x18, 0xE3, 0x6C, 0x80, 0x85, 0xDD, 0x7E, 9, 0xD9, 0xBC, 0x19, 0x8F, 0x1D, 0xD4, 0x3D, 0x37, 0xE1, 0x2F,
    0x15, 0x8D, 0x64, 6, 4, 0xFD, 0x29, 0xCF, 0xFA, 0x14, 0x2E, 0x25, 0x1F, 0xC9, 0x18, 0xE3, 0x6D, 0x81, 0x84,
    0x80, 0x3B, 0xB1, 0xE5, 0x4D, 0xF7, 0x51, 0x78, 0xA9, 0x13, 0xAD, 0xE9, 0x80, 0xC1, 0xB, 0x25, 0x93, 0xFC,
    0x4D, 0x89, 0x23, 0xC2, 0x7C, 0xB, 0x59, 0x15, 0xF6, 1, 0x50, 0x55, 0xBF, 0x81, 0x75, 0xC3, 0x10, 0x31, 0xB5,
    0xD3, 0x69, 0x84, 0x89, 0xBA, 0xD6, 0x89, 0xBD, 0x70, 0x19, 0x8E, 0x58, 0xA8, 0x3D, 0x9B, 0x5D, 0xF0, 0x49,
    0xE8, 0xAD, 0x9D, 0x7A, 0xD, 0x7E, 0x24, 0xDA, 0xFC, 0xD, 0x14, 0xC5, 0x23, 0x91, 0x11, 0xF5, 0xC0, 0x4B, 0xCD,
    0x44, 0x1C, 0xC5, 0x21, 0xDF, 0x61, 0x54, 0xED, 0xA2, 0x81, 0xB7, 0xE5, 0x74, 0x94, 0xB0, 0x47, 0xEE, 0xF1,
    0xA5, 0xBB, 0x21, 0xC8
};

GUNCON3::GUNCON3(QObject *parent) : QObject(parent), m_usb_dev(new QUsbDevice()), m_read_ep(Q_NULLPTR), m_write_ep(Q_NULLPTR)
{
 //   this->setupDevice();

    m_send.append(static_cast<char>(key[0]));
    m_send.append(static_cast<char>(key[1]));
    m_send.append(static_cast<char>(key[2]));
    m_send.append(static_cast<char>(key[3]));
    m_send.append(static_cast<char>(key[4]));
    m_send.append(static_cast<char>(key[5]));
    m_send.append(static_cast<char>(key[6]));
    m_send.append(static_cast<char>(key[7]));


  //  if (this->openDevice()) {
  //      qInfo("Device open!");
  //      this->write(&m_send);
        //this->read(&m_recv);
  //  } else {
  //      qWarning("Could not open device!");
  //  }
}

GUNCON3::~GUNCON3()
{
    qInfo("Closing...");
}

void GUNCON3::setBus(qint8 b)
{
    bus=b;
}

void GUNCON3::setDeviceAddress(quint8 da)
{
    device_address=da;
}

void GUNCON3::setupDevice()
{
    /* There are 2 ways of identifying devices depending on the platform.
   * You can use both methods, only one will be taken into account.
   */

    //para configuracion de los guncon3 introducir directamente el filtro
    qDebug("setupDevice");

    m_usb_dev->setLogLevel(QUsbDevice::logDebug);
    m_usb_dev->setTimeout(USB_TIMEOUT_MSEC);

    m_filter.pid = 0x0800;
    m_filter.vid = 0x0b9a;
    m_filter.bus = bus;
    m_filter.device_address = device_address;

    //
    m_config.alternate = 0;
    m_config.config = 1;
    m_config.interface = 0;





    //
    m_usb_dev->setId(m_filter);
    m_usb_dev->setConfig(m_config);
}

bool GUNCON3::openDevice()
{
    qDebug("Opening");

    if (m_usb_dev->open(device_address) == QUsbDevice::statusOK) {
        // Device is open
        return this->openHandle();
    }
    return false;
}

void GUNCON3::closeDevice()
{
    qDebug("Closing");

    if (m_usb_dev->isConnected()) {
        this->closeHandle();
        m_usb_dev->close();
    }
}

bool GUNCON3::openHandle()
{
    qDebug("Opening Handle");
    bool a = false, b = false;
    m_read_ep = new QUsbEndpoint(m_usb_dev, QUsbEndpoint::bulkEndpoint, USB_ENDPOINT_IN);
    m_write_ep = new QUsbEndpoint(m_usb_dev, QUsbEndpoint::bulkEndpoint, USB_ENDPOINT_OUT);

    connect(m_read_ep, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_write_ep, SIGNAL(bytesWritten(qint64)), this, SLOT(onWriteComplete(qint64)));

    a = m_read_ep->open(QIODevice::ReadOnly);
    if (a) {
        m_read_ep->setPolling(true);

    }

    b = m_write_ep->open(QIODevice::WriteOnly);

    return a && b;
}

void GUNCON3::closeHandle()
{
    qDebug("Closing Handle");
    if (m_read_ep != Q_NULLPTR) {
        m_read_ep->close();
        m_read_ep->disconnect();
        qInfo() << m_read_ep->errorString();
        delete m_read_ep;
        m_read_ep = Q_NULLPTR;
    }

    if (m_write_ep != Q_NULLPTR) {
        m_write_ep->close();
        m_write_ep->disconnect();
        qInfo() << m_write_ep->errorString();
        delete m_write_ep;
        m_write_ep = Q_NULLPTR;
    }
}

void GUNCON3::read(QByteArray *buf)
{
    quint16 btn_tmp;
    QByteArray b(m_read_ep->readAll());
    char data[15];
    //quint64 size;
    //size = m_read_ep->readAll();

    //qDebug() << "Reading" << data << 15;
    if(b.size()>14)
    {

    QByteArray dec_data(this->decode(b));
    if(dec_data.size()>12)
    {
    //------------
    abs_stickA_y=dec_data.at(0);//ABS_RY
    qDebug() << abs_stickA_y;
    abs_stickA_x=dec_data.at(1);//ABS_RX
    qDebug() << abs_stickA_x;
    abs_stickB_y=(dec_data.at(2)); //ABS_HAT0Y
    abs_stickB_x=(dec_data.at(3)); //ABS_HAT0X
    abs_z=(dec_data.at(4)*256+dec_data.at(5)); //Z
    abs_y=(dec_data.at(6)*256+dec_data.at(7)); //ABS_Y
    abs_x=(dec_data.at(8)*256+dec_data.at(9)); //ABS_X
    btn_tmp =0;
    btn_tmp +=BTN_TRIGGER_G3*((dec_data.at(11) & 0x20)?1:0); //BTN_TRIGGER
    btn_tmp +=BTN_A1*((dec_data.at(12) & 0x04)?1:0);  //A1
    btn_tmp +=BTN_A2*((dec_data.at(12) & 0x02)?1:0);  //A2
    btn_tmp +=BTN_B1*((dec_data.at(11) & 0x04)?1:0);  //B1
    btn_tmp +=BTN_B2*((dec_data.at(11) & 0x02)?1:0);  //B2
    btn_tmp +=BTN_C1*((dec_data.at(11) & 0x80)?1:0);  //C1
    btn_tmp +=BTN_C2*((dec_data.at(12) & 0x08)?1:0);  //C2
    btn_tmp +=BTN_A_STICK*((dec_data.at(10) & 0x80)?1:0);  //A STICK BUTTON
    btn_tmp +=BTN_B_STICK*((dec_data.at(10) & 0x40)?1:0);  //B STICK BUTTON
    btn_tmp +=OUT_OF_RANGE*((dec_data.at(11) & 0x10)?1:0);  // out of range
    btn_tmp +=ONE_LED_REFERENCE*((dec_data.at(11) & 0x08)?1:0);  // only one led reference ¿?

    //HAT to Button
    if(abs_stickB_y<100){btn_tmp+=BTN_DPAD_UP_G3;} //ABS_HAT0Y
    if(abs_stickB_y>160){btn_tmp+=BTN_DPAD_DOWN_G3;} //ABS_HAT0Y
    if(abs_stickB_x<100){btn_tmp+=BTN_DPAD_LEFT_G3;} //ABS_HAT0X
    if(abs_stickB_x>160){btn_tmp+=BTN_DPAD_RIGHT_G3;} //ABS_HAT0X
    buttons=btn_tmp;

    //------------
    buf->append(dec_data);
    }
    }

}

void GUNCON3::write(QByteArray *buf)
{

    qDebug() << "Writing" << *buf << buf->size();
    if (m_write_ep->write(buf->constData(), buf->size()) < 0) {
        qWarning("write failed");
    }

}

QByteArray GUNCON3::decode(QByteArray data2)
{

    QByteArray ret;
    ret.clear();
    qDebug()<<"Decode";
    int x, y, key_index;
    int bkey, keyr, byte;
    int a_sum,b_sum;
    int key_offset;
    unsigned char data[15];
    if(data2.size()!=15)
    {return ret;}
    for(int i=0;i<15;i++)
    {
        data[i]=data2[i];
    }

    b_sum = data[13] ^ data[12];
    b_sum = b_sum + data[11] + data[10] - data[9] - data[8];
    b_sum = b_sum ^ data[7];
    b_sum = b_sum & 0xFF;
    a_sum = data[6] ^ b_sum;
    a_sum = a_sum - data[5] - data[4];
    a_sum = a_sum ^ data[3];
    a_sum = a_sum + data[2] + data[1] - data[0];
    a_sum = a_sum & 0xFF;

    if (a_sum != key[7]) {
        if (1)
            qDebug() << "checksum mismatch: ";
        return NULL;
    }

    //key_offset = (((((key[1] ^ key[2]) - key[3] - key[4]) ^ key[5]) + key[6] - key[7]) ^ data[14]) + (unsigned char)0x26;
    key_offset = key[1] ^ key[2];
    key_offset = key_offset - key[3] - key[4];
    key_offset = key_offset ^ key[5];
    key_offset = key_offset + key[6] - key[7];
    key_offset = key_offset ^ data[14];
    key_offset = key_offset + 0x26;
    key_offset = key_offset & 0xFF;

    key_index = 4;

    //byte E is part of the key offset
    // byte D is ignored, possibly a padding byte - make the checksum workout
    for (x = 12; x >= 0; x--) {
        byte = data[x];
        for (y = 4; y > 1; y--) { // loop 3 times
            key_offset--;

            bkey = KEY_TABLE[key_offset + 0x41];
            keyr = key[key_index];
            if (--key_index == 0)
                key_index = 7;

            if ((bkey & 3) == 0)
                byte =(byte - bkey) - keyr;
            else if ((bkey & 3) == 1)
                byte = ((byte + bkey) + keyr);
            else
                byte = ((byte ^ bkey) ^ keyr);
        }
        ret.append(byte);
    }
    return ret;
}

bool GUNCON3::isButtonActive(quint16 button)
{
    return((buttons&button)?true:false); //activo negado
}

QByteArray GUNCON3::getSticksValue()
{
    QByteArray values;
    values.append(abs_stickA_x);
    values.append(abs_stickA_y);
    values.append(abs_stickB_x);
    values.append(abs_stickB_y);
    return (values);
}

qint16 GUNCON3::getAbsX()
{
    return (abs_x);
}

qint16 GUNCON3::getAbsY()
{
    return (abs_y);
}

void GUNCON3::init_send_read()
{
    this->write(&m_send);
    this->read(&m_recv);
}

void GUNCON3::poll()
{
    if(m_read_ep->poll())
    {
        this->read(&m_recv);
    }

}

void GUNCON3::onReadyRead()
{
    qDebug("onReadyRead");
    qDebug() << "Bytes available" << this->m_read_ep->bytesAvailable();
    this->read(&m_recv);
    emit(new_cmd());
    //this->write(&m_send);
}

void GUNCON3::onWriteComplete(qint64 bytes)
{
    qDebug() << "onWriteComplete" << bytes << "bytes";
}
