#pragma once

#include <RaspiLib.h>
#include "IPin.h"
#include "Utils.h"
#include "iostream"
#include <algorithm>
#include <chrono>
#include <thread>

using namespace PiLib;

namespace SpiderLib
{

  class Ultrasonic
  {
    private: 
        IPin *echo_pin;
        IPin *trigger_pin;

        uint64_t millis()
        {
            uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        
            return ms;
        }

        uint64_t micros()
        {
            uint64_t us = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        
            return us;
        }
    public:
    Ultrasonic(){}

    Ultrasonic(RaspiPinLabel triggerPin, RaspiPinLabel echoPin)
    {
        // get pins & set reference
        IPin &pin = RaspiLib::GetInstance().GetPin(triggerPin);
        if (pin.GetGPIONumber() != GPIO_DUMMY)
            trigger_pin = &pin;

        IPin &pin2 = RaspiLib::GetInstance().GetPin(echoPin);
        if (pin2.GetGPIONumber() != GPIO_DUMMY)
            echo_pin = &pin2;

        trigger_pin->SetPinMode(FSEL_OUTP);
        echo_pin->SetPinMode(FSEL_INPT);
    }
    
    void send_trigger_pulse()
    {
        trigger_pin->WriteBool(false);
        std::this_thread::sleep_for(std::chrono::microseconds(5));
        trigger_pin->WriteBool(true);
        std::this_thread::sleep_for(std::chrono::microseconds(5));
        trigger_pin->WriteBool(false);
    }

    void waitForEcho(bool state, int timeout)
    {
        while (echo_pin->ReadBool() != state && timeout --);
    }

    double getDistance()
    {
       /* uint64_t sizeArr = 0;
        uint64_t distance_cm[] = {0,0,0};
        uint64_t timeRet;
        for(int i = 0; i < 3; i++)
        {
            send_trigger_pulse();
            timeRet = pulse_in();
            distance_cm[i] = timeRet * 340/2/10000;
        }
        //array sortieren

        sizeArr = sizeof(distance_cm) / sizeof(uint64_t);
        std::sort(distance_cm,distance_cm + sizeArr);
        return int(distance_cm[1]); */

        send_trigger_pulse();
        waitForEcho(true,100000);
        uint64_t start = micros();

        waitForEcho(false,100000);
        uint64_t end = micros();

        double dist = (end - start) / 1000000. * 34000 / 2;


        return dist;
    }

  };


}