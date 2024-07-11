

/*
IINPUTS
LSC = PIN13
LSA = PIN 12
FC = PIN 14
CC = 27
CA = 26

OUTPUTS
LED_M = PIN 4
LED_E = PIN16
MC = PIN 17
MA = PIN 5
*/

//Librerias utilizadas
#include <stdio.h> //Libreria del lenguaje C
#include <stdlib.h>
#include <freertos/FreeRTOS.h>//Libreria que nos permite usar los delay
#include <freertos/task.h>
#include <driver/gpio.h> //libreria que nos permite el manejo dde kis oerifericos GPIO
#include <freertos/timers.h> //Libreria para utilizar el timer 
#include "esp_log.h" //Libreria para consultar el resultado
#include "i2c-lcd.h"
#include "esp_log.h"
#include "driver/i2c.h"

//definiciones de variables

static const char *tag = "Main";

#define TRUE 1
#define FALSE 0

#define ESTADO_INIT         0
#define ESTADO_ABRIENDO     1
#define ESTADO_CERRANDO     2
#define ESTADO_CERRADO      3
#define ESTADO_ABIERTO      4
#define ESTADO_EMERGENCIA   5
#define ESTADO_ERROR        6
#define ESTADO_ESPERA       7

//Definicion de la variable utilizada como handle
TimerHandle_t xTimers;
int timerID = 1;
int INTERVALO = 25;

//DECLARACION DE FUNCIONES
esp_err_t INTERRUPCION_25MS(void); //Funcion de lectura de los pines cada interrupcion
esp_err_t SET_TIMER(void);
static esp_err_t i2c_master_init(void);

static const char *TAG = "i2c-simple-example";

int Func_ESTADO_INIT();
int Func_ESTADO_ABRIENDO();
int Func_ESTADO_CERRANDO();
int Func_ESTADO_CERRADO();
int Func_ESTADO_ABIERTO();
int Func_ESTADO_EMERGENCIA();
int Func_ESTADO_ERROR();
int Func_ESTADO_ESPERA();

/*****************************************************/

#define TRUE 1
#define FALSE 0

#define ESTADO_INIT         0
#define ESTADO_ABRIENDO     1
#define ESTADO_CERRANDO     2
#define ESTADO_CERRADO      3
#define ESTADO_ABIERTO      4
#define ESTADO_EMERGENCIA   5
#define ESTADO_ERROR        6
#define ESTADO_ESPERA       7



volatile int ESTADO_ACTUAL = ESTADO_INIT;
volatile int ESTADO_SIGUIENTE = ESTADO_INIT;
volatile int ESTADO_ANTERIOR = ESTADO_INIT;
volatile unsigned int TimeCa = 0;
volatile int CONTADOR = 0;


volatile struct INPUTS{
    unsigned int LSA: 1;
    unsigned int LSC: 1;
    unsigned int CA:  1;
    unsigned int CC:  1;
    unsigned int FC:  1;
}inputs;
volatile struct OUTPUTS{
    unsigned int MC: 1;
    unsigned int MA: 1;
    unsigned int LED_EMERGENCIA: 1;
    unsigned int LED_MOVIMIENTO: 1;
}outputs;


/**************************************************** */

void vTimerCallback( TimerHandle_t pxTimer){

    if(ESTADO_ACTUAL == ESTADO_CERRANDO || ESTADO_ACTUAL == ESTADO_ABRIENDO){
        if(inputs.CA = TRUE || inputs.CA == TRUE){

            CONTADOR = 0;
        }
        CONTADOR++;

    }else{

        CONTADOR = 0;
    }
    INTERRUPCION_25MS();
}

void app_main() {


    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully"); 

    ESTADO_SIGUIENTE = Func_ESTADO_INIT();
    SET_TIMER();

    lcd_put_cur(0,0);
    lcd_send_string("Albert Cabra");
    lcd_put_cur(1,0);
    lcd_send_string("PUERTON.");
    lcd_clear();



    for (;;)
    {    
        if (ESTADO_SIGUIENTE == ESTADO_INIT)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_INIT();
        }
        if (ESTADO_SIGUIENTE == ESTADO_ESPERA)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_ESPERA();
        }
        if (ESTADO_SIGUIENTE == ESTADO_ABRIENDO)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_ABRIENDO();
        }
        if (ESTADO_SIGUIENTE == ESTADO_CERRANDO)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_CERRANDO();
        }
        if (ESTADO_SIGUIENTE == ESTADO_CERRADO)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_CERRADO();
        }
        if (ESTADO_SIGUIENTE == ESTADO_ABIERTO)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_ABIERTO();
        }
        if (ESTADO_SIGUIENTE == ESTADO_EMERGENCIA)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_EMERGENCIA();
        }
        if (ESTADO_SIGUIENTE == ESTADO_ERROR)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_ERROR();
        }
        
    }

}


