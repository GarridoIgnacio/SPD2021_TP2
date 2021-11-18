#include <LiquidCrystal.h>
#include <stdlib.h>
#include <ctype.h>

#define BotonUp 8
#define BotonDow 10
#define BotonSelect 9
#define BotonRandom 13
#define LedCorrecto 12 
#define LedError 11
#define TAM 4

LiquidCrystal pantallaLcd(2,3,4,5,6,7);

int indice;
char contador = 65;
char letraRecibida;
char VectorStrings[TAM][10] = {"DELOREAN","DUROTAN","ZERATUL","DOCBROWN"};
char palabra[10];         
char palabraOcultada[10];
char letra[10];
int botonUp(int);
int botonDown(int);
int botonSelect(void);
int botonRandom(void);
int OcultarLetras (char* palabra, char* palabraOcultada, int tam);
void LedsConfirmacion (int caso);
int VidasRestantes (int caso);
int LetraSeleccionada(char contador, char* palabra, char* palabraOcultada);
void WinOrLose(int win, int clear);
void(* resetSoftware)(void) = 0;
int CheckWord(char* palabraOcultada);
void GenerarRandom(void);


void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
  //PANTALLA
  pantallaLcd.begin (16, 2);
  //LEDS
  pinMode(LedCorrecto, OUTPUT);
  pinMode(LedError, OUTPUT);
  //BOTONES
  pinMode(BotonUp, INPUT);
  pinMode(BotonDow, INPUT);
  pinMode(BotonSelect, INPUT);
  pinMode(BotonRandom, INPUT);
  
}

int previoState = 0;
int upPrevState = 0;
int downPrevState = 0;
int selectPrevState = 0;
int randomPrevState = 0;
int aux = 0;
int caso = 1;
int vidas = 3;
int clear = 0;
unsigned long millisAntes = 0;
int flagOn = 0;
int flagPalabra=0;
int win;
int flagFinal=1;

void loop()
{
  if(flagFinal==1){
  GenerarRandom();
  
   contador = botonUp(contador);
   contador = botonDown(contador);
   pantallaLcd.setCursor(1, 0);
   pantallaLcd.print(contador);


   OcultarLetras(palabra, palabraOcultada, 10);
   pantallaLcd.setCursor(4, 1);
   pantallaLcd.print(palabraOcultada);


   caso = LetraSeleccionada(contador, palabra, palabraOcultada);
   LedsConfirmacion(caso);
   clear = VidasRestantes(caso);
   win = CheckWord(palabraOcultada);
  }
   if(win == 1 || clear == 1)
   {
     
     WinOrLose(win, clear);
   }
  delay(50);   	
}
/**
 * @brief Detecta si se apretó el boton y habilita la funcion random
 * @return rtn devuelve un 1 en caso de apretar el boton
 */
int botonRandom(void)
{ 
  int rtn = 0;
  int randomState = digitalRead(BotonRandom);
  
   if(randomState == 1 && randomPrevState == 0)
   {
    rtn = 1;				                
   }
  randomPrevState = randomState;
  return rtn;
}


/**
 * @brief Si se inicia el programa o se apreta el boton random se genera un nuermo de 0 a 4 que sera usado por el for, 
 * para buscar el indice de la matriz para determina la palabra a adivinar se copia en en la variable global char palabra.
 */
void GenerarRandom()
{  
  
  
  if(botonRandom()==1 || flagOn == 0)
  {
    indice = random(0, 4);
    pantallaLcd.clear();
    //Serial.println(indice);
    aux = 0;
    vidas=3;
    flagOn = 1;
    for(int i = 0; i < TAM ; i++)
    {
    if(i == indice)
  	{
    	strncpy(palabra, VectorStrings[i], sizeof(palabra));
      
  	}

  }

}
}
// 
/**
 * @brief Avanza de forma ascendente las opciones de letras del abecedario mostradas en el lcd.
 * @param botonUp Variable de tipo int  que modifica de manera ascendente la letra actual.
 * @param upState Variable de tipo int que  indica el estado de  la letra modificada.
 * @return contador Variable global que indica las letras del abecedario en un rango del 65 al 90 que son las letras del abecedario en el codigo ascii.
 */
