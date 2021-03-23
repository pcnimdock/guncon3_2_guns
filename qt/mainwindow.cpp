#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "guncon3.h"

#include <QScreen>
#include "events.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&gcon_m,SIGNAL(controller1_cmd()),this,SLOT(new_read_controller1()));
    connect(&gcon_m,SIGNAL(controller2_cmd()),this,SLOT(new_read_controller2()));
    ui->pixmap_mirilla->hide();
    calibracion_controller1=0;
    calibracion_controller2=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pantalla_calibracion()
{

    ui->controller1_ABS->hide();
    ui->controller1_stickA->hide();
    ui->controller1_stickB->hide();
    ui->controller2_ABS->hide();
    ui->controller2_stickA->hide();
    ui->controller2_stickB->hide();
    //obtener tamaño de la pantalla
    QList <QScreen *> pantalla = QApplication::screens();
    QSize *resolucion = new QSize();
    *resolucion = pantalla.at(0)->size();
    qint32 height_screen,width_screen;

    height_screen= resolucion->height();
    width_screen = resolucion->width();
    delete resolucion;
    int height_pix,width_pix;
    h_screen=height_screen;
    w_screen=width_screen;
    height_pix=height_screen/5;
    width_pix=height_pix;
    //el tamaño de la imagen de la mirilla será un quinto de la resolucion vertical
    ui->pixmap_mirilla->resize(width_pix,height_pix);

    //calcular los puntos
    ref_points[0][0]=height_pix/2;              ref_points[0][1]=height_pix/2;
    ref_points[1][0]=width_screen/2;            ref_points[1][1]=height_pix/2;
    ref_points[2][0]=width_screen-height_pix/2; ref_points[2][1]=height_pix/2;

    ref_points[3][0]=height_pix/2;              ref_points[3][1]=height_screen/2;
    ref_points[4][0]=width_screen/2;            ref_points[4][1]=height_screen/2;
    ref_points[5][0]=width_screen-height_pix/2; ref_points[5][1]=height_screen/2;

    ref_points[6][0]=height_pix/2;              ref_points[6][1]=height_screen-height_pix/2;
    ref_points[7][0]=width_screen/2;            ref_points[7][1]=height_screen-height_pix/2;
    ref_points[8][0]=width_screen-height_pix/2; ref_points[8][1]=height_screen-height_pix/2;

    pixmap_points[0][0]=0;                          pixmap_points[0][1]=0;
    pixmap_points[1][0]=width_screen/2-height_pix/2;pixmap_points[1][1]=0;
    pixmap_points[2][0]=width_screen-height_pix;    pixmap_points[2][1]=0;

    pixmap_points[3][0]=0;                          pixmap_points[3][1]=height_screen/2-height_pix/2;
    pixmap_points[4][0]=width_screen/2-height_pix/2;pixmap_points[4][1]=height_screen/2-height_pix/2;
    pixmap_points[5][0]=width_screen-height_pix;    pixmap_points[5][1]=height_screen/2-height_pix/2;

    pixmap_points[6][0]=0;                          pixmap_points[6][1]=height_screen-height_pix;
    pixmap_points[7][0]=width_screen/2-height_pix/2;pixmap_points[7][1]=height_screen-height_pix;
    pixmap_points[8][0]=width_screen-height_pix;    pixmap_points[8][1]=height_screen-height_pix;
    //almacenar posiciones
    for(int i=0;i<9;i++)
    {cal1.append_reference_point(ref_points[i][0],ref_points[i][1]);} //primer punto, arriba izq pixmap en 0,0

    for(int i=0;i<9;i++)
    {cal2.append_reference_point(ref_points[i][0],ref_points[i][1]);} //primer punto, arriba izq pixmap en 0,0

    ui->pixmap_mirilla->move(pixmap_points[0][0],pixmap_points[0][1]);
    ui->pixmap_mirilla->show();
    num_samples=0;
    trigger_act=0;
    this->showFullScreen();
}

void MainWindow::restablecer_pantalla()
{
    ui->pixmap_mirilla->hide();
    ui->controller1_ABS->show();
    ui->controller1_stickA->show();
    ui->controller1_stickB->show();
    ui->controller2_ABS->show();
    ui->controller2_stickA->show();
    ui->controller2_stickB->show();
    this->showNormal();

}

