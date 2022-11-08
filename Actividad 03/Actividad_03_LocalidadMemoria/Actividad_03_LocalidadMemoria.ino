// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Configuracion 
static const uint8_t buf_len = 255;

// variables globales
static char *msg_ptr = NULL;
static volatile uint8_t msg_flag = 0;

 
//*****************************************************************************
//Tareas

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

      
      digitalWrite(led_pin, HIGH);
    vTaskDelay(buf_len / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(buf_len / portTICK_PERIOD_MS);

      // Buffer libre, ponemos puntero en caracter nulo y limpiamos bandera de notificacion
      vPortFree(msg_ptr);
      msg_ptr = NULL;
      msg_flag = 0;
    }
  }
}

//*****************************************************************************
// Main 
void setup() {

  // 
  Serial.begin(115200);

  // 
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS ---");
  Serial.println("Actividad 03");

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
  
  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {

}
/*El codigo recibe datos del monitor serie y los almacena en el buffer de memoria, para despues mostrarnos lo que ingresamos al monitor
 * Estas acciones las hace atravez de dos tareas, se empieza haciendo las configuraciones iniciales del codigo y de las variables globales
 * despues en la tarea 1 se declaran las variables de la recepcion de datos, en este caso leemos caracteres y los alamacenamos en un vector
 * dicho vector tiene un tamaño definido y en caso de superar dicho tamaño nos marca un error, a su vez se hace uso de una bandera 
 * En la tarea 2 hacemos la impresion de los datos almacenados, para esta tarea se hace uso de la bandera que previamiente se configuro
 * la bandera de indica a la tarea 2 que ya se recibio y almaceno el mensaje de manera correcta, para que dicha tarea pueda acceder al espacio de
 * memoria donde se almaceno y pueda mostrarlo atravez del monitor serial.
 */
