#include "events.h"
#include <QDebug>
#include <QMessageBox>

#include <iostream>

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

Events::Events()
{

}

Events::~Events()
{
    libevdev_uinput_destroy(uidev);
}

char Events::init_events_mouse(int width, int height)
{
    struct input_absinfo info;
    info.minimum=0;
    info.maximum=width;
    info.resolution=1;
    info.flat=0;
    info.fuzz=0;
    dev = libevdev_new();
    libevdev_set_name(dev,"Guncon3Mouse");
    libevdev_enable_event_code(dev,EV_ABS,ABS_X,&info);
    info.minimum=0;
    info.maximum=height;
    libevdev_enable_event_code(dev,EV_ABS,ABS_Y,&info);
    libevdev_enable_event_code(dev,EV_KEY,BTN_TOUCH,NULL);
    libevdev_enable_property(dev,INPUT_PROP_DIRECT);
    //BTN_{MOUSE,LEFT,MIDDLE,RIGHT}
    //con lo anterior ya se debe de detectar como un touchscreen
    int rc=libevdev_uinput_create_from_device(dev,LIBEVDEV_UINPUT_OPEN_MANAGED,&uidev);
    libevdev_free(dev);
    if(rc<0)
    {

        qDebug() << "Error al abrir uinput, put sudo setfacl -m ur:usuario:rw /dev/uinput";
        std::cout << "C Style Critical Error Message, put sudo setfacl -m ur:usuario:rw /dev/uinput";
        return(-1);
    }
    return(0);



}

char Events::init_events_joy(int max,int min,bool cb_4_3)
{
    struct input_absinfo info;
    if(cb_4_3)
    {
        info.minimum=(max-(4*min)/3)/2;
        info.maximum=max-(max-(4*min)/3)/2;
    }
    else
    {
    info.minimum=0;
    info.maximum=max;
    }
    info.resolution=1;
    info.flat=0;
    info.fuzz=0;
    dev = libevdev_new();
    libevdev_set_name(dev,"Gcon3Vjoy");
    libevdev_set_id_vendor(dev,0x1234);
    libevdev_set_id_product(dev,0xbeae);
    libevdev_set_id_version(dev,0);
    libevdev_set_id_bustype(dev,3);
    info.resolution=1;
    info.flat=0;
    info.fuzz=0;
    info.minimum=-127;
    info.maximum=127;
    info.minimum=-256;
    info.maximum=256;

    libevdev_enable_event_code(dev,EV_ABS,ABS_X,&info);
    libevdev_enable_event_code(dev,EV_ABS,ABS_Y,&info);

    info.minimum=-127;
    info.maximum=127;
    libevdev_enable_event_code(dev,EV_ABS,ABS_RX,&info);
    libevdev_enable_event_code(dev,EV_ABS,ABS_RY,&info);
    libevdev_enable_event_code(dev,EV_ABS,ABS_HAT0X,&info);
    libevdev_enable_event_code(dev,EV_ABS,ABS_HAT0Y,&info);

    libevdev_enable_event_code(dev,EV_KEY,BTN_TRIGGER,NULL);
    libevdev_enable_event_code(dev,EV_KEY,BTN_0,NULL);
    libevdev_enable_event_code(dev,EV_KEY,BTN_1,NULL);
    libevdev_enable_event_code(dev,EV_KEY,BTN_2,NULL);
    libevdev_enable_event_code(dev,EV_KEY,BTN_3,NULL);
    libevdev_enable_event_code(dev,EV_KEY,BTN_4,NULL);
    libevdev_enable_event_code(dev,EV_KEY,BTN_5,NULL);
    //libevdev_enable_event_code(dev,EV_KEY,BTN_6,NULL);
    //libevdev_enable_event_code(dev,EV_KEY,BTN_7,NULL);
    //libevdev_enable_event_code(dev,EV_KEY,BTN_8,NULL);
    libevdev_enable_event_code(dev,EV_KEY,BTN_DPAD_UP,NULL);
    libevdev_enable_event_code(dev,EV_KEY,BTN_DPAD_DOWN,NULL);
    libevdev_enable_event_code(dev,EV_KEY,BTN_DPAD_LEFT,NULL);
    libevdev_enable_event_code(dev,EV_KEY,BTN_DPAD_RIGHT,NULL);
    int rc=libevdev_uinput_create_from_device(dev,LIBEVDEV_UINPUT_OPEN_MANAGED,&uidev);
    libevdev_free(dev);
    if(rc<0)
    {
        qDebug() << "Error al abrir uinput, put sudo setfacl -m u:$USER:rw /dev/uinput";
        qCritical( "C Style Critical Error Message, put sudo setfacl -m u:$USER:rw /dev/uinput" );

        return(-1);
    }
    return(0);

}

int Events::set_axis(unsigned int num, short int value)
{
    return (libevdev_uinput_write_event(uidev,EV_ABS,num,((int)value)));

}

int Events::set_button(unsigned int num, int value)
{
    return(libevdev_uinput_write_event(uidev,EV_KEY,num,value));
}

int Events::syn_report()
{
    return(libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0));
}
