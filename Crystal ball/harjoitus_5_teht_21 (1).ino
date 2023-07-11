// importataan ja määritellään Liquid crystal kirjasto + pinnit
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 7);
#define button_pin 2
#define sensor_pin A0

// määritellään tilt sensori pinniin
const int switchPin = 6;

// muuttuja napin tilan tarkasteluun
volatile byte buttonReleased = false;
// alustetaan tarvittavat globaalit muuttujat
int switchState = 0;
int prevSwitchState = 0;
int reply;

// funktio jota kutsutaan kun nappi vapautetaan
void buttonReleasedInterrupt() {
  
    // mitataan lämpötila
    int reading = analogRead(sensor_pin);
    // muutetaan floatiksi
    float luku = reading * 1.0;
  
    // muunnetaan lämpötilaksi
    //LT = 0.7 * luku - 77.4;
    float LT = ((5 * luku/ 1024.0) - 0.5) * 100;
  
    // tyhjennetään lcd ja printataan
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp is now:");
    lcd.setCursor(0, 1);
    lcd.print(LT);
  
    // päivitetään napin tila
    buttonReleased = true;
  
}

void setup() {
  lcd.begin(16, 7); // käynnistetään lcd
  pinMode(switchPin,INPUT); // määritellään sensori inputiksi
  pinMode(button_pin, INPUT_PULLUP); //napin moodiksi input 
  lcd.print("Ask the"); // printataan näytölle
  lcd.setCursor(0, 1); // siirretään kursori seuraavalle riville
  lcd.print("Crystal Ball!"); // printataan taas
  
  // keskeytysrutiini joka kutsuu funktiota
  // falling mode koska napin tila muuttuu HIGH -> LOW
  attachInterrupt(digitalPinToInterrupt(button_pin),
                  buttonReleasedInterrupt,
                  FALLING);

}


void loop() {
  // luetaan sensorin tila
  switchState = digitalRead(switchPin);
  // jos tila muuttunut...
  if (switchState != prevSwitchState) {
    if (switchState == LOW) {
      // valitaan kahdeksasta vastauksesta randomilla
      // yksi joka printataan näytölle
      reply = random(8);
      // näyttö tyhjennetään ja asetetaan kursori
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("The ball says:");
      lcd.setCursor(0, 1);
      // switch case rakenne määrittää randomilla generoidun
      // numeron mukaan vastauksen ja printtaa sen näytölle
      switch(reply){
        case 0:
        lcd.print("Yes");
        break;
        case 1:
        lcd.print("Most likely");
        break;
        case 2:
        lcd.print("Certainly");
        break;
        case 3:
        lcd.print("Outlook good");
        break;
        case 4:
        lcd.print("Unsure");
        break;
        case 5:
        lcd.print("Ask again");
        break;
        case 6:
        lcd.print("Doubtful");
        break;
        case 7:
        lcd.print("No");
        break;
      }
    }
  }
  // tallennetaan sensorin tila muuttujaan
    prevSwitchState = switchState;
}