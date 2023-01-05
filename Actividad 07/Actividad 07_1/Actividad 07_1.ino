
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Configuracion 
static const uint8_t buf_len = 255;

// Variables globales
static TimerHandle_t one_shot_timer = NULL;
static char *msg_ptr = NULL;
static volatile uint8_t msg_flag = 0;

//*****************************************************************************
// Callbacks
// La función es llamada cuando el timer expira
void myTimerCallback(TimerHandle_t xTimer) {

  // Mensaje del timer 0
  if ((uint32_t)pvTimerGetTimerID(xTimer) == 1) {
    Serial.println("One-shot timer expired");
  }
}

//Tareas: lectura desde el buffer del serial 
void readSerial(void *parameters){
  
  char c;
  char buf[buf_len];
  uint8_t idx = 0;

  //Limpieza de buffer
  memset(buf, 0, buf_len);

  //Loop infinito 
  while(1) {
    
    // Lectura de caracteres del serial 
    if(Serial.available() > 0){
      c = Serial.read();

      //Almacena caracteres recibidos en el buffer si no sobre pasa el limite del 
      if(idx < buf_len - 1){
        buf[idx] = c;
        idx++;
      }

      //Creamos el buffer para imprimir el mensaje
      if(c == '\n'){

        //Ultimo caracter '\n', se reemplaza por un caracter nulo 
        // ´\n´
        buf[idx - 1]= '\0';

        //Intenta ubicar el mensaje en la localidad de memoria, si esta en uso lo 
        if(msg_flag ==0){
          msg_ptr =(char *)pvPortMalloc(idx* sizeof(char));

          //Si regresa un cero entonces se borra 
          configASSERT(msg_ptr);

          // Copiamos mensaje 
          memcpy(msg_ptr, buf, idx);

          //Notificacion de que estaa listo el mensaje
          msg_flag = 1;
        }

        //Reinicio de buffer y el puntero 
        memset(buf, 0, buf_len);
        idx = 0;
      }
    } 
  }
}

// Tarea: imprime mensaje siempre y cuando la bandera este en alto y el buffer libre 
void printMessage(void *parameters) {
  while (1) {

    // Esperamos bandera
    if (msg_flag == 1) {
      Serial.println(msg_ptr);

      
      xTimerStart(one_shot_timer, portMAX_DELAY);

      // Buffer libre, ponemos puntero en caracter nulo y limpiamos bandera de notificacion
      vPortFree(msg_ptr);
      msg_ptr = NULL;
      msg_flag = 0;
    }
  }
}

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {

  // Configure Serial
  Serial.begin(115200);

  // retardo 
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Timers---");

   // Start Serial receive task
  xTaskCreatePinnedToCore(readSerial,
                          "Lectura Serial",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Start Serial print task
  xTaskCreatePinnedToCore(printMessage,
                          "Escritura Mensaje",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Timer Simple tiro
  one_shot_timer = xTimerCreate(
                      "One-shot timer",           // Name of timer
                      2000 / portTICK_PERIOD_MS,  // Period of timer (in ticks)
                      pdFALSE,                    // Auto-reload
                      (void *)1,                  // Timer ID
                      myTimerCallback);           // Callback function

  // Garantizamos que fue creado
  if (one_shot_timer == NULL) {
    Serial.println("Could not create one of the timers");
  } else {
    
    // mensaje inicial 
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println("Starting timers...");

    // Start timers
    //xTimerStart(one_shot_timer, portMAX_DELAY);
  }
}

void loop() {
  // Execution should never get here
}
