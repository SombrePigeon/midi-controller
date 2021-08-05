#include <stdio.h>
#include <iostream>
#include <iomanip>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pinconf.h"


extern "C" void app_main(void)
{
    //Configure ADC
    for (size_t i = 0; i < 5; i++)
    {
        if(pottardADCUnit[i] == ADC_UNIT_1)
        {
            adc1_config_width(width);
            adc1_config_channel_atten((adc1_channel_t)pottardChannels[i], atten);
        }
        else if (pottardADCUnit[i] == ADC_UNIT_2)
        {
            adc2_config_channel_atten((adc2_channel_t)pottardChannels[i], atten);
        }
    }

    //init buttons
    for(const gpio_num_t& pin : buttonsPins)
    {
        gpio_config_t io_conf;
        //disable interrupt
        io_conf.intr_type = GPIO_INTR_DISABLE;
        //set as input mode
        io_conf.mode = GPIO_MODE_INPUT;
        //bit mask of the pins that you want to set,e.g.GPIO18/19
        io_conf.pin_bit_mask = (1ULL<<pin);
        //disable pull-down mode
        io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
        //disable pull-up mode
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        //configure GPIO with the given settings
        gpio_config(&io_conf);
    }

    //init posButton
    for(const gpio_num_t& pin : posButtonPins)
    {
        gpio_config_t io_conf;
        //disable interrupt
        io_conf.intr_type = GPIO_INTR_DISABLE;
        //set as input mode
        io_conf.mode = GPIO_MODE_INPUT;
        //bit mask of the pins that you want to set
        io_conf.pin_bit_mask = (1ULL<<pin);
        //disable pull-down mode
        io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
        //disable pull-up mode
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        //configure GPIO with the given settings
        gpio_config(&io_conf);
    }

    std::cout << "posbutton, buttons, pottards : " << std::endl;

    while(1) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        int read_raw(0);
        bool buttonState(false);

        std::cout <<'\r' << "[[" << std::flush ;
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
        //print buttons
        for (size_t i = 0; i < 5; i++)
        {
            std::cout << gpio_get_level(buttonsPins[i]) << ',';
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
                adc2_get_raw( (adc2_channel_t) pottardChannels[i], width, &read_raw);
            }
            std::cout << std::setw(4) << read_raw << ',';
        }
        std::cout <<"]]" << std::flush;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
