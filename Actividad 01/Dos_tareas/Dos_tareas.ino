//#include <Arduino.h> // Si utiliza el entorno de Arduino, esta linea debera ser comentada
/**
 * Tarea que realiza el parpadeo de un LED
 */

// Configuracion de nucleos a utilizar 
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Numero de GPIO a utilizar 
static const int led_pin = 4;
static const int led_pin2 = 33;

// Tarea: blink LED
void toggleLED(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Tarea2: blink LED2
void toggleLED2(void *parameter) {
  while(1) {
    digitalWrite(led_pin2, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    digitalWrite(led_pin2, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
void setup() {

  // Configuracion de GPIO como salida
  pinMode(led_pin, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  
  // Task to run forever
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLED,    // Function to be called
              "Toggle LED1", // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              2,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)
              
  xTaskCreatePinnedToCore(            
              toggleLED2,    // Function to be called
              "Toggle LED1", // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)

}
void loop() {
  
}
