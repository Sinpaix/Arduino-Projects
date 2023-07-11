// määritellään pinnit
int PIRsensor=2;
int buzzer=10;
int ledpin=13;
const int pinOFswitch = 3; 

// määritellään tarvittavat globaalit muuttujat
int thief=0;
int ValueOFswitch; 

void setup()
{
  pinMode(PIRsensor, INPUT); // pir sensorin moodiksi input
  pinMode(buzzer, OUTPUT); // kaiuttimen moodiksi output
  pinMode(ledpin, OUTPUT); // ledin moodiksi output
  pinMode(pinOFswitch, INPUT); // liukukytkimen moodiksi input

}

void loop()
{
  // luetaan liukukytkimen tila ON vai OFF
  ValueOFswitch = digitalRead(pinOFswitch); 
  
  // jos tila ON...
  if (ValueOFswitch == HIGH)
  {
    
  int val = analogRead(A1); // luetaan potentiometrin arvo
  thief = digitalRead(PIRsensor); // luetaan pirsensorin tila
  
  // jos pirsensori hälyttää liikkeestä..
  if(thief ==1){
    
    int threshold = 512; // asetetaan kaiuttimelle raja-arvo
	// jos potentiometsin arvo suurempi kuin raja-arvo..
	if (val > threshold) {
    // .. muutetaan kaiuttimen äänenkorkeutta
  	tone(10, map(val, 0, 1023, 2000, 4000)); 
      
    digitalWrite(buzzer,HIGH); // kaiutin päälle
    digitalWrite(ledpin, HIGH); // sytytetään led
    delay(1000);
    digitalWrite(buzzer,LOW); // sammutetaan kaiutin
    digitalWrite(ledpin, LOW); // sammutetaan led
    // nollataan arvot
    noTone(10); 
    val = 0;
    delay(1000);
	}
	else {
  	noTone(10);
    digitalWrite(buzzer,LOW);
    digitalWrite(ledpin, LOW);
    }
    
  	
  }
    
  }
  
  
}