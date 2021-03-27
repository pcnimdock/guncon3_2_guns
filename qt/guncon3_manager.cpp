#include "guncon3_manager.h"

GUNCON3_MANAGER::GUNCON3_MANAGER(QObject *parent) : QObject(parent)
{
    //LISTAR USB
    lista_usb = m_usb_info.devices();
    filtrar_usb_guncon3();
    if(lista_guncon3.size()>0)
    {
        //hay guncon3's conectados al inicio
        connect_guncon3_controllers();

    }
    connect(&controller1,SIGNAL(new_cmd()),this,SLOT(new_cmd_controller1()));
    connect(&controller2,SIGNAL(new_cmd()),this,SLOT(new_cmd_controller2()));

}

void GUNCON3_MANAGER::connect_guncon3_controllers()
{
    if(lista_guncon3.size()>0)
    {
        controller1.closeDevice();
        controller1.setBus(lista_guncon3.at(0).bus);
        controller1.setDeviceAddress(lista_guncon3.at(0).device_address);
        controller1.setupDevice();
        if (controller1.openDevice())
        {
            qInfo("Device1 open!");
            if(controller1_event.init_events_joy(1024,1024,0)<0)
            {
                qInfo("Error al abrir udev controlador 1");
            }
            controller1.init_send_read();

        }
        else
        {
            qWarning("Could not open device 1!");
        }
        if(lista_guncon3.size()>1)
        {
            controller2.closeDevice();
            controller2.setBus(lista_guncon3.at(1).bus);
            controller2.setDeviceAddress(lista_guncon3.at(1).device_address);
            controller2.setupDevice();
            if (controller2.openDevice())
            {
                qInfo("Device2 open!");
               if(controller2_event.init_events_joy(1024,1024,0)<0)
               {
               qInfo("Error al abrir udev del controlader 2");
               }
                controller2.init_send_read();


            }
            else
            {
                qWarning("Could not open device 2!");
            }

        }
    }
    else
    {
        qWarning("No devices");
    }

}

void GUNCON3_MANAGER::init_calibration(quint8 controller)
{

}

void GUNCON3_MANAGER::set_event(quint8 controller,qint16 x,qint16 y)
{
    if(controller==0)
    {
        controller1_event.set_axis(ABS_X,x);
        controller1_event.set_axis(ABS_Y,y);
        controller1_event.set_button(BTN_0,controller1.isButtonActive(BTN_A1));
        controller1_event.set_button(BTN_1,controller1.isButtonActive(BTN_A2));
        controller1_event.set_button(BTN_2,controller1.isButtonActive(BTN_B1));
        controller1_event.set_button(BTN_3,controller1.isButtonActive(BTN_B2));
        controller1_event.set_button(BTN_4,controller1.isButtonActive(BTN_C1));
        controller1_event.set_button(BTN_5,controller1.isButtonActive(BTN_C2));

        controller1_event.set_button(BTN_TRIGGER,controller1.isButtonActive(BTN_TRIGGER_G3));
        controller1_event.set_button(BTN_DPAD_UP,controller1.isButtonActive(BTN_DPAD_UP_G3));
        controller1_event.set_button(BTN_DPAD_DOWN,controller1.isButtonActive(BTN_DPAD_DOWN_G3));
        controller1_event.set_button(BTN_DPAD_LEFT,controller1.isButtonActive(BTN_DPAD_LEFT_G3));
        controller1_event.set_button(BTN_DPAD_RIGHT,controller1.isButtonActive(BTN_DPAD_RIGHT_G3));
        controller1_event.syn_report();
    }
    else
    {
        controller2_event.set_axis(ABS_X,x);
        controller2_event.set_axis(ABS_Y,y);
        controller2_event.set_button(BTN_0,controller2.isButtonActive(BTN_A1));
        controller2_event.set_button(BTN_1,controller2.isButtonActive(BTN_A2));
        controller2_event.set_button(BTN_2,controller2.isButtonActive(BTN_B1));
        controller2_event.set_button(BTN_3,controller2.isButtonActive(BTN_B2));
        controller2_event.set_button(BTN_4,controller2.isButtonActive(BTN_C1));
        controller2_event.set_button(BTN_5,controller2.isButtonActive(BTN_C2));

        controller2_event.set_button(BTN_TRIGGER,controller2.isButtonActive(BTN_TRIGGER_G3));
        controller2_event.set_button(BTN_DPAD_UP,controller2.isButtonActive(BTN_DPAD_UP_G3));
        controller2_event.set_button(BTN_DPAD_DOWN,controller2.isButtonActive(BTN_DPAD_DOWN_G3));
        controller2_event.set_button(BTN_DPAD_LEFT,controller2.isButtonActive(BTN_DPAD_LEFT_G3));
        controller2_event.set_button(BTN_DPAD_RIGHT,controller2.isButtonActive(BTN_DPAD_RIGHT_G3));
        controller2_event.syn_report();

    }

}

