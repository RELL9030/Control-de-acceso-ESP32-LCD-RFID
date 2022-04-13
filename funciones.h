/*
 *****************************************************
 *   VARIABLES Y OBJETOS (GLOBALES)
 *****************************************************
*/
const int RS=5,EN=17,DB4=16,DB5=4,DB6=2,DB7=15;// LCD pins;
LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7); //INICIALIZACION LCD 16x2

// GPIO donde LED esta conectado 
//RGB Ánodo comun(+): 32,33,25 Cátodo comun(-): 25,33,32
//BICOLOR: RED:25 BLUE:33 
const int ledPinR =  25;
const int ledPinG =  32;
const int ledPinB =  33;
/*
 *****************************************************
 *    FUNCTIONS
 *****************************************************
*/
//Función para cambiar estado de led
void setColor(int R, int G, int B) {
  digitalWrite(ledPinR, R);
  digitalWrite(ledPinG, G);
  digitalWrite(ledPinB, B);
}

//Función para comparar dos vectores
 boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}

//Función para mostrar mensaje en la lcd
void Lcd_mensaje(String d,int Col,int Fil,int r, int g, int b)
{
   lcd.clear(); //Limpiamos la pantalla LCD
   lcd.setCursor(Col, Fil);
   lcd.print(d);
   setColor(r,g,b);
}
