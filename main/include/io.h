#ifndef IO_H
#define IO_H

#include "event.h"
#include "pinconf.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


class Io
{
private:
    QueueLogicToIo& m_input;
    QueueIoToLogic& m_output;
public:
    Io(QueueLogicToIo& input, QueueIoToLogic& output);
    ~Io();
    //start io managment
    void operator()();

    void initADC();
    void initButtons();
    void initPosButton();
};

Io::Io(QueueLogicToIo& input, QueueIoToLogic& output):
m_input(input),
m_output(output)
{
    //install gpio isr service
    gpio_install_isr_service(0);

    initButtons();
    initPosButton();
    initADC();
}

Io::~Io()
{
}

void Io::operator()()
{
    std::cout << "posbutton, buttons, pottards : " << std::endl;

    while(1) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        int read_raw(0);
        bool buttonState(false);

        std::cout <<'\r' << "[[" << std::flush;
        //print posButtons
        for (size_t i = 0; i < 4; i++)
        {
            buttonState = gpio_get_level(posButtonPins[i]);
            if(buttonState)
            {
                std::cout << i + 1;
            }
        }

        std::cout <<"],[";
        //print buttons test
        while(!m_output.empty())
        {
            EventIo ev(m_output.front());
            m_output.pop();
            std::cout << "Button " << (char)(ev.id + 48) << "changed state to : " << ev.btnState << ',';
            std::cout <<"]]" << std::flush;
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

        std::cout <<"],[";
        //print pottards
        for (size_t i = 0; i < 5; i++)
        {
            if(pottardADCUnit[i] == ADC_UNIT_1)
            {
                read_raw = adc1_get_raw((adc1_channel_t)pottardChannels[i]);
            }
            else if(pottardADCUnit[i] == ADC_UNIT_2)
            {
                adc2_get_raw( (adc2_channel_t) pottardChannels[i], adc_width, &read_raw);
            }
            std::cout << std::setw(4) << read_raw << ',';
        }
        std::cout <<"]]" << std::flush;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void Io::initADC()
{
    //Configure ADC
    for (size_t i = 0; i < 5; i++)
    {
        if(pottardADCUnit[i] == ADC_UNIT_1)
        {
            adc1_config_width(adc_width);
            adc1_config_channel_atten((adc1_channel_t)pottardChannels[i], adc_atten);
        }
        else if (pottardADCUnit[i] == ADC_UNIT_2)
        {
            adc2_config_channel_atten((adc2_channel_t)pottardChannels[i], adc_atten);
        }
    }
}
static void IRAM_ATTR gpio_button_isr_handler(void* arg)
{
    std::tuple<uint8_t,QueueIoToLogic&>* dataPtr = 
        reinterpret_cast<std::tuple<uint8_t,QueueIoToLogic&>*>(arg);
    uint8_t id(std::get<0>(*dataPtr));
    QueueIoToLogic& queue(std::get<1>(*dataPtr));
    EventIo ev;
    ev.id = id;
    ev.btnState=gpio_get_level(buttonsPins[id]);
    queue.push(ev);
}

static void IRAM_ATTR gpio_posButton_isr_handler(void* arg)
{
    /*std::tuple<uint8_t,QueueIoToLogic&>* dataPtr = 
        reinterpret_cast<std::tuple<uint8_t,QueueIoToLogic&>*>(arg);
    uint8_t id(std::get<0>(*dataPtr));
    QueueIoToLogic& queue(std::get<1>(*dataPtr));
    EventIo ev;
    ev.id = id;
    ev.btnState=gpio_get_level(posButtonPins[id]);
    queue.push(ev);*/
}

void Io::initButtons()
{
    //init button interupt
    uint8_t id(0);
    for(const gpio_num_t& pin : buttonsPins)
    {
        gpio_config_t io_conf;
        //interrupt of rising edge
        io_conf.intr_type = GPIO_INTR_ANYEDGE;
        //bit mask of the pins
        io_conf.pin_bit_mask = (1ULL<<pin);
        //set as input mode
        io_conf.mode = GPIO_MODE_INPUT;
        //disable pull-down mode
        io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;//not needed i think
        //disable pull-up mode
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        //configure GPIO with the given settings
        gpio_config(&io_conf); 

        //create data for isr
        std::tuple<uint8_t,QueueIoToLogic&>* ptr = 
            new std::tuple<uint8_t,QueueIoToLogic&>(id, m_output);
        //hook isr handler for specific gpio pin
        gpio_isr_handler_add(pin, gpio_button_isr_handler
            , ptr);
        id++;
    }

}

void Io::initPosButton()
{
    //init with intr posButton
    for(const gpio_num_t& pin : posButtonPins)
    {
        gpio_config_t io_conf;
        //interrupt of rising edge
        io_conf.intr_type = GPIO_INTR_POSEDGE;
        //bit mask of the pins
        io_conf.pin_bit_mask = (1ULL<<pin);
        //set as input mode
        io_conf.mode = GPIO_MODE_INPUT;
        //disable pull-down mode
        io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;//not needed i think
        //disable pull-up mode
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        //configure GPIO with the given settings
        gpio_config(&io_conf); 

        //create data for isr
        std::tuple<uint8_t,QueueIoToLogic&>* ptr = 
            new std::tuple<uint8_t,QueueIoToLogic&>(5, m_output);
        //hook isr handler for specific gpio pin
        gpio_isr_handler_add(pin, gpio_posButton_isr_handler
            , ptr);
    }
}

#endif