#include <stdio.h>
#include <math.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <driver/gpio.h>
#include <driver/adc.h>
#include <driver/ledc.h>
#include <esp_timer.h>

// Definición del canal ADC y del pin LED
adc1_channel_t adc_pot = ADC1_CHANNEL_6;
#define LED 2

// Variables globales
float Obj = 18;
float voltajeRMS;
float Vmedido;
float voltajes[100];
void init_hw(void);
int i = 0;
esp_timer_handle_t periodic_timer;

// Prototipo de la función de interrupción
void periodic_timer_callback(void* arg);

void app_main()
{
    // Inicializar el hardware
    init_hw();

    // Configuración del temporizador de interrupción de 1 ms
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &periodic_timer_callback,
        .name = "periodic"
    };
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 1000)); // 1000 us = 1 ms

    while (1)
    {
        // Cálculo de la media después de la adquisición de 100 muestras
        for (size_t j = 1; j < 101; j++)
        {
            voltajes[j] = pow(voltajes[j], 2);
            voltajes[j] = (voltajes[j] + voltajes[j - 1]);
            printf("%d\n", j);
        }
        float vtemp = (voltajes[100] / 100);
        vtemp = sqrt(vtemp);
        printf("Voltaje: %4.2fV\n", vtemp);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Inicialización del hardware
void init_hw(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(adc_pot, ADC_ATTEN_DB_12);

    gpio_config_t io_config;
    io_config.mode = GPIO_MODE_OUTPUT;
    io_config.pin_bit_mask = (1 << LED);
    io_config.pull_down_en = GPIO_PULLDOWN_ONLY;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_config);
}

// Función de callback para la interrupción de 1 ms
void periodic_timer_callback(void* arg)
{
    // Adquirir muestra del ADC
    int pot = adc1_get_raw(adc_pot);
    Vmedido = (pot * Obj / 4095);
    voltajes[i] = Vmedido;

    // Imprimir el voltaje medido
    printf("Voltaje%d: %fV\n", i, (voltajes[i]));

    // Incrementar el índice y reiniciarlo si es necesario
    i++;
    if (i >= 100)
    {
        i = 0;
    }
}