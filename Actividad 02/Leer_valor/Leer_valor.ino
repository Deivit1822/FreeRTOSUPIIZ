// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

//variables
static const int led_pin = 33;
int valor=0;

// Mensaje inicial 
const char msg[] = "Mensaje inicial --- Actividad 02 ---";

//*****************************************************************************
// Tareas

// Tarea 1
void ReadTask(void *parameter) {
  while(true){
  while(Serial.available()==0){  
  }
  if(Serial.available() > 0){
  valor=Serial.parseInt();
  Serial.println(" Num= "+ String(valor));
    }
  }
}

// Tarea 2
void LedTask(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(valor / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(valor / portTICK_PERIOD_MS);
  }
}

//*****************************************************************************
// Main 

void setup() {

  // Serial lento para poder visualizar 
  Serial.begin(9600);

  // Retardo antes de iniciar 
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS UPIIZ ---");

  // Print self priority
  Serial.print("Configuracion y ciclo de tareas corriendo ");
  Serial.println(xPortGetCoreID());
  //Serial.print(" con prioridad ");
  //Serial.println(uxTaskPriorityGet(NULL));

  xTaskCreatePinnedToCore(ReadTask,
                          "Tarea 1",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  xTaskCreatePinnedToCore(LedTask,
                          "Tarea 2",
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
