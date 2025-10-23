#include <IRremote.h>
int pinIR = 7;
int rojo=3;
int verde=5;
int azul=6;
long r=0,v=0,a=0;
IRrecv irrecv(pinIR);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(rojo, OUTPUT); 
  pinMode(verde, OUTPUT); 
  pinMode(azul, OUTPUT); 
}

void loop() { 
  if(irrecv.decode(&results)){
    long code=results.value;    
    Serial.println(results.value);  
    
      
    if(code==16743045&&a==0){
      while (a < 95){
        a++;
        analogWrite(azul,a);
        delay (100);

         
    }
    if(code==16738455){
      a=0;
      analogWrite(azul,a);
    }
  
    

    
    
   
    delay(1000); 
    irrecv.resume();
    Serial.print("Rojo: ");
    Serial.print(r);
    Serial.print(" Verde: ");
    Serial.print(v);
    Serial.print(" Azul: ");
    Serial.println(a);    
   }
}
}
