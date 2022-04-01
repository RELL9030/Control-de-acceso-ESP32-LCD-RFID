/*
 * Date: 31/03/2022
 * Title: Control de acceso ESP32 + LCD + RFID + BT
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
#include <SPI.h>
#include <MFRC522.h>
#include "funciones.h"
/*
 *****************************************************
 *   VARIABLES Y OBJETOS (GLOBALES)
 *****************************************************
*/
#define RST_PIN  22    //GPIO22 para el reset del RC522
#define SS_PIN  21   //GPIO21 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522

byte ActualUID[4]; //almacenará el código del Tag leído

/* *****************************************************
 * Target 1:   CB 12 A4 21     TAG 1: 07 24 70 34   
 * Target 2:   04 5E 4C 63     TAG 2: E9 BD DE 2A
 ******************************************************/
byte Usuario1[4]= {0x4D, 0x5C, 0x6A, 0x45} ; //código del usuario 1
byte Usuario2[4]= {0xC1, 0x2F, 0xD6, 0x0E} ; //código del usuario 2

/*
 *****************************************************
 *   SETUP
 *****************************************************
*/
void setup() {
  Serial.begin(115200); //Iniciamos La comunicacion serial
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
  Serial.println("Control de acceso:");
}

/*
 *****************************************************
 *    LOOP
 *****************************************************
*/
void loop() {
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
                  Serial.print("     ");                 
                  //comparamos los UID para determinar si es uno de nuestros usuarios  
                  if(compareArray(ActualUID,Usuario1))
                    Serial.println("Acceso concedido...");
                  else if(compareArray(ActualUID,Usuario2))
                    Serial.println("Acceso concedido...");
                  else
                    Serial.println("Acceso denegado...");
                  
                  // Terminamos la lectura de la tarjeta tarjeta  actual
                  mfrc522.PICC_HaltA();
          
            }
      
  }
  
}
