#include <Otto.h>
#include <Otto_gestures.h>
#include <Otto_mouths.h>
#include <Otto_sounds.h>

#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Otto Otto;  //This is Otto!

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

const unsigned char PROGMEM corazon1 [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x07, 0xC0, 0x03, 0xF8, 0x0F, 0xF0,
0x0F, 0xFE, 0x3F, 0xF8, 0x0F, 0xFE, 0x3F, 0xFC, 0x1F, 0xFF, 0x7F, 0xFC, 0x3F, 0xFF, 0xFF, 0xFC,
0x3F, 0xFF, 0xFF, 0xFE, 0x3F, 0xFF, 0xFF, 0xFE, 0x3F, 0xFF, 0xFF, 0xFE, 0x3F, 0xFF, 0xFF, 0xFE,
0x3F, 0xFF, 0xFF, 0xFC, 0x3F, 0xFF, 0xFF, 0xFC, 0x1F, 0xFF, 0xFF, 0xFC, 0x1F, 0xFF, 0xFF, 0xFC,
0x0F, 0xFF, 0xFF, 0xF8, 0x0F, 0xFF, 0xFF, 0xF8, 0x07, 0xFF, 0xFF, 0xF0, 0x03, 0xFF, 0xFF, 0xF0,
0x03, 0xFF, 0xFF, 0xC0, 0x01, 0xFF, 0xFF, 0xC0, 0x00, 0xFF, 0xFF, 0xC0, 0x00, 0xFF, 0xFF, 0x00,
0x00, 0x7F, 0xFE, 0x00, 0x00, 0x1F, 0xFC, 0x00, 0x00, 0x1F, 0xFC, 0x00, 0x00, 0x07, 0xF8, 0x00,
0x00, 0x07, 0xF0, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x80, 0x00
};

 
#define LED 13
const int sensorPin = 7;
char lectura; //Variable para guardar la lectura del Serial
//String comando;
Servo servoX;
Servo servoY;
int ejeX;
int ejeY;
int gradosEjeX;
int gradosEjeY;
const int EJEX_INICIAL = 90;
const int EJEY_INICIAL = 120; 
boolean duerme;
int num_caricias = 0;

void setup()
{
   //Iniciamos el Serial:
   Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  Otto.init(11,11,11,11,false, -1, 12, 8, 9);
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  //delay(2000);

  // Clear the buffer.
  display.clearDisplay();
   
   servoX.attach(4);
   servoY.attach(5);
   //Inicializamos el pin 13
   pinMode(LED, OUTPUT);
   pinMode(sensorPin, INPUT);
   Serial.println("N");
  // Inicializamos los servos
  ejeX = EJEX_INICIAL;
  ejeY = EJEY_INICIAL;
  servoX.write(ejeX);
  servoY.write(ejeY);   
  duerme = true;

  // draw a white circle, 10 pixel radius
  display.fillRect(display.width()/2, display.height()/2, 15,1, WHITE);
  display.display();
}
 
void loop()
{

   //Si recibimos algo por serial, lo guardamos
      String comando = "";
      if(Serial.available() >= 1 and duerme == false)
      {
        comando = Serial.readStringUntil('\n');
        Serial.println("N");
        //Serial.println(comando);
      }
      
        if (comando[0]== 'S')
        {
          gradosEjeX = stringToNumber(comando.substring(1,4));
          gradosEjeY = stringToNumber(comando.substring(4,7));
          
          mueveGradosX(gradosEjeX);
          mueveGradosY(gradosEjeY);
        
          pestaneo(1);               

          Serial.println("Y");
        }
        else
        {
          sensorTactil();
        } 
}

int stringToNumber(String thisString) {
  int i, value, length;
  length = thisString.length();
  char blah[(length+1)];
  for(i=0; i<length; i++) {
    blah[i] = thisString.charAt(i);
  }
  blah[i]=0;
  value = atoi(blah);
  return value;
}

void mueveGradosX(int num_gradosX)
{
  int cont = abs(num_gradosX);

  for (int i=0; i<cont; i++)
  {
      if (num_gradosX > 0)
      {
          ejeX= ejeX + 1;
      }
      else
      {
          ejeX = ejeX -1;
      }
      servoX.write(ejeX);
      delay(10);
  }
}

