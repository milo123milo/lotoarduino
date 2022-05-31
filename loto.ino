

// #include <LiquidCrystal.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Za komentarisi ako koristis displej sa vezbi
LiquidCrystal_I2C lcd(0x27,16,4);  

// Odkomentarisi liniju ispod za displej za vezbi
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int brojevi[7] = {0}; // Niz za cuvanje brojeva korisnika inicijalizovan na nule
int loto[7] = {0}; // Niza za cuvanje nasumicnih brojeva 
int pogodak = 0; // Broj pogodataka
int i = 0; // Brojac prvi
int j = 0; // Brjac drugi
void setup()
{
  //Inicijalizacija random funkcije za generisanje rnasumicnih brojeva
  randomSeed(analogRead(0)); 

  //Otvaranje Serijskog porta
  Serial.begin(9600);

  //Ja sam koristio drugaciji displej pa je ovo njegova inicijalizacija
  //Ne vazi za onaj sto koristimo na vezbama
  //Za komentarisati ako se koristi dispelj sa vezbi
  lcd.init();                      
  lcd.init();
  lcd.backlight();

  //Odkomentariasti za displej sa vezbi
  //lcd.begin(16,4);
  //lcd.setCursor(0,0);

  //Sve ostalo prema dolje je isto i za LCD na vezbama
  lcd.setCursor(0,0);
  lcd.print("Unesi brojeve");
}

void loop(){
  //Podesavanje Kursora za ispis unosenih brojeva
  lcd.setCursor(0,1);

  //Petlja koja broji 7 puta
  for(i=0; i < 7; i++) {
      //Pozivamo funkciju koja cita po jedan broj za Serijskog porta
      unos();

      //Prikazujemo na LCD unijeti broj
      lcd.print(brojevi[i]);

      //Ovo ce se izvrsiti 7 puta
    }

  //Kursor za sledeci prikaz
  lcd.setCursor(0,0);

  //Brisemo sve sa displeja
  lcd.clear();
  lcd.print("Izvlacenje");

  //Broj je opcion, predstavlja trajanje izvlacenja
  delay(2000);

  //Pozivamo funkciju za nasumicno generisanje brojeva 
  //Funkcija lotofun() smesta nasumicne brojeve u loto[7] niz
  lotofun();

  //Cistimo displej
  lcd.clear();
  //Postavljamo kursor za sledeci ekran
  lcd.setCursor(0, 0);

  //Ovdije prebrojavanje i printanje loto[7] niza dijelimo u dvije for petlje
  //Prva prikazuje prva cetri broja na prvoj liniji ekrana
  //Druga prikazuje poslednja tri broja na drugoj linji ekrana
  //Da bi sve stalo sa zarazima
  
  for(i=0; i < 4; i++) {  //Prva
      lcd.print(loto[i]);
      lcd.print(",");
      delay(1200); //Vreme izvlacenja izmedju brojeva
    }
    lcd.setCursor(0,1);
  for(i=4; i < 7; i++) {  //Druga
      lcd.print(loto[i]);
      //Provjerava da li smo stigli prebrojavnjem
      // do poslednjeg clana, ako jesmo uklanja poslednji zarez
      // da bi format izgledao 1,2,3,4,5,6,7 a ne 1,2,3,4,5,6,7,
      if( i != 6){
      lcd.print(",");
      }
      delay(1200); //Vrijeme izvlacenja izmedju brojeva
    }

  // Pozivamo funkciju koja provjerava koliko smo brojeva pogodili  
  pogodjeno();

  //Cistimo displej
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Broj Pogodaka: ");
  
  // Prikazujemo broj pogodaka na prvoj liniji ekrana
  lcd.print(pogodak);

  lcd.setCursor(0,1);

  // Ako je broj pogodaka 7 na drugoj liniji prikazujemo "Cestitamo"
  // ako nije prikazujemo "Vise Srece"
  if(pogodak == 7){
    lcd.print("Cestitamo!");
  }else{
    lcd.print("Vise srece!");
    }
   
}
//FUNKCIJA UNOS
void unos(){
  //Ceka dok se vrijednost Seriala ne promjeni koja je po defaultu 0
  //To jest odje kod "stoji" dok Serial.available() ne bude razlicit od 0
  while(Serial.available() == 0);
  //Ako unesemo nesto u Serial terminal mijenjamo vrijednost Serial.available()
  // Pa onda kod nastavlja dalje

  //Ocitavamo vrijedost sa Seriala i prebacujemo je u Int
  //Kreiramo privremenu promjenjivu koja cuva nas unos
  int temp = Serial.parseInt();   

  //Provjerava dal je broj u trazenom opsegu
  //Ako nije poziva se na pocetak
  if(temp > 49 || temp < 0) {
    Serial.println("");
    Serial.println("Broj Nije Vazeci");
    Serial.println("Unesi opet.");
    Serial.end(); //Brisemo unetu vrijednost, tj. gasimo port
    Serial.begin(9600); // Otvaramo port opet - vrijednost je izbrisana
    unos(); // Pozivamo se opet
    }
  brojevi[i] = temp; // Pakujemo ocitane vrijednosti u niz brojeva
                      // Brojac i se menja u void loop()
  
  Serial.println("");
  Serial.print(i+1);  //Estetike radi prikazujemo da je broj uspesno unet
  Serial.print(" Broj Unet");

  //Gasimo port i brisemo sve zaostale vrednosti
  Serial.end();
  //Otvaramo novi za sledece citanje
  Serial.begin(9600);
  
}

//FUNKCIJA LOTOFUN
void lotofun() {
    // Obicnom for petljom na svakom nesto loto[] niza stavljamo po  jedan
    // nasumicni broj
   for(i=0; i < 7; i++) {
      
      loto[i] = random(40);
      //Vrti sedam puta
    }

}

//FUNKCIJA POGODJENO
void pogodjeno() {
  // Koristimo nestovane for petlje da bi provjerili svaki sa svakim clanom
  for(i=0; i < 7; i++) {
    // U ovo for petlji brojimo i do 7
    for(j=0; j<7; j++){
      //U ovoj petlji i se ne menja ali zato brojimo j do 7
      
      // i = 1, j = 1
      // i = 1, j = 2
      // ...
      // i = 2, j = 1
      // ...
     if(brojevi[i] == loto[j]){ 
        pogodak++;
      }
    }
    }
}
