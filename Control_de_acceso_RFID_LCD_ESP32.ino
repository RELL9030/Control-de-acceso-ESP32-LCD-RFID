/*
 * Date: 31/03/2022
 * Title: Control de acceso ESP32 + LCD + RFID
 * Author: Rubén Lozano
 * 
 * Referencias: https://naylampmechatronics.com/blog/22_tutorial-modulo-lector-rfid-rc522.html
 *              
*/

/*
 Pins  SPI     UNO  Mega2560  Leonardo      ESP32       ESP8266
  1     SDA(SS)   10     53        10      GPIO21         D8
  2       SCK     13     52        SCK1    GPIO18 o 14    D5
  3       MOSI    11     51        MOSI1   GPIO23 o 13    D7
  4       MISO    12     50        MISO1   GPIO19 o 12    D6
  5       IRQ      *      *         *        *            *
  6       GND     GND     GND       GND    GND            GND
  7       RST      5      ?         Reset  GPIO22 O #     D3
  8      +3.3V   3V3     3V3       3.3V    3V3           3.3
        * No necesario
*/

/*
 *****************************************************
 *   LIBRERIAS
 *****************************************************
*/
#include <LiquidCrystal.h>  //Libreria encargada de la comunicación con la Pantalla
#include <SPI.h>
#include <MFRC522.h>
#include "funciones.h"
/*
 *****************************************************
 *   VARIABLES Y OBJETOS (GLOBALES)
 *****************************************************
*/
const int RS=5,EN=17,DB4=16,DB5=4,DB6=2,DB7=15;// LCD pins;
LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7); //INICIALIZACION LCD 16x2
const int SDA_PIN=21,RST_PIN=22;   //GPIO21 para el SS (SDA) del RC522 
                                   //GPIO22 para el reset del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522
byte ActualUID[4]; //almacenará el código del Tag leído


byte Usuario1[4]= {0xCB, 0x12, 0xA4, 0x21} ; //código del usuario 1
byte Usuario2[4]= {0x04, 0x5E, 0x4C, 0x63} ; //código del usuario 2

/*
 *****************************************************
 *   SETUP
 *****************************************************
*/
void setup() {
  lcd.begin(16, 2); // Configurar el número de columnas y filas de la pantalla LCD:
  Serial.begin(115200); //Iniciamos La comunicacion serial
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el módulo MFRC522
  Serial.println("Control de acceso:");
  lcd.clear(); //Limpiamos la pantalla LCD
}

/*
 *****************************************************
 *    LOOP
 *****************************************************
*/
void loop() {
    
 for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    lcd.scrollDisplayLeft(); //Desplaza el texto una posición a la izquierda
    delay(300); // Esperamos un bit:
    }
  // Revisamos si hay nuevas tarjetas  presentes
  if ( mfrc522.PICC_IsNewCardPresent()) //Esta función nos devuelve verdadero o falso dependiendo si hay una tarjeta presente cerca al módulo RC522.
        {  
      //Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()) //Se llama a esta función cuando queremos comunicarnos con una tarjeta, nos devuelve un valor verdadero si logra seleccionar una tarjeta para la lectura, de lo contrario nos retorna un valor falso.
            {
                  // Enviamos serialemente su UID
                  Serial.print(F("Card UID:"));
                  //mfrc522.uid.size Nos retorna el tamaño en Bytes del código de identificación de la tarjeta seleccionada.
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(mfrc522.uid.uidByte[i], HEX);   
                          ActualUID[i]=mfrc522.uid.uidByte[i];          
                  }
                  lcd.clear(); //Limpiamos la pantalla LCD
                  lcd.print("Coloque la tarjeta cerca del lector");
                  Serial.print("     ");                 
                  //comparamos los UID para determinar si es uno de nuestros usuarios  
                  if(compareArray(ActualUID,Usuario1)){
                    Serial.println("Acceso concedido...");
                       lcd.clear();
                       lcd.setCursor(0, 1);
                       lcd.print("Acceso concedido...");
                  }else{ if(compareArray(ActualUID,Usuario2)){
                    Serial.println("Acceso concedido...");
                       lcd.clear();
                       lcd.setCursor(0, 1);
                       lcd.print("Acceso concedido...");
                  }else{
                    Serial.println("Acceso denegado...");
                       lcd.clear();
                       lcd.setCursor(0, 1);
                       lcd.print("Acceso denegado...");
                  // Terminamos la lectura de la tarjeta tarjeta  actual
                  mfrc522.PICC_HaltA();
          
            }           
          }
        }
      }
      
  }
  
}