int botonUp(int)
{
  int upState = digitalRead(BotonUp);
  
  if(upState == 1 && upPrevState == 0)
  {
    if(contador >= 65 && contador <= 90)
    {
      contador++;
      if(contador==91)
      {
          contador=65;
      }
  	}
  }
	upPrevState = upState;
	return contador;
}

// bajar
/**
 * @brief Avanza de forma descendente las opciones de letras del abecedario mostradas en el Lcd.
 * @param botonDown Variable de tipo int que modifica de manera descendente la letra actual. 
 * @param downState Variable de tipo int que indica el estado actual de la letra modificada.
 * @return Contador variable global que indica las letras del abecedario en un rango del 65 al 90 que son las letras del abecedario en el codigo ascii.
 */
int botonDown(int)
{
  int downState = digitalRead(BotonDow);
  
  if(downState == 1  && downPrevState == 0)
  {
    if(contador <= 90 && contador >= 65 )
    {
      contador--;
      if(contador == 64)
      {
        contador = 90;
      }
    }
  }
  downPrevState = downState;
  return contador;
}

/**
 * @brief Oculta los caracteras del array,
 * Recorre dicho array con un for y remplaza con un '_'.
 * @param palabra Recibe como parametro la palabra obtenida del Random.
 * @param palabraOcultada Puntero que recibira los '_'.
 * @param tam Longitud del array.
 * @return int 1 si se pudo ocultar la palabra, de lo contrario retornara 0.
 */
int OcultarLetras (char* palabra, char* palabraOcultada, int tam)
{   
  char palabraAux[10];
  int retorno;

  retorno = 0;
  
  if(palabra != NULL && tam > 0 && aux == 0)
  {
    strncpy(palabraAux, palabra, 10);

    for (int i = 0; i < 10; i++)
    {
      if(i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || (i == 7 && palabraAux[i] != '\0'))
      {
        palabraAux[i] = '_';
      }
    }
    strncpy(palabraOcultada, palabraAux, 10);
    retorno = 1;
  }    
    return retorno;
}

/**
 * @brief Selecciona la letra actual para probar en los espacios de la palabra buscada.
 * @param botonSelect Variable global de tipo int que define si la opcion seleccionada corresponde a algun espacio de la palabra buscada.
 * @return rtn escribe la letra en el espacio correspondiente en caso de ser acertada.
 */
int botonSelect(void)
{ 
  int rtn = 0;
  int selectedState = digitalRead(BotonSelect);
  
   if(selectedState == 1 && selectPrevState == 0)
   {
    rtn = 1;				                
   }
  selectPrevState = selectedState;
  return rtn;
}

/**
 * @brief Enciende el led verde si se ha ingresado una letra correspondiente a la palabra oculta,
 * de lo contrario si no es correcta encendera el led rojo.
 * @param caso Recibe el caso que define si se ingreso correctamente la letra.
 */

void LedsConfirmacion (int caso)
{ 
  unsigned long millisAhora = millis();

  switch (caso)
  {
    case 1:
      if(millisAhora - millisAntes > 600)
      {
        millisAntes = millisAhora;
        digitalWrite(LedCorrecto, HIGH);
      }
      digitalWrite(LedCorrecto, LOW);  
      break;

    case 2:
      if(millisAhora - millisAntes>600)
      {
        millisAntes = millisAhora;
        digitalWrite(LedError, HIGH); 
      }
      digitalWrite(LedError, LOW);
      break;
  }
}
/**
 * @brief Recibe por parametro caso == 2 el caracter no coincide con el existente en el array,
 *  Y resta la variables vias hasta llegar a 0.
 * @param caso Variable global del tipo int que define si hubo coincidencia o no.
 * @return int CeroVidas cuando llega el contador vidas a 0 retoran un 1 sino un 0.
 */
