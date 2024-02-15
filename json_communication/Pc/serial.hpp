#pragma once

#include <iostream>
#include <string>

#include "include/serial/SerialPort.hpp"

class Serial
{
private:
    SerialPort* _serial;
    char* _port;
    unsigned int _BAUD;
public:
    inline Serial();
    inline Serial(const char*, unsigned int);
    inline ~Serial();

    void inline set(const char*, unsigned int);
    void inline setPort(const char* buffer);
    void inline setBAUD(unsigned int buf_size);
    void inline setPort(std::istream&);
    void inline setBAUD(std::istream&);


    int inline readSerialPort(const char* buffer, unsigned int buf_size);
    bool inline writeSerialPort(const char* buffer, unsigned int buf_size);
    bool inline isConnected();
    void inline closeSerial();

    std::string inline readLine();  //Inspiré de : https://github.com/fedetft/serial-port/blob/master/1_simple/SimpleSerial.h

};

Serial::Serial()
{
    _serial = nullptr;
    _port = nullptr;
    _BAUD = 0;
}

Serial::Serial(const char* portName, unsigned int BAUD)
{
    set(portName, BAUD);
}

Serial::~Serial()
{
    delete _serial, _port;
    _serial = nullptr;
    _port = nullptr;
    _BAUD = 0;
}

void Serial::set(const char* portName, unsigned int BAUD)
{
    *_port = *(portName);
    _BAUD = BAUD;
    _serial = new SerialPort(_port, _BAUD);
}

void Serial::setPort(std::istream& stream)
{
    std::string inputCom;
    stream >> inputCom;
    setPort(inputCom.c_str());
}

void Serial::setPort(const char* portName)
{
    delete _serial, _port;
    _port = strstr((char*)portName, portName);
    std::cout << "PORT : " << _port << std::endl;
    _serial = new SerialPort(_port, _BAUD);
}

void Serial::setBAUD(std::istream& stream)
{
    unsigned int inputCom;
    stream >> inputCom;
    setBAUD(inputCom);
}

void Serial::setBAUD(unsigned int BAUD)
{
    _BAUD = BAUD;
    _serial = new SerialPort(_port, _BAUD);
}

int Serial::readSerialPort(const char* buffer, unsigned int buf_size)
{
    if (_serial == nullptr) return 0;
    return _serial->readSerialPort(buffer, buf_size);
}

bool Serial::writeSerialPort(const char* buffer, unsigned int buf_size)
{
    if (_serial == nullptr) return false;
    bool resp = _serial->writeSerialPort(buffer, buf_size);
    return resp;
}

bool Serial::isConnected()
{
    if (_serial == nullptr) return false;
    return _serial->isConnected();
}

void Serial::closeSerial()
{
    if (_serial == nullptr) return;
    _serial->closeSerial();
}


std::string Serial::readLine()
{
    char c;
    std::string result;
    for (;;)
    {
        if (result.length() > 500) return result;
        _serial->readSerialPort(&c, 1);
        switch (c)
        {
        case '\r':
            break;
        case '\0':
            break;
        case '\n':
            return result;
        default:
            result += c;
        }
    }
}