#include <SoftwareServo.h> 
 
SoftwareServo myservo;  // create servo object to control a servo 
 
int aperto = 0;   //variabile per switch case per aprire o chiudere i cassetti
int sn = 0;    //porte sono già aperte o chiuse? 0 = chiuse / 1 = perla / 2 = tigre
int motore = 90;  //giro del servo che all'inizio viene impostato a neutro
const int limite = 600;
int fsrPin = 0;     // the FSR and 10K pulldown are connected to a0
int peso;     // the analog reading from the FSR resistor divider
int pos = 0;
int posizione = 0;

void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  Serial.begin(9600);
} 
 
void loop() 
{ 
 peso = analogRead(fsrPin);

//scelta della gatta in base al peso
  if (peso <= 100) {
    aperto = 0;    //nessuna gatta sulla bilancia
    Serial.println(aperto);
  } else if (peso <= limite) {
    aperto = 1;    //perla sulla bilancia
    Serial.println(aperto); 
  } else {
    aperto = 2;    //tigre sulla bilancia
    Serial.println(aperto);
  }
  
//in base alla gatta apro o chiudo o non faccio nulla se già aperto
  switch (aperto) {
  case 0:    //nessuna gatta sulla bilancia
    SoftwareServo::refresh();
    posizione = myservo.read();
    Serial.println(posizione);

    if (sn > 0) {              //se una gatta è appena scesa
      if (posizione > 90) {    //in particolare la tigre è scesa
        for (pos = posizione; pos = 90; pos--) {    //faccio tornare indietro il servo dalla posizione in cui era
          myservo.write(pos);   
          delay(15);
          SoftwareServo::refresh();
        }
        sn = 0;    //poi imposto la gabbia sul chiuso
      } else {    //se invece è la perla ad essere scesa
        for (pos = posizione; pos = 90; pos++) {  //faccio tornare indietro il servo
          myservo.write(pos);
          delay(15);
          SoftwareServo::refresh();
        }
        sn = 0;          //e identifico che è chiuso
      }
      sn = 0;
    }
    break;  
  
  case 1:    //parte perla
    SoftwareServo::refresh();
    posizione = myservo.read();
    Serial.println(posizione);

    if (sn < 1 || sn > 1) {  //se era chiuso oppure la tigre è appena scesa
      SoftwareServo::refresh();
      posizione = myservo.read();    
      for (pos = posizione; pos = 135; pos++) {
        myservo.write(pos);   // se è chiuso o aperta per la tigre, apro la perla
        delay(15);
        SoftwareServo::refresh();
      } 
      sn = 1;          //e identifico che è già aperta per lei
    }
    break;  
  
  case 2:    //parte tigre
    SoftwareServo::refresh();
    posizione = myservo.read();
    Serial.println(posizione);

    if (sn < 2) {
      SoftwareServo::refresh();
      posizione = myservo.read();
      for (pos = posizione; pos = 45; pos--) {
        myservo.write(pos);   // se una gabbia è chiusa o aperta per la perla, apro la tigre
        delay(15);
        SoftwareServo::refresh();
      }
      sn = 2;    //e identifico che è già aperta per lei
    }
    break;  
  } 
  
  delay(1000);
}
