#ifndef PINCONF_H
#define PINCONF_H

#include "driver/gpio.h"
#include "driver/adc.h"

constexpr gpio_num_t buttonsPins[] {
    GPIO_NUM_23,
    GPIO_NUM_21,
    GPIO_NUM_17,
    GPIO_NUM_22,
    GPIO_NUM_16
};

constexpr gpio_num_t pottardPins[] {
    GPIO_NUM_27,
    GPIO_NUM_25,
    GPIO_NUM_26,
    GPIO_NUM_35,
    GPIO_NUM_33
};

constexpr adc_channel_t pottardChannels[] {
    ADC_CHANNEL_7,
    ADC_CHANNEL_8,
    ADC_CHANNEL_9,
    ADC_CHANNEL_7,
    ADC_CHANNEL_5
};

constexpr adc_unit_t pottardADCUnit[] {
    ADC_UNIT_2,
    ADC_UNIT_2,
    ADC_UNIT_2,
    ADC_UNIT_1,
    ADC_UNIT_1
};

constexpr gpio_num_t posButtonPins[] {
    GPIO_NUM_5,
    GPIO_NUM_4,
    GPIO_NUM_32,
    GPIO_NUM_2 //this gpio is linked to an internal-LED

};

constexpr adc_atten_t atten = ADC_ATTEN_DB_11;
//précision adc
constexpr adc_bits_width_t width = ADC_WIDTH_BIT_9;


#endif