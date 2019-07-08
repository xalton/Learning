int ledj = 2; // led jaune 
int ledr = 3; // led rouge
int ledv = 4; // led verte 
int ledb = 5; // led bleu
int potentio = 0; // potentiomètre
int valeur = 0; //valeur potentiomètre
int bouton = 6; //buton
int etat = 0;
int valeurb = 0;

void setup(){
  pinMode (ledj,OUTPUT);
  pinMode (ledr,OUTPUT);
  pinMode (ledv,OUTPUT);
  pinMode (ledb,OUTPUT);
  pinMode (bouton,INPUT); 
  Serial.begin(9600); 
}

void loop (){
  valeurb = digitalRead(bouton);
  delay(250);
  Serial.print("valeur état: ");
  Serial.println(etat);
  Serial.print("valeur du bouton: ");
  Serial.println(valeurb);

  if (valeurb == HIGH && etat == 0){
    etat = 1;
    digitalWrite(ledr, LOW); 
    digitalWrite(ledj, LOW);
    digitalWrite(ledv, LOW);
    digitalWrite(ledb, LOW);
  } 
  
  
  
  if (valeurb == HIGH && etat == 1){
    etat = 0;
    digitalWrite(ledr, LOW); 
    digitalWrite(ledj, LOW);
    digitalWrite(ledv, LOW);
    digitalWrite(ledb, LOW);
  } 
  
  /*if(etat == 0){
    valeur = analogRead(potentio);
    bouton =  digitalRead(6);
    Serial.println(valeur);
    delay(100);
    if (valeur >= 250) {
      digitalWrite(ledj, HIGH);
    } 
    if (valeur >= 500) {
      digitalWrite(ledr, HIGH);
      digitalWrite(ledj, HIGH);
    }
    if (valeur >= 750) {
      digitalWrite(ledr, HIGH); 
      digitalWrite(ledj, HIGH);
      digitalWrite(ledv, HIGH);
    }
    if (valeur >= 1000) {
      digitalWrite(ledr, HIGH); 
      digitalWrite(ledj, HIGH);
      digitalWrite(ledv, HIGH);
      digitalWrite(ledb, HIGH);
    }
  }*/
}
















