#if ! (ESP8266 || ESP32 )
  #error No se esta usando un ESP8266/ESP32
#endif

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#include "Credentials.h"

#define MYSQL_DEBUG_PORT      Serial
#define REPORTING_PERIOD_MS     1000

#define _MYSQL_LOGLEVEL_      1

#include <MySQL_Generic.h>

#define USING_HOST_NAME     false

#if USING_HOST_NAME
// Optional using hostname, and Ethernet built-in DNS lookup
char server[] = ""; // change to your server's hostname/URL
#else
IPAddress server(195, 235, 211, 197);
#endif

uint16_t server_port = 3306;    //3306;

char database[] = "prigO2theTop";           //"test_arduino";
char table[]    = "Datos_Sensores";          //"test_arduino";

String temp, query;

PulseOximeter pox;

uint32_t tsLastReport = 0;


MySQL_Connection conn((Client *)&client);

MySQL_Query *query_mem;

int Vo, sp02;
int avgSp02 = 0;
int i = 0;
float bpm;
float avgBpm = 0;
float avgTemp = 0;
float R1 = 100000;              // resistencia fija del divisor de tension (10k)
float logR2, R2, TEMPERATURA;
float c1 = 2.114990448e-03, c2 = 0.3832381228e-04, c3 = 5.228061052e-07;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
  Serial.println("Dedo detectado");
  
  if((int) bpm > 0 && sp02 > 50  && TEMPERATURA > 34){
    Serial.println("Datos+1");
    i++;
    avgBpm += bpm;
    avgSp02 +=sp02;
    avgTemp += TEMPERATURA;

    if (i == 20){

      avgBpm = avgBpm/20;
      avgSp02 =avgSp02/20;
      avgTemp = avgTemp/20;

      query = String("INSERT INTO ") + database + "." + table
            + " (id_Dato, id_Sesion, sensor, fecha_hora_lectura, valor) VALUES (100, 'Prueba', '2022-03-12 10:34:09', '30')";
      
      conn.connect(server, server_port, user, password);
      runInsert(query);
      conn.close();
  
      digitalWrite(14,HIGH);
      i = 0;
      avgBpm = 0;
      avgSp02 = 0;
      avgTemp = 0;
    }
  }
}

void setup()
{
  pinMode(14,OUTPUT);
  
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
    Serial.print(".");
  }

  //    // print out info about the connection:
  //    MYSQL_DISPLAY1("Connected to network. My IP address is:", WiFi.localIP());
  //
  //    MYSQL_DISPLAY3("Connecting to SQL Server @", server, ", Port =", server_port);
  //    MYSQL_DISPLAY5("User =", user, ", PW =", password, ", DB =", database);

  Serial.print("Initializing pulse oximeter..");

  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip 
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }
  
  pox.setOnBeatDetectedCallback(onBeatDetected);

}


void runInsert(String INSERT_SQL)
{
  // Initiate the query class instance
  MySQL_Query query_mem = MySQL_Query(&conn);

  if (conn.connected())
  {
    MYSQL_DISPLAY(INSERT_SQL);

    // Execute the query
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
    MYSQL_DISPLAY("No hay conexion con el servidor");
  }
}



void loop()
{
  // Make sure to call update as fast as possible
  pox.update();

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

    Vo = analogRead(A0);
    R2 = R1 * (1023.0 / (float)Vo - 1.0); // conversion de tension a resistencia
    logR2 = log(R2);
    TEMPERATURA = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
    TEMPERATURA = TEMPERATURA - 275.15;   // Kelvin a Centigrados (Celsius) + correccion de desfase
    bpm = pox.getHeartRate();
    sp02 = pox.getSpO2() + 2;
    if (sp02 == 2) {
      sp02 = 0;
    }

    if (sp02 >= 100) {
      sp02 = 100;
    }

    Serial.print("Heart rate:");
    Serial.print(bpm);
    Serial.print("bpm / SpO2:");
    Serial.print(sp02);
    Serial.print("% / temp:");
    Serial.print(TEMPERATURA);
    Serial.println("ºC");

    tsLastReport = millis();

  }

}
