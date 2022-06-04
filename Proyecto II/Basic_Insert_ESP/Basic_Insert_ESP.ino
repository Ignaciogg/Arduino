#if ! (ESP8266 || ESP32 )
  #error This code is intended to run on the ESP8266/ESP32 platform! Please check your Tools->Board setting
#endif

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
PulseOximeter pox;

#include "Credentials.h"
#define REPORTING_PERIOD_MS     1000

#define MYSQL_DEBUG_PORT      Serial

// Debug Level from 0 to 4
#define _MYSQL_LOGLEVEL_      1

#include <MySQL_Generic.h>

#define USING_HOST_NAME     false

#if USING_HOST_NAME
  // Optional using hostname, and Ethernet built-in DNS lookup
  char server[] = "your_account.ddns.net"; // change to your server's hostname/URL
#else
  IPAddress server(195, 235, 211, 197);
#endif

uint16_t server_port = 3306;    //3306;

char default_database[] = "prigO2theTop";           //"test_arduino";
char default_table[]    = "Datos_Sensores (id_Sesion, sensor, fecha_hora_lectura, valor)";          //"test_arduino";
String default_value = ""; 
String INSERT_SQL = "";
int i = 0;


//            Pulsioximetro
//======================================================================

uint32_t tsLastReport = 0;

void onBeatDetected()
{
    Serial.println("Pulso detectado!");
}

void setup()
{
    Serial.begin(115200);

    Serial.print("Inicializando pulsioximetro...");

    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    pox.setOnBeatDetectedCallback(onBeatDetected);

    Serial.begin(115200);
  while (!Serial);

  MYSQL_DISPLAY1("\nStarting Basic_Insert_ESP on", ARDUINO_BOARD);
  MYSQL_DISPLAY(MYSQL_MARIADB_GENERIC_VERSION);

  // Begin WiFi section
  MYSQL_DISPLAY1("Connecting to", ssid);
  
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    MYSQL_DISPLAY0(".");
  }

  // print out info about the connection:
  MYSQL_DISPLAY1("Connected to network. My IP address is:", WiFi.localIP());

  MYSQL_DISPLAY3("Connecting to SQL Server @", server, ", Port =", server_port);
  MYSQL_DISPLAY5("User =", user, ", PW =", password, ", DB =", default_database);

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

MySQL_Connection conn((Client *)&client);
MySQL_Query *query_mem;

//======================================================================
void loop()
{
    pox.update();
    float frecs[11];
    int oxs[11];
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
    Serial.println(i);
    
    if (i>10){
      quicksort(oxs, 0, 10);
      for (int i = 0; i<11;i++){
        Serial.print(frecs[i]);
        Serial.print(", ");
      }
      Serial.println("");
      i=0;
      delay(3000);
      MYSQL_DISPLAY("Connecting...");

      default_value = "(7, 'Prueba', '2022-05-26 11:00:00', '99')";
      INSERT_SQL = String("INSERT INTO ") + default_database + "." + default_table 
                   + " VALUES " + default_value +";";
  
      //if (conn.connect(server, server_port, user, password))
      if (conn.connectNonBlocking(server, server_port, user, password) != RESULT_FAIL)
      {
        delay(500);
        runInsert();
        conn.close();                     // close the connection
      } 
      else 
      {
        MYSQL_DISPLAY("\nConnect failed. Trying again on next iteration.");
      }
    
      MYSQL_DISPLAY("\nSleeping...");
      MYSQL_DISPLAY("================================================");
     
      delay(60000);
    }

}

void runInsert()
{
  // Initiate the query class instance
  MySQL_Query query_mem = MySQL_Query(&conn);

  if (conn.connected())
  {
    MYSQL_DISPLAY(INSERT_SQL);
    
    // Execute the query
    // KH, check if valid before fetching
    if ( !query_mem.execute(INSERT_SQL.c_str()) )
    {
      MYSQL_DISPLAY("Insert error");
    }
    else
    {
      MYSQL_DISPLAY("Data Inserted.");
    }
  }
  else
  {
    MYSQL_DISPLAY("Disconnected from Server. Can't insert.");
  }
}