#define GUNCON2_BUTTON_TRIGGER			0x2000
#define GUNCON2_BUTTON_A				0x0008
#define GUNCON2_BUTTON_B				0x0004
#define GUNCON2_BUTTON_C				0x0002
#define GUNCON2_BUTTON_SELECT			0x4000
#define GUNCON2_BUTTON_START			0x8000
#define GUNCON2_DPAD_UP      			0x0010
#define GUNCON2_DPAD_DOWN				0x0040
#define GUNCON2_DPAD_LEFT 				0x0080
#define GUNCON2_DPAD_RIGHT 				0x0020

QByteArray GUNCON3_MANAGER::get_guncon2_transform_controller(quint8 controller,quint16 x,quint16 y)
{
    QByteArray arr;
    quint16 buttons=0;
    arr.append(controller);
    //qint16 x=0;
    //qint16 y=0;
    qint16 out=0;
    qint8 c2=0;
    if(controller==0)
    {
        buttons += GUNCON2_BUTTON_TRIGGER*controller1.isButtonActive(BTN_TRIGGER_G3);
        buttons += GUNCON2_BUTTON_A*controller1.isButtonActive(BTN_A1);
        buttons += GUNCON2_BUTTON_B*controller1.isButtonActive(BTN_A2);
        buttons += GUNCON2_BUTTON_C*controller1.isButtonActive(BTN_B1);
        buttons += GUNCON2_BUTTON_SELECT*controller1.isButtonActive(BTN_C1);
        buttons += GUNCON2_BUTTON_START*controller1.isButtonActive(BTN_C2);
        QByteArray b(controller1.getSticksValue());
        //STICK B
        if((quint8)(b.at(3))<200){buttons|=GUNCON2_DPAD_DOWN;}
        if((quint8)(b.at(3))>25){buttons|=GUNCON2_DPAD_UP;}
        if((quint8)(b.at(2))<200){buttons|=GUNCON2_DPAD_RIGHT;}
        if((quint8)(b.at(2))>25){buttons|=GUNCON2_DPAD_LEFT;}
        out = controller1.isButtonActive(ONE_LED_REFERENCE);
        c2 = controller1.isButtonActive(BTN_C2);
       // x=controller1.getAbsX();
       // y=controller1.getAbsY();
    }
    else if(controller==1)
    {
        buttons += GUNCON2_BUTTON_TRIGGER*controller2.isButtonActive(BTN_TRIGGER_G3);
        buttons += GUNCON2_BUTTON_A*controller2.isButtonActive(BTN_A1);
        buttons += GUNCON2_BUTTON_B*controller2.isButtonActive(BTN_A2);
        buttons += GUNCON2_BUTTON_C*controller2.isButtonActive(BTN_B1);
        buttons += GUNCON2_BUTTON_SELECT*controller2.isButtonActive(BTN_C1);
        buttons += GUNCON2_BUTTON_START*controller2.isButtonActive(BTN_C2);
        QByteArray b(controller2.getSticksValue());
        //STICK B
        if((quint8)(b.at(3))<200){buttons|=GUNCON2_DPAD_DOWN;}
        if((quint8)(b.at(3))>25){buttons|=GUNCON2_DPAD_UP;}
        if((quint8)(b.at(2))<200){buttons|=GUNCON2_DPAD_RIGHT;}
        if((quint8)(b.at(2))>25){buttons|=GUNCON2_DPAD_LEFT;}
        out = controller2.isButtonActive(ONE_LED_REFERENCE);
        c2 = controller2.isButtonActive(BTN_C2);
        //x=controller2.getAbsX();
        //y=controller2.getAbsY();
    }

    arr.append(buttons&0xFF);
    arr.append(buttons/256);
    arr.append(x&0xFF);
    arr.append(x/256);
    arr.append(y&0xFF);
    arr.append(y/256);
    quint8 zero=0;
    quint8 temp_var;
    if(out){temp_var=0xFE;}else{temp_var=0;}
    if(c2){temp_var|=0x01;}
    arr.append(temp_var);
    arr.append(zero);
    return (arr);

}

void GUNCON3_MANAGER::filtrar_usb_guncon3()
{
    int num=0;
    QList<quint8> dev_addr;
    lista_guncon3.clear();
    for(num=0;num<lista_usb.size();num++)
    {
        if(lista_usb.at(num).vid==0x0b9a)
        {
            if(lista_usb.at(num).pid==0x0800)
            {
                lista_guncon3.append(lista_usb.at(num));
                dev_addr.append(lista_usb.at(num).device_address);
            }
        }
    }
    quint8 temp=0;
    while(temp<dev_addr.size())
    {
        lista_guncon3[temp].device_address=dev_addr[temp];
        temp++;
    }
}

void GUNCON3_MANAGER::new_cmd_controller1()
{
    emit (controller1_cmd());
}
void GUNCON3_MANAGER::new_cmd_controller2()
{
    emit (controller2_cmd());
}

void GUNCON3_MANAGER::new_read()
{
    controller1.poll();
    timer.start();
}