int VidasRestantes (int caso)
{
  int ceroVidas = 0;

  if(caso == 2){
    vidas--;
    //Serial.println(vidas);
  }

  pantallaLcd.setCursor(10, 0);
  pantallaLcd.print("LF:");
  
  switch (vidas)
  {
    case 1:
      pantallaLcd.setCursor(14, 0);
      pantallaLcd.print(" ");
    break;

    case 2:
      pantallaLcd.setCursor(15, 0);
      pantallaLcd.print(" ");
    break;

    case 3:
      pantallaLcd.setCursor(13, 0);
      pantallaLcd.print("***");
    break;

    case 0:
      ceroVidas = 1;
    break;
  }
  return ceroVidas;  
}
/**
 * @brief Recibe por paremetros la letra (contador), la palabra a adivinar(palabra) y la palabra ocultada (palabraOcultada), 
 * con un for se busca que la letra coincida con una de la palabra.
 * @param contador Es la letra que debe buscar coincidencia.
 * @param palabra El array que recorre el ford.
 * @param palabraOcultada Donde se va a guardar la letra en el indice de la palabra ocultada.
 * @return int Retorana 1 si hubo coinsidencia o 2 si no.
 */
int LetraSeleccionada (char contador, char* palabra, char* palabraOcultada)
{
  char palabraAux[10];
  int i;
  int caseLedsCorrecto;
  int flag;
  strncpy(palabraAux, palabra, 10);
  
  if(botonSelect() == 1)
  {  
    flag = 0;
    for( i = 0; i < 10; i++)
    {
      if(palabraAux[i] == contador)
      {
        palabraOcultada[i] = contador;
        aux = 1;
        caseLedsCorrecto = 1; 
        flag = 1;
      }
      else
      {
        if(flag == 0)
        {
          caseLedsCorrecto = 2; 
        }
      }                           
    }
  }
 
  return caseLedsCorrecto;  
}

/**
 * @brief Recibe la palabra ocultada por parametro (palabraOcultada) y con la funcion isalpha,
 * chequea que todos los caracteres sean letras y no signos '_'. 
 * @param palabraOcultada La palabra ocultada con los signos.
 * @return int Retorna 1 si todo los caracteres son letras 0 si no lo son.
 */
int CheckWord(char* palabraOcultada)
{
  
  int rtn = 0;
  char palabraAux[10];
  int contador = 0;
  strncpy(palabraAux, palabraOcultada, 10);

  for(int i = 0; i < 10; i++)
  {
    if(isalpha(palabraAux[i]) != 0)
    {
      contador++;
    }
  }
  if(contador == strlen(palabraAux))
  {
    rtn = 1;
  }
  return rtn;
}

/**
 * @brief Recibe por parametro win y clear, determina si gano o perdio e imprime el mensaje correspondiente,
 *  luego resetea el programa.
 * @param win Es el retorno de CheckWord(). 
 * @param clear Es el retorno de VidasRestantes().
 */
void WinOrLose(int win, int clear )
{
  unsigned long millisAhora = millis();

  if(clear == 1)
  { 
    if(flagFinal==1)
    {
      flagFinal=0;
      pantallaLcd.clear();
    }
    pantallaLcd.setCursor(0, 0);
    pantallaLcd.print("LOSER");
      
    if(millisAhora - millisAntes > 1000)
    {
      millisAntes = millisAhora;
      resetSoftware();
    }
  }
  else if(win == 1 )
  {
    if(flagFinal==1)
    {
      flagFinal=0;
      pantallaLcd.clear();
    }
    pantallaLcd.setCursor(0, 0);
    pantallaLcd.print("WINNER");
    if(millisAhora - millisAntes > 1000)
    {
      millisAntes = millisAhora;
      resetSoftware();
    }
  }
}