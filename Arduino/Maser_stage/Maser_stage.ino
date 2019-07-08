#include <SPI.h>
#include <Ethernet.h>
byte mac[] = {0x90, 0xA2, 0xDA, 0x11, 0x36, 0xE4}; //adresse mac qu'on a définit pour la carte
IPAddress ip(172.26.4.171); //adrèsse ip de la carte
//IPAddress gateway( 192, 168, 2, 1 );
//IPAddress subnet( 255, 255, 255, 0 );

//domaine du serveur
char server[] = "smtp.oma.be";

EthernetClient client;
EthernetServer server(25);

//On déclare nos variable, attention ici les input et output sont à considérer par rapport au schéma
int rel = 6; //relai orange-AC/DC-INPUT 
int relOUT = 7 //relai orange-AC/DC-OUTPUT
int rel2 = 4; //relai vert-DIODE-INPUTA
int rel2OUT = 5 //relai vert-DIODE-OUTPUTA 
int rel3 =  2; //relai jaune-DIODE-INPUTB
int rel3OUT =  3; //relai jaune-DIODE-OUTPUTB
int messsage = 0; //nombre de méssage
int bouton = 8; //bouton poussoir
int etatbouton = 0; //état du bouton
int e = 0; //numéro de référence de l'erreur

void setup(){
  //ici les input et output sont à considérer par rapport à l'Arduino
  pinMode (rel,OUTPUT);
  pinMode (relOUT,INPUT);
  pinMode (rel2,OUTPUT);
  pinMode (rel2OUT,INPUT);  
  pinMode (rel3,OUTPUT); 
  pinMode (rel3OUT,INPUT);
  pinMode (bouton,INPUT);
  Ethernet.begin(mac,ip);
  server.begin();
  Serial.begin(9600);
  //Serial.print("Server address:");
  //Serial.println(Ethernet.localIP());
}
void loop(){
  //on lit l'état du bouton
  etatbouton = digitalRead(bouton);
  //Serial.print("état du bouton":);
  //Serial.println(etatbouton);
  
  //on envoit un signal aux relais pour tester leur état
  etatin = digitalWrite(rel,HIGH); // signal pour le relai orange (en entrée)
  etat2in = digitalWrite(rel2,HIGH);// signal pour le relai vert (en entrée)
  etat3in = digitalWrite(rel3,LOW);// signal pour le relai jaune (en entrée)  

  //On enregistre l'état du relai après le signal 
  etatout = digitalRead(relOUT);// relai orange (en sortie)
  etat2out = digitalRead(rel2OUT);// relai vert (en sortie)
  etat3out = digitalRead(rel3OUT);//  relai jaune (en sortie)  
  //Serial.print("état initial du relai 1 orange":);
  //Serial.println(etat1out);
  //Serial.print("état initial du relai 2 vert":);
  //Serial.println(etat21out);
  //Serial.print("état initial du relai 3 jaune":);
  //Serial.println(etat31out);
  
  //On test nos relais pour detecter s'il y a une panne d'alimentation et on envoit le mail/sms dans ce cas
  while (message <= 1){
    //cas 1: relai 1 activé
    if (etatin == etatout){
      //relai 2&3 activé
      if (etat2in == etat2out && etat3in == etat3out){
        //pas d'érreur
        message = 0;
      }
      //relai 2 désactivé & 3 activé
      if (etat2in != etat2out && etat3in == etat3out){
        //érreur 1: relai 2 défaillant
        e = 1;
        if(sendEmail()) Serial.println(F("Email sent"));
      else Serial.println(F("Email failed"))
        message = message + 1;
      }
      //relai 2&3 désactivé
      if (etat2in != etat2out && etat3in != etat3out){
        //érreur 2: relai 1 défaillant et pas d'alim pour la Diode
        e = 2; 
        if(sendEmail2()) Serial.println(F("Email sent"));
      else Serial.println(F("Email failed"))
        message = message + 1;
      }
      else{
      //relai 2  activé & 3 désactivé  
        //érreur 3: relai 3 défaillant
        e = 3;
        if(sendEmail()) Serial.println(F("Email sent"));
      else Serial.println(F("Email failed"))
        message = message + 1;
      }
    }
    //cas2: relai 1 désactivé
    if (etatin != etatout){ 
      // relai 2&3 activé
      if (etat2in == etat2out && etat3in == etat3out){
        //érreur 4: relai 2 et 3 defaillant sachant relai AC/DC defaillant
        e = 4;
        if(sendEmail()) Serial.println(F("Email sent"));
        else Serial.println(F("Email failed"))
        message = message + 1;
      }
      // relai 2 activé & 3 désactivé
      if (etat2in == etat2out && etat3in != etat3out){
        //érreur 5: relai 3 défaillant + Diode pas alimentée sachant relai AC/DC defaillant
        e = 5;
        if(sendEmail()) Serial.println(F("Email sent"));
        else Serial.println(F("Email failed"))
        message = message + 1;
      }
      // relai 2 & 3 désactivé
      if (etat2in != etat2out && etat3in != etat3out){
        //érreur 6: relai 1&2&3 défaillant et pas d'alim pour la Diodde sachant relai AC/DC defaillant
        e = 6;
        if(sendEmail()) Serial.println(F("Email sent"));
        else Serial.println(F("Email failed"))
        message = message + 1;
      }
      else{
      // relai 2 désactivé & relai 3 activé  
        //érreur 7: relai 1 défaillant donc pas d'alim venant de l'AC/DC 
        e= 7;
        if(sendEmail()) Serial.println(F("Email sent"));
        else Serial.println(F("Email failed"))
        message = message + 1;
      }
    }
  } 
 // Test de l'état du bouton qui sert de reset en remettant la valeur de message à 0
 Serial.print("état du bouton":); 
 Serial.println(etatbouton); 
 while(etatbouton == HIGH && message >= 1){
   e=0;
   if(sendEmail()) Serial.println(F("Email sent"));
   else Serial.println(F("Email failed"))
   message = 0;
  

   delay(1000);   
 }
 delay(600000);
}    




