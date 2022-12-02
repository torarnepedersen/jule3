/*
 * Jule3 av Tor Arne Pedersen
 * Effekten er at fra en farge velges det en tilfeldig kanal (rød, grønn eller blå) som skal få en ny
 * tilfeldig fargetone (0-255). Deretter fades alle juletrekulene over til ny farg mens kule nr 2, 3 etc
 * henger <stegmellomoppdatering> antall trinn etter i fadingen som gir en slags "bølgeeffekt". For at 
 * dette skal være mulig må mikrokontrolleren huske <stegmellomoppdatering> verdier for alle kanaler
 * og alle juletrekuler, så dette er ganske minnekrevende. En Arduino uno med 12 juletrekuler og fem
 * steg mellom oppdatering bruker 420 bytes (20 % av dynamisk minne).
 * 
 * To-do:
 * Slå sammen løsning for å fade opp og fade ned vha feks en variabel som er endringstrinn=1 eller -1.
 * Slå sammen løsning for å gå gjennom tre kanaler uten at det går ut over hastigheten.
 */

#include <Adafruit_NeoPixel.h>
typedef struct {  //typedef struct lager en variabel som kan inneholde flere elementer. Elementene kan være av forskjellig type, selv om alle her er byte. Brukes som julekulefarge.B feks.
  byte R;
  byte G;
  byte B;
} julekulefarge;

const int antalljulekuler = 12;
const int stegmellomoppdatering = 10;
julekulefarge julekule[antalljulekuler*stegmellomoppdatering] = {};  //Her lages et array av typen julekulefarge. Et array er en rekke variabler av samme type.

int velgfarge=0;
int velgfargetone=0;

const int neopin = 2;
Adafruit_NeoPixel juletre = Adafruit_NeoPixel(antalljulekuler, neopin, NEO_GRB + NEO_KHZ800);

void initminnematrise() {  //Fyll opp alle elementer i hele array med nullverdier.
  for (byte i=0; i<antalljulekuler*stegmellomoppdatering; i++) {
    julekule[i].R=0;
    julekule[i].G=0;
    julekule[i].B=0;
  }
}

void oppdaterjulekuler() {
  for (byte i=0; i<antalljulekuler;i++) {
    juletre.setPixelColor(i, juletre.Color(julekule[i*stegmellomoppdatering].R, julekule[i*stegmellomoppdatering].G, julekule[i*stegmellomoppdatering].B));
  }
  juletre.show();
  delay(10);
}

void skiftfarge() {
  velgfarge=random(3);
  velgfargetone=random(4)*85;  //Hvis man velger random(255) i RGB-kanaler blir det veldig mye blasse farger. Med random(4)*85 får du 0,85,170 eller 255.
  if (velgfarge==0) {
    if (velgfargetone>julekule[0].R) { //fargen skal tones opp
      byte endringsskala=velgfargetone-julekule[0].R;
      for (byte i=0; i<endringsskala;i++) { //for hver endring i fargetone
        for (int j=antalljulekuler*stegmellomoppdatering-1; j>0;j--){ //for hver linje i minnematrise unntatt første
          julekule[j].R=julekule[j-1].R;
        }
        julekule[0].R++;  //julekule[0].R økes
        oppdaterjulekuler();
      }
    }
    else if (velgfargetone<julekule[0].R) {  //fargen skal tones ned
      byte endringsskala=julekule[0].R-velgfargetone;
      for (byte i=0; i<endringsskala;i++) { //for hver endring i fargetone
        for (int j=antalljulekuler*stegmellomoppdatering-1; j>0;j--){ //for hver linje i minnematrise unntatt første
          julekule[j].R=julekule[j-1].R; //Hver verdi i matrisen som husker forrige verdi flyttes ett hakk bak.
        }

        julekule[0].R--;  //julekule[0].R reduseres
        oppdaterjulekuler();
      }      
    }
  }
    else if (velgfarge==1) {
    if (velgfargetone>julekule[0].G) { //fargen skal tones opp
      byte endringsskala=velgfargetone-julekule[0].G;
      for (byte i=0; i<endringsskala;i++) { 
        for (int j=antalljulekuler*stegmellomoppdatering-1; j>0;j--){ 
          julekule[j].G=julekule[j-1].G;
        }
        julekule[0].G++;  //julekule[0].G økes
        oppdaterjulekuler();
      }
    }
    else if (velgfargetone<julekule[0].G) {  //fargen skal tones ned
      byte endringsskala=julekule[0].G-velgfargetone;
      for (byte i=0; i<endringsskala;i++) { //for hver endring i fargetone
        for (int j=antalljulekuler*stegmellomoppdatering-1; j>0;j--){ //for hver linje i minnematrise unntatt første
          julekule[j].G=julekule[j-1].G;
        }
        julekule[0].G--;  //julekule[0].G reduseres
        oppdaterjulekuler();
      }      
    }
  }
    else if (velgfarge==2) {
    if (velgfargetone>julekule[0].B) { //fargen skal tones opp
      byte endringsskala=velgfargetone-julekule[0].B;
      for (byte i=0; i<endringsskala;i++) { //for hver endring i fargetone
        for (int j=antalljulekuler*stegmellomoppdatering-1; j>0;j--){ //for hver linje i minnematrise unntatt første
          julekule[j].B=julekule[j-1].B;
        }
        julekule[0].B++;  //julekule[0].B økes
        oppdaterjulekuler();
      }
    }
    else if (velgfargetone<julekule[0].B) {  //fargen skal tones ned
      byte endringsskala=julekule[0].B-velgfargetone;
      for (byte i=0; i<endringsskala;i++) { //for hver endring i fargetone
        for (int j=antalljulekuler*stegmellomoppdatering-1; j>0;j--){ //for hver linje i minnematrise unntatt første
          julekule[j].B=julekule[j-1].B;
        }
        julekule[0].B--;  //julekule[0].B reduseres
        oppdaterjulekuler();
      }      
    }
  }
}

void setup() {
  initminnematrise();
  juletre.begin();
  }
  
void loop() {
  skiftfarge();
  }
