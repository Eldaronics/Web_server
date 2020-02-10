#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float tempC;
float blank1 = 0;
float blank2 = 0;
float blank3 = 0;


byte mac[] = {
  0x48, 0xC2, 0xA1, 0xF3, 0x8D, 0xB7
};
byte ip[] = {
  192, 168, 1, 147
};

EthernetServer server = EthernetServer(80);

void setup()
{
  Serial.begin(9600);
  sensors.begin();
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop()
{
   sensors.requestTemperatures();
   
  blank1 = sensors.getTempCByIndex(0);
  blank2 = 0;
  blank3 = 0;
  
  EthernetClient client = server.available();
  if (client)
  {
    boolean BlankLine = true;
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();

        if (c == '\n' && BlankLine)
        {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html\n");
          client.println("<html><head><META HTTP-EQUIV=""refresh"" CONTENT=""5"">\n");
          client.println("<title>Web Server V1.0</title></head>");
          client.println("<body style=\"background-color:#f3f007\">\n");
          client.println("<h1>Web Server</h1>");
          client.println("<h3>Birinci b&#246lm&#601: </h3>");
          client.println(blank1);
          client.println("&deg;C");
          client.println("<br><br><br>");
          client.println("<h3>&#304kinci b&#246lm&#601: </h3>");
          client.println(blank2);
          client.println("&deg;C");
          client.println("<br><br><br>");
          client.println("<h3>&#220&#231&#252nc&#252 b&#246lm&#601: </h3>");
          client.println(blank3);
          client.println("&deg;C");
          client.println("</body>\n</html>");

          break;
        }
        if (c == '\n')
        {
          BlankLine = true;
        }
        else if (c != '\r')
        {
          BlankLine = false;
        }
      }
    }
    delay(10);
    client.stop();
  }
}
