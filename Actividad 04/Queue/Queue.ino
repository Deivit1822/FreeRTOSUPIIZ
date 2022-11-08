// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Settings
static const uint8_t msg_queue_len = 500;

// Globals
static QueueHandle_t msg_queue;

//*****************************************************************************
// Tasks

void printMessages(void *parameters) {

  int item;

  // Loop forever
  while (1) {

    
    if (xQueueReceive(msg_queue, (void *)&item, 0) == pdTRUE) {
    }
    Serial.println(item);

    // Wait before trying again
    vTaskDelay(1000 / portTICK_PERIOD_MS); //Queue de input
  }
}

// Main 

void setup() {

  // Configure Serial
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Queue Demo---");

  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  // Start print task
  xTaskCreatePinnedToCore(printMessages,
                          "Print Messages",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);
}

void loop() {

  static int num  = 0;

  if (xQueueSend(msg_queue, (void *)&num, 10) != pdTRUE) {
    Serial.println("Queue full");
  }
  num++;

  vTaskDelay(100 / portTICK_PERIOD_MS); //queue de output
}

/*Cambiando el delay "vTaskDelay(10 / portTICK_PERIOD_MS); //Queue de input", si el valor es muy corto se imprime el mismo numero muchas veces 
 *y si es muy largo se imprime una vez, pero cambiando el delay "vTaskDelay(1000 / portTICK_PERIOD_MS); //queue de output}" a una valor pequeño 
 * el Queue no alcanza de tiempo para sacar los datos y se llena 
 * tambien cambiando el tamaño en "static const uint8_t msg_queue_len = 5;" podemos hacer que la lista sea mas larga y no se llene tan rapido
 *de esta forma le damos tiempo suficiente para que el primer dato de la fila salga y entre un nuevo dato aunque el delay sea corto
 */