int Func_ESTADO_INIT(){
    // Registro en el log e inicialización del mensaje en la pantalla
    ESP_LOGE(tag, "INICIANDO PROGRAMA");
    lcd_put_cur(0,0);
    lcd_send_string("Iniciando...");

    // Actualizar variables de estado
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_INIT;

    // Registro en el log del mensaje de configuración de pines
    ESP_LOGE(tag, "CONFIGURACION DE LOS PINES");

    // Estructura de configuración de pines
    gpio_config_t IO_CONFIG;

    // Configurar pines de entrada
    IO_CONFIG.mode = GPIO_MODE_INPUT;
    IO_CONFIG.pin_bit_mask = (1 << 13) | (1 << 12) | (1 << 14) | (1 << 27) | (1 << 26);
    IO_CONFIG.pull_down_en = GPIO_PULLDOWN_ENABLE;
    IO_CONFIG.pull_up_en = GPIO_PULLUP_DISABLE;
    IO_CONFIG.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&IO_CONFIG);

    // Configurar pines de salida
    IO_CONFIG.mode = GPIO_MODE_OUTPUT;
    IO_CONFIG.pin_bit_mask = (1 << 4) | (1 << 16) | (1 << 17) | (1 << 5);
    IO_CONFIG.pull_down_en = GPIO_PULLDOWN_DISABLE;
    IO_CONFIG.pull_up_en = GPIO_PULLUP_DISABLE;
    IO_CONFIG.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&IO_CONFIG);

    for (;;)
    {
        return ESTADO_ESPERA;
    }
}


int Func_ESTADO_ABRIENDO(){
    // Mensaje de apertura del portón
    printf("ABRIENDO EL PORTON");
    lcd_clear();
    lcd_put_cur(0,0);
    lcd_send_string("ABRIENDO PORTON");

    // Actualizar variables de estado
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ABRIENDO;

    // Configuración de salidas en este estado
    outputs.LED_MOVIMIENTO = TRUE;
    outputs.LED_EMERGENCIA = FALSE;
    outputs.MA = TRUE;
    outputs.MC = FALSE;

    for (;;)
    {
        // Verificar si se activa el interruptor de límite de abierta
        if(inputs.LSA == TRUE){
            return ESTADO_ABIERTO; // Retorna el estado del portón abierto
        }
        if (inputs.LSA == TRUE && inputs.LSC == TRUE){
            return ESTADO_ERROR;
        }
        if (inputs.FC == TRUE){
            return ESTADO_EMERGENCIA;
        }
        if(inputs.CC == TRUE ){
            return ESTADO_CERRANDO;
        }
        if (CONTADOR == 3600){
            return ESTADO_ERROR;
        }
    }
}

int Func_ESTADO_ABIERTO(){
    // Mensaje de portón abierto
    printf("PORTON ABIERTO");
    lcd_clear();
    lcd_put_cur(0,0);
    lcd_send_string("PORTON ABIERTO");

    // Actualizar variables de estado
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ABIERTO;

    // Configuración de salidas en este estado
    outputs.LED_MOVIMIENTO = FALSE;
    outputs.LED_EMERGENCIA = FALSE;
    outputs.MA = FALSE;
    outputs.MC = FALSE;

    for (;;)
    {
        return ESTADO_ESPERA;
    }
}

int Func_ESTADO_CERRANDO(){
    // Mensaje de cierre del portón
    printf("CERRANDO EL PORTON");
    lcd_clear();
    lcd_put_cur(0,0);
    lcd_send_string("CERRANDO PORTON");

    // Actualizar variables de estado
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_CERRANDO;

    // Configuración de salidas en este estado
    outputs.LED_MOVIMIENTO = TRUE;
    outputs.LED_EMERGENCIA = FALSE;
    outputs.MA = FALSE;
    outputs.MC = TRUE;

    for (;;)
    {
        // Verificar si se activa el interruptor de límite de cerrada
        if (inputs.LSC == TRUE){
            return ESTADO_CERRADO;
        }
        if (inputs.LSA == TRUE && inputs.LSC == TRUE){
            return ESTADO_ERROR;
        }
        if (inputs.FC == TRUE){
            return ESTADO_EMERGENCIA;
        }
        if (inputs.CA == TRUE){
            return ESTADO_ABRIENDO;
        }
        if (CONTADOR == 3600){
            return ESTADO_ERROR;
        }
    }
}

int Func_ESTADO_CERRADO(){
    // Mensaje de portón cerrado
    printf("PORTON CERRADO ");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_CERRADO;
    lcd_clear();
    lcd_put_cur(0,0);
    lcd_send_string("PORTON CERRADO");
  
    // Configuración de salidas en este estado
    outputs.LED_MOVIMIENTO = FALSE;
    outputs.LED_EMERGENCIA = FALSE;
    outputs.MA = FALSE;
    outputs.MC = FALSE;

    for (;;)
    {
        return ESTADO_ESPERA;
    }
}



