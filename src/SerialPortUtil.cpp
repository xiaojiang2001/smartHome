#include "SerialPortUtil.h"
#include <QDebug>
QSerialPort* SerialPortUtil::serialPort = nullptr;
QByteArray SerialPortUtil::dataToSend = {};

void SerialPortUtil::initSerialPort(const QString &portName, int baudRate) {
    if (serialPort == nullptr) {
        serialPort = new QSerialPort();
    }

    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    SerialPortUtil::dataToSend.append(static_cast<char>(0xFD));
    SerialPortUtil::dataToSend.append(static_cast<char>(0x02));
    SerialPortUtil::dataToSend.append(static_cast<char>(0xE2));
    SerialPortUtil::dataToSend.append(static_cast<char>(0x8D));
    SerialPortUtil::dataToSend.append(static_cast<char>(0x61));
    SerialPortUtil::dataToSend.append(static_cast<char>(0x45));
    SerialPortUtil::dataToSend.append(static_cast<char>(0xDF));

    if (!serialPort->open(QIODevice::ReadWrite)) {
        // 错误处理：打开串口失败
        // 例如，可以抛出异常或打印错误信息
    }
}

void SerialPortUtil::lightCon()
{
    static int a = 0;
    a++;
    qDebug() << SerialPortUtil::dataToSend << "send" << a;
    serialPort->write(SerialPortUtil::dataToSend);
}