//définition des fonctions utiles à l'envoit d'un email:
/*

byte sendEmail(e)
{
  byte thisByte = 0;
  byte respCode;
 
  if(client.connect(server,25) == 1) {
    Serial.println(F("connected"));
  } else {
    Serial.println(F("connection failed"));
    return 0;
  }
 
  if(!eRcv()) return 0;
  Serial.println(F("Sending helo"));
 
// change to your public ip
  client.println(F("helo 1.2.3.4"));
 
  if(!eRcv()) return 0;
  Serial.println(F("Sending From"));
 
// change to your email address (sender)
  client.println(F("MAIL From: <masterArduino@${ptf-ard1}.oma.be >"));
 
  if(!eRcv()) return 0;
 
// Déstinataire
   Serial.println(F("Sending To"));
   client.println(F("RCPT To: <time_lab@oma.be >"));
   client.println(F("RCPT To: < smsmail@oma.be >"));
 
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending DATA"));
  client.println(F("DATA"));
 
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending email"));
 
// Déstinataire
   client.println(F("RCPT To: <time_lab@oma.be >"));
   client.println(F("RCPT To: < smsmail@oma.be >"));
 
// l'envoit du mail suivant la référence de l'érreur
  if(e==0){
    client.println(F("From: Me <masterArduino@${ptf-ard1}.oma.be>"));
 
    client.println(F("Subject: Plus d'erreurs \r\n"));
 
    client.println(F("Le problème d'alimentation a été résolu. "));
 
    client.println(F(".")); 
  }
  if(e==1){
    client.println(F("From: Me <masterArduino@${ptf-ard1}.oma.be>"));
 
    client.println(F("Subject: Panne relai 2 \r\n"));
 
    client.println(F("Le relai 1 est activé, mais il y a un problème avec le relai 2"));
 
    client.println(F("."));
  }
  if(e==2){
    client.println(F("From: Me <masterArduino@${ptf-ard1}.oma.be>"));
 
    client.println(F("Subject: Panne relai 1 \r\n"));
 
    client.println(F("Le relai 1 est défaillant et il n'y a pas d'alimentation pour la Diode"));
 
    client.println(F(".")); 
  }
  
  if(e==3){
    client.println(F("From: Me <masterArduino@${ptf-ard1}.oma.be>"));
 
    client.println(F("Subject: Panne relai 3 \r\n"));
 
    client.println(F("Le relai 3 est défaillant "));
 
    client.println(F(".")); 
  }
  
  if(e==4){
    client.println(F("From: Me <masterArduino@${ptf-ard1}.oma.be>"));
 
    client.println(F("Subject: Relai 2 et 3 defaillant \r\n"));
 
    client.println(F("Les relais 2 et 3 sont defaillant sachant que le relai 1 est désactivé "));
 
    client.println(F(".")); 
  }
  
  if(e==5){
    client.println(F("From: Me <masterArduino@${ptf-ard1}.oma.be>"));
 
    client.println(F("Subject: relai 3 défaillant \r\n"));
 
    client.println(F("Le relai 3 défaillant + Diode pas alimentée sachant relai 1 désactivé "));
 
    client.println(F(".")); 
  }
  
  if(e==6){
    client.println(F("From: Me <masterArduino@${ptf-ard1}.oma.be>"));
 
    client.println(F("Subject: relai 3 défaillant et pas d'alim pour la Diodde \r\n"));
 
    client.println(F("Le relai 1&2&3 défaillant et pas d'alim pour la Diodde sachant relai AC/DC désactivé"));
 
    client.println(F(".")); 
  }
  
  if(e==7){
    client.println(F("From: Me <masterArduino@${ptf-ard1}.oma.be>"));
 
    client.println(F("Subject: Pas d'alim venant de l'AC/DC relai2 désactivé \r\n"));
 
    client.println(F("Le relai 1&2 défaillant donc pas d'alim venant de l'AC/DC "));
 
    client.println(F(".")); 
  }
 
  
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending QUIT"));
  client.println(F("QUIT"));
 
  if(!eRcv()) return 0;
 
  client.stop();
 
  Serial.println(F("disconnected"));
 
  return 1;
}
 
byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;
 
  while(!client.available()) {
    delay(1);
    loopCount++;
 
    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }
 
  respCode = client.peek();
 
  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }
 
  if(respCode >= '4')
  {
    efail();
    return 0;  
  }
 
  return 1;
}
 
 
void efail()
{
  byte thisByte = 0;
  int loopCount = 0;
 
  client.println(F("QUIT"));
 
  while(!client.available()) {
    delay(1);
    loopCount++;
 
    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return;
    }
  }
 
  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }
 
  client.stop();
 
  Serial.println(F("disconnected"));
*/


