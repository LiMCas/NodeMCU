#include <Arduino.h>
#include <ESP8266WiFi.h>

const char ssid[] = "NodeMCU_Limber"; //El ssid permite identificar el nombre de la red
const char password [] = "abc123456"; //Para ingresar a la red
WiFiServer server(80);                //puerto de comunicaciones

int pinLed = 2;                       //Declara el pin
int estado = LOW;                     //Declara el estado

void setup() {
  Serial.begin(115200);               //Inicializa la salida serial  con un valor en Baudios

  pinMode(pinLed, OUTPUT);           //Inicializar GPIO2
  digitalWrite(pinLed, estado);         //valor inicial del pin

  server.begin();                     //inicia le servidor

  WiFi.mode(WIFI_AP);                //Estable el modo de conexión
  WiFi.softAP(ssid, password);       //Muestra la red

  Serial.print("Dirección IP Acces Point");
  Serial.println(WiFi.softAPIP());    //Imprimer la direccion iP

  Serial.print("Dirección MAC");
  Serial.println(WiFi.softAPmacAddress());  // Imprime la direccion MAC

}
void loop () {
  // Comprobar si el cliente se conecto
  WiFiClient client = server.available();
  if (!client){
    return;
  }
  //Espera la comunicación de alguien
  Serial.println("Nuevo cliente");
  while (!client.available()) {
    delay(1);
  }
  //Imprimir la candida de clientes conextadas
  Serial.printf("Clientes conectados al Acces Point: %d\n",WiFi.softAPgetStationNum());

  //Lee una petición
  String peticion = client.readStringUntil('r');
  Serial.println(peticion);
  Serial.println(peticion);
  client.flush();

  //Comprobar la petición
  if (peticion.indexOf("/LED=ON") != -1) {
    estado = HIGH;
  }
  if (peticion.indexOf("/LED=OFF") != -1) {
    estado = LOW;
  }

  //Enciende el LED en funcion de la petición
  digitalWrite(pinLed, estado);
  //Enviar pagina HTML de respuesta al clientes
  // Envía la página HTML de respuesta al cliente
  //client.println("Servidor HTTP");
  //client.println("");                                     //No olvidar esta línea de separación
  client.println("<!DOCTYPE HTML>");
  client.println("<meta charset='UTF-8'>");
  client.println("<html>");

  //Imprime el estado del led
    client.print("<h1>El LED está ahora: ");
    if(estado == HIGH) {
      client.print("ENCENDIDO</h1>");
      } else {
        client.print("APAGADO</h1>");
    }
    //Se crean botones con estilos para modificar el estado del LED
client.println("<button type='button' onClick=location.href='/LED=ON' style='margin:auto; background-color:green; color:#A9F5A9; padding:5px; border:2px solid black; width:200;'><h2> ENCENDER</h2> </button>");
client.println("<button type='button' onClick=location.href='/LED=OFF' style='margin:auto; background-color:red; color:#F6D8CE; padding:5px; border:2px solid black; width:200;'><h2> APAGAR</h2> </button><br><br>");

client.println("</html>");
delay(1);
Serial.println("Petición finalizada");          // Se finaliza la petición al cliente. Se inicaliza la espera de una nueva petición.

  }