int Func_ESTADO_ERROR(){
    // Mensaje de error en la pantalla
    lcd_clear();
    lcd_put_cur(0,1);
    lcd_send_string("ERROR");

    // Actualizar variables de estado
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ERROR;

    // Configuración de salidas en este estado
    outputs.LED_MOVIMIENTO = FALSE;
    outputs.LED_EMERGENCIA = TRUE;
    outputs.MA = FALSE;
    outputs.MC = FALSE;

    // Mensaje de error
    printf("\nERROR!");

    for (;;)
    {
        vTaskDelay(500/portTICK_PERIOD_MS);
        outputs.LED_EMERGENCIA = FALSE;
        vTaskDelay(500/portTICK_PERIOD_MS);
        outputs.LED_EMERGENCIA = TRUE;
        vTaskDelay(500/portTICK_PERIOD_MS);
        outputs.LED_EMERGENCIA = FALSE;

        return ESTADO_ESPERA;
    }
}

int Func_ESTADO_EMERGENCIA(){
    // Mensaje de estado de emergencia
    printf("ESTADO DE EMERGENCIA");
    lcd_clear();
    lcd_put_cur(0,0);
    lcd_send_string("ESTADO DE EMERGENCIA");

    // Actualizar variables de estado
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_EMERGENCIA;

    // Configuración de salidas en este estado
    outputs.LED_MOVIMIENTO = FALSE;
    outputs.LED_EMERGENCIA = TRUE;
    outputs.MA = FALSE;
    outputs.MC = FALSE;
    
    for (;;)
    {
        vTaskDelay(1500/portTICK_PERIOD_MS);
        if (inputs.FC == FALSE)
        {
            return ESTADO_ANTERIOR;
        }
    }
}


int Func_ESTADO_ESPERA(){
    // Mensaje de estado de espera en la pantalla
    lcd_clear();
    lcd_put_cur(0,0);
    lcd_send_string("ESPERANDO");

    // Actualizar variables de estado
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ESPERA;

    // Configuración de salidas en este estado
    outputs.LED_EMERGENCIA = FALSE;
    outputs.LED_MOVIMIENTO = FALSE;
    outputs.MA = FALSE;
    outputs.MC = FALSE;
    ESP_LOGE(tag, "ESTADO ESPERA");

    for (;;)
    {
        // Verificar entradas para el siguiente movimiento
        if (inputs.LSA == FALSE && inputs.FC == FALSE && inputs.LSC == FALSE){
            return ESTADO_CERRANDO;
        }
        if (inputs.CA == TRUE && inputs.FC == FALSE && inputs.LSA == FALSE && inputs.FC == FALSE){
            return ESTADO_ABRIENDO;
        }
        if (inputs.CC == TRUE && inputs.FC == FALSE){
            return ESTADO_CERRANDO;
        }
        if (inputs.CA == TRUE && inputs.FC == FALSE){
            return ESTADO_ABRIENDO;
        }
        if (inputs.FC == TRUE){
            return ESTADO_EMERGENCIA;
        }
        if(inputs.LSA == TRUE && inputs.LSC == TRUE){
            return ESTADO_ERROR;
        }
    }
}

esp_err_t SET_TIMER(void){
    // Iniciar la configuración del temporizador
    printf("Iniciando la configuracion del timer...");

    xTimers = xTimerCreate("Timer", 
                          (pdMS_TO_TICKS(INTERVALO)),
                          pdTRUE, 
                          ( void * )timerID, 
                          vTimerCallback);
        
    if (xTimers == NULL)
    {
        printf("El timer no pudo ser creado");
    }else 
    {
        if (xTimerStart(xTimers, 0) != pdPASS)
        {
            printf("El timer puede no estar configurado");
        }
    }

    return ESP_OK;
}

esp_err_t INTERRUPCION_25MS(void){
    // Configuración de los pines del microcontrolador
    // Pines de entrada
    inputs.LSC = (gpio_get_level(13) == TRUE)?TRUE:FALSE;
    inputs.LSA = (gpio_get_level(12) == TRUE)?TRUE:FALSE;
    inputs.FC = (gpio_get_level(14) == TRUE)?TRUE:FALSE;
    inputs.CC = (gpio_get_level(27) == TRUE)?TRUE:FALSE;
    inputs.CA = (gpio_get_level(26) == TRUE)?TRUE:FALSE;

    // Pines de salida
    if (outputs.LED_MOVIMIENTO == TRUE)
    {
        gpio_set_level(4,TRUE);
    }else
    {
        gpio_set_level(4,FALSE);
    }
    if (outputs.LED_EMERGENCIA == TRUE)
    {
        gpio_set_level(16,TRUE);
    }else
    {
        gpio_set_level(16,FALSE);
    }
    if (outputs.MC == TRUE)
    {
        gpio_set_level(17,TRUE);
    }else
    {
        gpio_set_level(17,FALSE);
    }
    if (outputs.MA == TRUE)
    {
        gpio_set_level(5,TRUE);
    }else
    {
        gpio_set_level(5,FALSE);
    }

    return ESP_OK;
}

static esp_err_t i2c_master_init(void){
    // Inicialización del maestro I2C
    int i2c_master_port = I2C_NUM_0;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}