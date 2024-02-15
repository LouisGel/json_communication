/********
 * Fichier: communication.hpp
 * Auteurs: Louis-Xavier Gélinas
 * Date: 10 février 2023 (creation)
 * Description: Permet d'établire une communication Json
 *              entre un ordinateur et l'Arduino.
********/

#ifndef COMMUNICATION
#define COMMUNICATION

#include <Arduino.h>
#include <ArduinoJson.h>

template <size_t LEN>
class Commuication
{
private:
    HardwareSerial* _serial;
    StaticJsonDocument<LEN> _message;
    void (*_onMessageFunc)(StaticJsonDocument<LEN>&);

public:
    Commuication();
    Commuication(HardwareSerial*, void(StaticJsonDocument<LEN>& doc));
    ~Commuication();

    void sendMessage(StaticJsonDocument<LEN>&);
    void onMessageFunc(void(StaticJsonDocument<LEN>&));
    void setSerial(HardwareSerial*);
    void update();

private:
    void onMessage();

};

template <size_t LEN>
Commuication<LEN>::Commuication()
{
    _serial = &Serial;
    _onMessageFunc = nullptr;
}

template <size_t LEN>
Commuication<LEN>::Commuication(HardwareSerial* serial, void onMessageFunc(StaticJsonDocument<LEN>& doc))
{
    _serial = serial;
    _onMessageFunc = onMessageFunc;
}

template <size_t LEN>
Commuication<LEN>::~Commuication()
{
    _message.clear();
}

template <size_t LEN>
void Commuication<LEN>::sendMessage(StaticJsonDocument<LEN>& doc)
{
    serializeJson(doc, (*_serial));
    (*_serial).println();
}

template <size_t LEN>
void Commuication<LEN>::onMessage()
{
    deserializeJson(_message, Serial);
    if(_onMessageFunc != nullptr) (*_onMessageFunc)(_message);
}

template <size_t LEN>
void Commuication<LEN>::onMessageFunc(void onMessageFunc(StaticJsonDocument<LEN>&))
{
    _onMessageFunc = onMessageFunc;
}

template <size_t LEN>
void Commuication<LEN>::setSerial(HardwareSerial* serial)
{
    _serial = serial;
}

template <size_t LEN>
void Commuication<LEN>::update()
{
    if(!_serial->available()) return;
    onMessage();
}

#endif