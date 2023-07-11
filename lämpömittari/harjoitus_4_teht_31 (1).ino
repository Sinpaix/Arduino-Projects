// alustetaan pinnit
#define led_pin 12
#define button_pin 3
#define button_pin3 2
#define sensor_pin A0
#define button_pin2 4
// määritellään eeprom
#include <EEPROM.h>

// määritellään timerin globaalit muuttujat
unsigned long startMillis;  // ajanoton aloitus millisekunteina
unsigned long currentMillis; // tämän hetkinen aika millisekunteina
unsigned long period = 5000;  // haluttu periodi millisekunteina

// määritellään tarvittavat muut globaalit muuttujat
int muistipaikka = 0;


// tallennetaan muuttujaan onko nappi vapautettu
volatile byte buttonReleased = false;
// funktio jota kutsutaan kun tyhjennä muisti nappi vapautetaan
void buttonReleasedInterrupt() {
  Serial.print("Tyhjennet");
  Serial.print((char)228);
  Serial.print((char)228);
  Serial.print("n EEPROM...\n");
  // loopissa käydään koko muisti läpi ja tallennetaan nollia
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  
  Serial.print("EEPROM on tyhj");
  Serial.print((char)228); 
  Serial.print(".\n");
 
  muistipaikka = 0;

  buttonReleased = true;
  
}
// funktio jota kutsutaan kun tulosta nappi vapautetaan
void tulosta() {
  // käydään loopissa koko muisti läpi
  for (muistipaikka = 1 ; muistipaikka < EEPROM.length() ; muistipaikka++) {
	
    
    // luetaan luku muistipaikasta
    int mittaushetki = EEPROM.read(muistipaikka);
    
    // jos luku on eri suuri kuin nolla (eli tallennettu tietoa)
    if (mittaushetki != 0)
    {
     
      muistipaikka++;
      int mittaustulos = EEPROM.read(muistipaikka);
      // muutetaan mittaustulos floatiksi luku muuttujaan
      float luku = mittaustulos * 1.0;
  
      // muunnetaan lämpötilaksi
      //LT = 0.7 * luku - 77.4;
      float LT = ((5 * luku/ 1024.0) - 0.5) * 100;
      
      // tulostetaan aika, mittaustulos ja LT sarjaporttiin
       Serial.print("Aika: ");
       Serial.print(mittaushetki);
       Serial.print(" \t ADC: ");
       Serial.print(mittaustulos);
       Serial.print(" \t LT: ");
       Serial.print(LT);
       Serial.print(".\n");
   
    }
    
  }
  

  buttonReleased = true;
}

void mittaus() {
  
   
  // tallennetaan mittaushetki muuttujaan aika sekunteina
  	unsigned long aika = currentMillis / 1000;
  // mitataan lämpötila
    int reading = analogRead(sensor_pin);
  // luetaan muistista edellisen mittauksen tulos
  	int edellinen = EEPROM.read(muistipaikka);
  // jos mittaustulos on eri suuri kuin edellinen mittaus..
  if (reading != edellinen)
  {
    muistipaikka++;
    // tallennetaan aika ja mittaustulos muistiin
    EEPROM.write(muistipaikka, aika);
    muistipaikka++;
    EEPROM.write(muistipaikka, reading);
     Serial.print("Mittaus tallennettu. \n"); // testi tulostus
  }
}


void setup()
{
  // käynnistetään sarjaportti tulostukselle
  Serial.begin(9600);
  //pinMode(led_pin, OUTPUT); // ledin pin moodiksi output
  pinMode(button_pin2, INPUT_PULLUP); //napin moodiksi input 
  pinMode(button_pin3, INPUT_PULLUP); //napin moodiksi input 
  pinMode(button_pin, INPUT_PULLUP); //napin moodiksi input 
  startMillis = millis();  // tallennetaan käynnistysaika
  
  // keskeytysrutiini joka kutsuu tyhjennä muisti funktiota
  // falling mode koska napin tila muuttuu HIGH -> LOW
  attachInterrupt(digitalPinToInterrupt(button_pin3),
                  buttonReleasedInterrupt,
                  FALLING);
  // keskeytysrutiini joka kutsuu tulosta funktiota
  // falling mode koska napin tila muuttuu HIGH -> LOW
  attachInterrupt(digitalPinToInterrupt(button_pin),
                  tulosta,
                  FALLING);

}

void loop()
{
  currentMillis = millis();  // luetaan tämän hetkinen aika
  
  
  // jos haluttu periodi on kulunut, kutsutaan mittaus funktiota
  if (currentMillis - startMillis >= period)  
  {
    mittaus();
    
    // tallennetaan nykyhetki seuraavan ajanoton aluksi
    startMillis = currentMillis;  
  }
    
    // jos muistipaikat täyttyvät, nollataan laskuri
    // uudet arvot ylikirjoittavat vanhojen päälle
    if (muistipaikka == EEPROM.length()) {

    muistipaikka = 0;

  }
   
}