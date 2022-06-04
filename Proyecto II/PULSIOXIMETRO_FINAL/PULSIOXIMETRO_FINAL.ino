#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000

PulseOximeter pox;

uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup()
{
    Serial.begin(115200);

    Serial.print("Initializing pulse oximeter..");

    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    pox.setOnBeatDetectedCallback(onBeatDetected);
}
//funcion para dividir el array y hacer los intercambios
int dividir(int *array, int inicio, int fin)
{
  int izq;
  int der;
  int pibote;
  int temp;

  pibote = array[inicio];
  izq = inicio;
  der = fin;

  //Mientras no se cruzen los índices
  while (izq < der){
    while (array[der] > pibote){
    der--;
    }

  while ((izq < der) && (array[izq] <= pibote)){
      izq++;
    }

    // Si todavia no se cruzan los indices seguimos intercambiando
  if(izq < der){
      temp= array[izq];
      array[izq] = array[der];
      array[der] = temp;
    }
  }

  //Los indices ya se han cruzado, ponemos el pivote en el lugar que le corresponde
  temp = array[der];
  array[der] = array[inicio];
  array[inicio] = temp;

  //La nueva posición del pivote
  return der;
}

//            Funcion Quicksort
//======================================================================
//funcion recursiva para hacer el ordenamiento
void quicksort(int *array, int inicio, int fin)
{
  int pivote;
  if(inicio < fin)
  {
    pivote = dividir(array, inicio, fin );
    quicksort( array, inicio, pivote - 1 );//ordeno la lista de los menores
    quicksort( array, pivote + 1, fin );//ordeno la lista de los mayores
  }
}

//======================================================================
void loop()
{
    pox.update();
    float frecs[11];
    int oxs[11];
    int i = 0;
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
      Serial.print("Heart rate:");
      Serial.print(pox.getHeartRate());
      frecs[i] = pox.getHeartRate();
      Serial.print("bpm / SpO2:");
      Serial.print(pox.getSpO2());
      Serial.println("%");
      oxs[i] = pox.getSpO2();

      tsLastReport = millis();
      i += 1;
    }
    
    if (i>10){
      quicksort(oxs, 0, 10);
    for (int i = 0; i<11;i++){
      Serial.print(frecs[i]);
      Serial.print(", ");
    }
    Serial.println("");
    i=0;
    delay(3000);
    }
    
}
