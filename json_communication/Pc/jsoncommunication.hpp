#pragma once

#include <iostream>
#include <string>

#include "include/json.hpp"
#include "serial.hpp"

using json = nlohmann::json;

template <size_t LEN>
class JsonCommuication
{
private:
    Serial* _serial;
    json _j_message;
    void (*_onMessageFunc)(json&);

    bool _incoming;

public:
    JsonCommuication();
    JsonCommuication(Serial*, void(json&));
    ~JsonCommuication();

    bool sendMessage(json&);
    void onMessageFunc(void(json&));
    void setSerial(Serial*);
    bool connected() const;
    void update();

    bool incoming() const;
    json getMessage() const;

private:
    void onMessage();
};

template <size_t LEN>
JsonCommuication<LEN>::JsonCommuication()
{
    _onMessageFunc = nullptr;
    _serial = nullptr;
}

template <size_t LEN>
JsonCommuication<LEN>::JsonCommuication(Serial* serial, void onMessageFunc(json& doc))
{
    _serial = serial;
    _onMessageFunc = onMessage;
}

template <size_t LEN>
JsonCommuication<LEN>::~JsonCommuication()
{
    delete _serial, _onMessageFunc;
    _serial->closeSerial();
    _onMessageFunc = nullptr;
    _serial = nullptr;
    _j_message.clear();
}

template <size_t LEN>
bool JsonCommuication<LEN>::sendMessage(json& doc)
{
    if (!connected()) return 0;
    std::string message = doc.dump();
    return _serial->writeSerialPort(message.c_str(), message.length());
}

template <size_t LEN>
void JsonCommuication<LEN>::onMessageFunc(void onMessageFunc(json& doc))
{
    _onMessageFunc = onMessageFunc;
}

template <size_t LEN>
void JsonCommuication<LEN>::setSerial(Serial* serial)
{
    _serial = serial;
}

template <size_t LEN>
bool JsonCommuication<LEN>::connected() const
{
    return _serial->isConnected();
}

template <size_t LEN>
void JsonCommuication<LEN>::onMessage()
{
    if (_onMessageFunc != nullptr) (*_onMessageFunc)(_j_message);
}

template <size_t LEN>
void JsonCommuication<LEN>::update()
{
    if (_serial == nullptr) return;
    if (!connected()) return;
    _j_message.clear();
    std::string msg = _serial->readLine();
    try { _j_message = json::parse(msg); }
    catch (...) {}
    if (!_j_message.empty())
    {
        _incoming = true;
        onMessage();
    }
    else
    {
        _incoming = false;
    }
}

template <size_t LEN>
bool JsonCommuication<LEN>::incoming() const
{
    return _incoming;
}

template <size_t LEN>
json JsonCommuication<LEN>::getMessage() const
{
    return _j_message;
}
