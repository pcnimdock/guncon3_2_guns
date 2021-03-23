#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQueue>
//#include <QtSerialPort/QSerialPort>

#include "guncon3_manager.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    GUNCON3_MANAGER gcon_m;
    void pantalla_calibracion();
    void restablecer_pantalla();
    void incrementar_modo_res_juego();
    void mandar_cadena_a_stm32(quint8 controller);
    void mandar_evento(quint8 controller);

private:
    quint16 h_screen=256,w_screen=640;

    //variables calibracion
    double KX1, KX2, KX3, KY1, KY2, KY3;        // coefficients for calibration algorithm
    Calibration cal1,cal2;
    int pixmap_points[9][2];
    int ref_points[9][2];
    quint8 num_samples;
    bool trigger_act;
    bool calibracion_controller1;
    bool calibracion_controller2;
    QQueue<QByteArray> cola;
    //QSerialPort serial;
    quint16 res_jug_x=640;
    quint16 res_jug_y=240;
    quint8 modo_res_juego=0;
    quint16 from_stm32_offsetx=0;
    quint16 from_stm32_offsety=0;
    quint8 cola_vacia_serial=1;
    //qint32 height_screen_save,width_screen_save;

public slots:
     void new_read_controller1();
     void new_read_controller2();
     //void cadena_recibida_stm32(); //cadena recibida del serialport
     //void cadena_enviada_stm32();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