void MainWindow::incrementar_modo_res_juego()
{
    switch(modo_res_juego)
    {
    case 0: modo_res_juego=1; res_jug_x=640;res_jug_y=256;
        ui->lbl_res->setText("640x256");
        break;
        case 1:
        modo_res_juego=2; res_jug_x=768;res_jug_y=768;
        ui->lbl_res->setText("768x768");
        break;
        case 2:
        modo_res_juego=0; res_jug_x=640;res_jug_y=240;
        ui->lbl_res->setText("640x240");
        break;
    default:
        modo_res_juego=0; res_jug_x=640;res_jug_y=240;
        ui->lbl_res->setText("640x240");
        break;
    }

}


void MainWindow::mandar_evento(quint8 controller)
{
    //n_controller botonesH botonesL xL xH yL yH C2
    QByteArray b;
    QByteArray c;
    quint16 x,y;
    b.clear();
    switch(controller)
    {
    case 0: //controlador 1
        x=gcon_m.controller1.getAbsX();
        y=gcon_m.controller1.getAbsY();
        //transformar según resolución del juego
        x=((x*1.0)*(res_jug_x*1.0)/(w_screen*1.0));
        y=((y*1.0)*(res_jug_y*1.0)/(h_screen*1.0));
        gcon_m.set_event(0,x,y);
        //gcon_m.controller1_event.set_axis(ABS_X,x);
        //gcon_m.controller1_event.set_axis(ABS_Y,y);
        //gcon_m.controller1_event.set_button(BTN_0,)
        //b.append(gcon_m.get_guncon2_transform_controller(0,x,y));

        break;
    case 1: //controlador 2
        x=gcon_m.controller2.getAbsX();
        y=gcon_m.controller2.getAbsY();
        //transformar según resolución del juego
        x=((x*1.0)*(res_jug_x*1.0)/(w_screen*1.0));
        y=((y*1.0)*(res_jug_y*1.0)/(h_screen*1.0));
        gcon_m.set_event(1,x,y);
        //b.append(gcon_m.get_guncon2_transform_controller(1,x,y));

        break;
    }
}



void MainWindow::new_read_controller1()
{
    //comprobar si el boton STICK A y STICK B están apretados
    //si es así, abrir pantalla de calibración
    static bool trigger_ant=1;
    static bool cambiar_res_juego=0;

    static quint8 wait=125;
    if(calibracion_controller1)
    {
        if(gcon_m.controller1.isButtonActive(BTN_TRIGGER_G3))
        {
            //guardar posicion
            //incrementar num_samples
            if(trigger_ant==1)
            {
                trigger_ant=0;
                num_samples++;
                cal1.append_sample_point(gcon_m.controller1.getAbsX(),gcon_m.controller1.getAbsY());
                //mover posición del pixmap

                if(num_samples<9)
                {
                    ui->pixmap_mirilla->move(pixmap_points[num_samples][0],pixmap_points[num_samples][1]);
                }
                else
                {
                    //guardar calibracion y reestablecer pantalla
                    calibracion_controller1=0;
                    restablecer_pantalla();
                }
            }
        }
        else
        {if(trigger_ant==0){trigger_ant=1;}}

    }
    else
    {
        short x_abs=gcon_m.controller1.getAbsX();
        short y_abs=gcon_m.controller1.getAbsY();
        ui->lbl_absx->setText(QString::number(x_abs));
        ui->lbl_absy->setText(QString::number(y_abs));

        cal1.Do_Calibration(&x_abs,&y_abs);

        ui->lbl_calx->setText(QString::number(x_abs));
        ui->lbl_caly->setText(QString::number(y_abs));
        ui->controller1_ABS->setX(x_abs*1.0/w_screen-0.5);
        ui->controller1_ABS->setY(y_abs*1.0/h_screen-0.5);
        QByteArray b(gcon_m.controller1.getSticksValue());
        float stax,stay,stbx,stby;
        stax = ((quint8)b.at(0));
        stax /=127.0;
        stax -=128/127;
        if(stax>1){stax=1;}
        stay = ((quint8)b.at(1));
        stay /=127.0;
        stay -=128/127;
        if(stay>1){stay=1;}

        stbx = ((quint8)b.at(2));
        stbx /=127.0;
        stbx -=128/127;
        if(stbx>1){stbx=1;}
        stby = ((quint8)b.at(3));
        stby /=127.0;
        stby -=128/127;
        if(stby>1){stby=1;}
        ui->controller1_stickA->setX(stax);
        ui->controller1_stickA->setY(-1*stay);
        ui->controller1_stickB->setX(stbx);
        ui->controller1_stickB->setY(-1*stby);
        //mandar_cadena_a_stm32(0);

        //gcon_m.controller1_event.set_axis(ABS_X,stax);
        //gcon_m.controller1_event.set_axis(ABS_Y,stay);

        gcon_m.set_event(0,(x_abs*1.0*512)/(w_screen)-256,(y_abs*1.0*512)/(h_screen)-256);
        if(wait>0)
        {wait--;}
        if(gcon_m.controller1.isButtonActive(BTN_A_STICK)&&gcon_m.controller1.isButtonActive(BTN_B_STICK)&&wait==0)
        {
            cal1.reset();
            pantalla_calibracion();
            calibracion_controller1=1;
        }
        if(cambiar_res_juego==0)
        {
            if(gcon_m.controller1.isButtonActive(BTN_B_STICK)&&gcon_m.controller1.isButtonActive(BTN_B2)&&wait==0)
            {
                //cambiar la resolucion del juego
                incrementar_modo_res_juego();
                cambiar_res_juego=1;
            }
        }
        else
        {
            if(!(gcon_m.controller1.isButtonActive(BTN_B_STICK)&&gcon_m.controller1.isButtonActive(BTN_B2)&&wait==0))
            {
                cambiar_res_juego=0;
            }
        }

    }


}