void mueveGradosY(int num_gradosY)
{
  int cont = abs(num_gradosY);

  for (int i=0; i<cont; i++)
  {
      if (num_gradosY > 0)
      {
          ejeY= ejeY - 1;
      }
      else
      {
          ejeY = ejeY + 1;
      }
      servoY.write(ejeY);
      delay(10);
  }
}

void wakeUp()
{
  display.clearDisplay();
  display.display();  
  display.fillRect(display.width()/2, display.height()/2, 6,2, WHITE);
  display.display();
  
  for (int i=0; i<60; i++)
  {
    ejeY = ejeY - 1;
    servoY.write(ejeY);
    delay(30);
  }
  delay(300);  
  servoX.write(120); 
  delay(100);
  servoX.write(70);  
  delay(100);
  servoX.write(120);
  delay(100);
  servoX.write(70);
  delay(100);
  servoX.write(120);
  delay(100);
  servoX.write(70);
  delay(100);
  servoX.write(90);
  delay(150);
   pestaneo(2);
   Serial.println("Y");  
}



void pestaneo(int numVeces)
{
  for (int i=0; i<numVeces; i++)
  {
    display.clearDisplay();
    display.display();
    display.fillRect(display.width()/2, display.height()/2, 15,1, WHITE);
    display.display();
    
    delay(100);
    display.clearDisplay();
    display.display();
    display.fillCircle(display.width()/2, display.height()/2, 5, WHITE);
    display.display();
    delay(100);
  }
}

void inlove()
{
  display.clearDisplay();
  display.display();
   
  for (int i=0; i<3; i++)
  {
    display.drawBitmap(42, 1, corazon1, 32, 32, WHITE);
    display.display();
    delay(100);
    display.clearDisplay();
    display.display();
    delay(100);
  }
  display.drawBitmap(42, 1, corazon1, 32, 32, WHITE);
  display.display();
  delay(1000);
  pestaneo(2);
  
}

void caricia()
{
  num_caricias = num_caricias + 1;
  
  display.clearDisplay();
  display.display();
  display.fillRect(display.width()/2, display.height()/2, 15,1, WHITE);
  display.display();
  
  for (int i=0; i<20; i++)
  {
    ejeY = ejeY + 1;
    servoY.write(ejeY);
    delay(20);
  }

  for (int i=0; i<20; i++)
  {
    ejeY = ejeY -1;
    servoY.write(ejeY);
    delay(20);
  }

  if (num_caricias < 3)
  {
    pestaneo(3);
    Otto.sing(S_fart1);
    Serial.println("Y");
  }
  else
  {
    Otto.sing(S_superHappy);
    inlove();
    posicionInicial();
    num_caricias = 0;
    Otto.sing(S_disconnection);
    Serial.println("N");
  }
  
}

void sensorTactil()
{
    int sensor = digitalRead(sensorPin);
    if (sensor == HIGH)
    {
      Serial.println("N");
      if (duerme == true)
      {
        Otto.sing(S_connection);
        wakeUp();
        Otto.sing(S_happy);
        duerme = false;
        Serial.println("Y");   
      }
      else
      {
        caricia();
      }
      
    } 
}

void posicionInicial()
{
  int cont = 0;
  cont = ejeX - EJEX_INICIAL;
  for (int i=0; i< abs(cont); i++)
  {
    if (cont > 0)
    {
      ejeX = ejeX - 1;
    }
    else
    {
      ejeX = ejeX + 1;
    }
    servoX.write(ejeX);
    delay(20);
  }

  cont = ejeY - EJEY_INICIAL;
  for (int i=0; i< abs(cont); i++)
  {
    if (cont > 0)
    {
      ejeY = ejeY - 1;
    }
    else
    {
      ejeY = ejeY + 1;
    }
    servoY.write(ejeY);
    delay(20);
  }
  display.clearDisplay();
  display.display();
  display.fillRect(display.width()/2, display.height()/2, 15,1, WHITE);
  display.display();
  duerme = true;
  
}
