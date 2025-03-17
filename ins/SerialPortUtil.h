#ifndef SERIALPORTUTIL_H
#define SERIALPORTUTIL_H

#include <QSerialPort>

class SerialPortUtil {
public:
    static QSerialPort* serialPort;
    static QByteArray dataToSend;
    static void initSerialPort(const QString &portName, int baudRate = QSerialPort::Baud9600);
    static void lightCon();

};

#endif // SERIALPORTUTIL_H