void MainWindow::new_read_controller2()
{
    static bool trigger_ant=1;
    static quint8 wait=125;
    static bool cambiar_res_juego=1;
    if(calibracion_controller2)
    {

        if(gcon_m.controller2.isButtonActive(BTN_TRIGGER_G3))
        {
            //guardar posicion
            //incrementar num_samples
            if(trigger_ant==1)
            {
                trigger_ant=0;
                num_samples++;
                cal2.append_sample_point(gcon_m.controller2.getAbsX(),gcon_m.controller2.getAbsY());
                //mover posición del pixmap

                if(num_samples<9)
                {
                    ui->pixmap_mirilla->move(pixmap_points[num_samples][0],pixmap_points[num_samples][1]);
                }
                else
                {
                    //guardar calibracion y reestablecer pantalla
                    calibracion_controller2=0;
                    restablecer_pantalla();
                }
            }
        }
        else
        {if(trigger_ant==0){trigger_ant=1;}}

    }
    else
    {
        short x_abs=gcon_m.controller2.getAbsX();
        short y_abs=gcon_m.controller2.getAbsY();

        cal2.Do_Calibration(&x_abs,&y_abs);

        ui->controller2_ABS->setX(x_abs*1.0/w_screen-0.5);
        ui->controller2_ABS->setY(y_abs*1.0/h_screen-0.5);
        QByteArray b(gcon_m.controller2.getSticksValue());
        float stax,stay,stbx,stby;
        stax = ((quint8)b.at(0));
        stax /=127.0;
        stax -=128/127;
        if(stax>1){stax=1;}
        stay = ((quint8)b.at(1));
        stay /=127.0;
        stay -=128/127;
        if(stay>1){stay=1;}

        stbx = ((quint8)b.at(2));
        stbx /=127.0;
        stbx -=128/127;
        if(stbx>1){stbx=1;}
        stby = ((quint8)b.at(3));
        stby /=127.0;
        stby -=128/127;
        if(stby>1){stby=1;}
        ui->controller2_stickA->setX(stax);
        ui->controller2_stickA->setY(-1*stay);
        ui->controller2_stickB->setX(stbx);
        ui->controller2_stickB->setY(-1*stby);
        //mandar_cadena_a_stm32(1);
        gcon_m.set_event(1,x_abs,y_abs);
        if(wait>0)
        {wait--;}
        if(gcon_m.controller2.isButtonActive(BTN_A_STICK)&&gcon_m.controller2.isButtonActive(BTN_B_STICK)&&wait==0)
        {
            cal2.reset();
            pantalla_calibracion();
            calibracion_controller2=1;
        }
        if(cambiar_res_juego==0)
        {
            if(gcon_m.controller1.isButtonActive(BTN_B_STICK)&&gcon_m.controller1.isButtonActive(BTN_B2)&&wait==0)
            {
                //cambiar la resolucion del juego
                incrementar_modo_res_juego();
                cambiar_res_juego=1;
            }
        }
        else
        {
            if(!(gcon_m.controller1.isButtonActive(BTN_B_STICK)&&gcon_m.controller1.isButtonActive(BTN_B2)&&wait==0))
            {
                cambiar_res_juego=0;
            }
        }
    }
}



