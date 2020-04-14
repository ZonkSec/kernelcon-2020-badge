#include <APA102.h>
#include <EEPROM.h>

// apa102 pins
const uint8_t dataPin = 18;
const uint8_t clockPin = 19;

//input pins
int chalpin = 9;  
int btnpin =2;  

//variables to aid in menu 
int mode = EEPROM.read(0);
uint8_t interupted = 0;
uint8_t menublink = 1;
int modenumber = 11;

uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;
uint8_t order[] = {1,2,3,6,5,0};

// Create an object for writing to the LED strip. and supporting variables
APA102<dataPin, clockPin> ledStrip;
const uint16_t ledCount = 9;
rgb_color colors[ledCount];
rgb_color last_colors[ledCount];
uint8_t brightness = 2;

void setup()
{
  pinMode(btnpin, INPUT);
  pinMode(chalpin, INPUT);
  attachInterrupt(digitalPinToInterrupt(btnpin), push, FALLING);
  randomSeed(analogRead(0));
  Serial.begin(9600);
  Serial.println(F("BOOT"));
}

//**********************************************************************************************
// MAIN LOOP - operates the menu based on a counter intremented by interupt button press
// counter then calls into a function to handle that specific mode. checks for doublepress and serial state
//***********************************************************************************************
void loop()
{
  interupted = 0;
  //checks for chal 1 pad to see if shorted. if so, does special animation
  if (digitalRead(chalpin)==LOW){
    detachInterrupt(digitalPinToInterrupt(btnpin));
    chase(0,0,255,100);
    chase(0,0,255,100);
    chase(0,0,255,50);
    chase(0,0,255,50);
    chase(0,0,255,25);
    chase(0,0,255,10);
    chase(0,0,255,10);
    chase(0,0,255,5);
    chase(0,0,255,5);
    chase(0,0,255,5);
    chase(0,0,255,5);
    chase(0,0,255,1);
    chase(0,0,255,1);
    chase(0,0,255,1);
    chase(0,0,255,1);
    chase(0,0,255,1);
    chase(0,0,255,1);
    chase(0,0,255,5);
    chase(0,0,255,5);
    chase(0,0,255,5);
    chase(0,0,255,5);
    chase(0,0,255,10);
    chase(0,0,255,10);
    chase(0,0,255,25);
    chase(0,0,255,50);
    chase(0,0,255,50);
    Serial.begin(9600);
    Serial.println(F("                       /)"));
    Serial.println(F("              /\\___/\\ (("));
    Serial.println(F("              \\`@_@'/  ))"));
    Serial.println(F("              {_:Y:.}_//"));
    Serial.println(F("--------------{_}^-'{_}-------------------------------------------------"));
    Serial.println(F("You entered the sewers! Nice work! Have a special light mode!\n"));
    Serial.println(F("c3VwZXJzZWNyZXRiYWRnZXBsYWNlLmtlcm5lbGNvbi5vcmc=\n"));
    Serial.println(F("There was a fun CTF challenge/game here,"));
    Serial.println(F("but has since been removed when the con went virtual.\n"));
    Serial.println(F("Come back next year! w/ <3, -@zonksec"));
    Serial.println(F("PS: im curious how many people will find this. DM on twitter if you do!"));
    Serial.println(F("------------------------------------------------------------------------"));
  }
  else{
      Serial.println(F("Find the entrance to the sewer..."));
      switch (mode % modenumber){
        case 0: //MODE: each color fades rainbow independtly
          mode_indicator(1,0);
          rainbow(70);
          break;
        case 1: //MODE: lock previous fade state
          break;
        case 2: //MODE: each color fades rainbow together
          mode_indicator(2,0);
          rainbow(0);
          break;
        case 3: //MODE: lock previous fade state
          break;
        case 4: //MODE only reel backlights are on
          mode_indicator(3,0);
          for(uint16_t i = 0; i < ledCount; i++){
            colors[i].red = 0;
            colors[i].green = 0;
            colors[i].blue = 0;
          }
          colors[8].red = 255;
          colors[8].green = 255;
          colors[8].blue = 255;
          colors[7].red = 255;
          colors[7].green = 255;
          colors[7].blue = 255;
          ledStrip.write(colors, ledCount, brightness);
          break;
        case 5: //MODE strobes colors together
          mode_indicator(0,0);
          party();
          break;
        case 6: //MODE chase LED, color change each loop
          mode_indicator(5,0);
          party_chase();
          break;  
        case 7: //MODE knight rider
          mode_indicator(6,0);
          knight_rider();
          break; 
        case 8: //MODE twirling bar
          mode_indicator(1,1);
          party_swing();
          break; 
        case 9: //MODE color_twinkle
          mode_indicator(2,1);
          color_twinkle();
          break;
        case 10: //MODE white_twinkle
          mode_indicator(3,1);
          white_twinkle();
          break;
    }
  }
}

//this is the innterupt fuction. increments mode and handles double press logic
void push(){
  mode++;
  delay(10); //debounce delay
  interupted = 1;
  menublink = 1;

  //only set to eeprom if mode not one of the lock fade modes (cause their state is not preserved and causes black boot)
  if ((mode % modenumber) == 1){
    EEPROM.write(0, ((mode % modenumber)-1));
  }
  else if ((mode % modenumber) == 3){
    EEPROM.write(0, ((mode % modenumber)-1));
  }
  else{
    EEPROM.write(0, (mode % modenumber));
  }
  return;
}

//this is used to blnk what mode its in
void mode_indicator(uint8_t led, uint8_t flash){
    if (menublink == 1){
      memset(colors, 0, sizeof(colors));
      colors[led].red = 255;
      colors[led].green = 255;
      colors[led].blue = 255;
      ledStrip.write(colors, ledCount, brightness);
      if (flash == 1){
        delay(333);
        solid_color(0,0,0);
        delay(333);
        colors[led].red = 255;
        colors[led].green = 255;
        colors[led].blue = 255;
        ledStrip.write(colors, ledCount, brightness);
        delay(333);
      }
      else{
        delay(1000);
      }
    menublink = 0;
  }
}

void color_twinkle(){
  memset(colors, 0, sizeof(colors));
  ledStrip.write(colors, ledCount, brightness); 
  colors[random(9)].red = 255;
  colors[random(9)].green = 255;
  colors[random(9)].blue = 255;
  colors[random(9)].red = 255;
  colors[random(9)].green = 255;
  colors[random(9)].blue = 255;
  colors[random(9)].red = 255;
  colors[random(9)].green = 255;
  colors[random(9)].blue = 255;
  if(interupted == 1) {
    return;
  }
  ledStrip.write(colors, ledCount, brightness); 
  delay(250);

}

void white_twinkle(){
  int x = random(9);
  int y = random(9);
  int z = random(9);
  memset(colors, 0, sizeof(colors));
  ledStrip.write(colors, ledCount, brightness); 
  colors[x].red = 255;
  colors[x].green = 255;
  colors[x].blue = 255;
  colors[y].red = 255;
  colors[y].green = 255;
  colors[y].blue = 255;
  colors[z].red = 255;
  colors[z].green = 255;
  colors[z].blue = 255;
  if(interupted == 1) {
    return;
  }
  ledStrip.write(colors, ledCount, brightness); 
  delay(50);
}

void knight_rider(){
  memset(colors, 0, sizeof(colors));
  ledStrip.write(colors, ledCount, brightness); 
  colors[0].red = 255;
  colors[0].green = 0;
  colors[0].blue = 0;
  colors[1].red = 255;
  colors[1].green = 0;
  colors[1].blue = 0;
  colors[7].red = 255;
  colors[7].green = 0;
  colors[7].blue = 0;
  if(interupted == 1) {
    return;
  }
  ledStrip.write(colors, ledCount, brightness); 
  delay(250);

  memset(colors, 0, sizeof(colors));
  ledStrip.write(colors, ledCount, brightness); 
  colors[2].red = 255;
  colors[2].green = 0;
  colors[2].blue = 0;
  colors[4].red = 255;
  colors[4].green = 0;
  colors[4].blue = 0;
  colors[5].red = 255;
  colors[5].green = 0;
  colors[5].blue = 0;
  if(interupted == 1) {
    return;
  }
  ledStrip.write(colors, ledCount, brightness); 
  delay(250);
  
  memset(colors, 0, sizeof(colors));
  ledStrip.write(colors, ledCount, brightness); 
  colors[3].red = 255;
  colors[3].green = 0;
  colors[3].blue = 0;
  colors[8].red = 255;
  colors[8].green = 0;
  colors[8].blue = 0;
  colors[6].red = 255;
  colors[6].green = 0;
  colors[6].blue = 0;
  if(interupted == 1) {
    return;
  }
  ledStrip.write(colors, ledCount, brightness); 
  delay(250);

    memset(colors, 0, sizeof(colors));
  ledStrip.write(colors, ledCount, brightness); 
  colors[2].red = 255;
  colors[2].green = 0;
  colors[2].blue = 0;
  colors[4].red = 255;
  colors[4].green = 0;
  colors[4].blue = 0;
  colors[5].red = 255;
  colors[5].green = 0;
  colors[5].blue = 0;
  if(interupted == 1) {
    return;
  }
  ledStrip.write(colors, ledCount, brightness); 
  delay(250);
}

void solid_color(uint8_t r, uint8_t g, uint8_t b){
  if(interupted == 1) {
    return;
  }
  for(uint16_t i = 0; i < ledCount; i++){
    colors[i].red = g;
    colors[i].green = b;
    colors[i].blue = r;
  }
  ledStrip.write(colors, ledCount, brightness);
}

rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v){
    uint8_t f = (h % 60) * 255 / 60;
    uint8_t p = (255 - s) * (uint16_t)v / 255;
    uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch((h / 60) % 6){
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    return rgb_color(r, g, b);
}

void rainbow(uint8_t x){
  uint8_t time = millis() >> 4;
  for(uint16_t i = 0; i < ledCount; i++)
  {
    uint8_t p = time - i * x; //seems to be the delay between LEDs. low they all change together, higer they are more seperate
    colors[i] = hsvToRgb((uint32_t)p * 359 / 256, 255, 255);
  }
  colors[8].red = 255;
  colors[8].green = 255;
  colors[8].blue = 255;
  colors[7].red = 255;
  colors[7].green = 255;
  colors[7].blue = 255;
  ledStrip.write(colors, ledCount, brightness);
}

void party(){
    solid_color(55,0,0);delay(50);
    solid_color(0,255,0);delay(50);
    solid_color(0,0,255);delay(50);
    solid_color(255,255,0);delay(50);
    solid_color(0,255,255);delay(50);
    solid_color(255,0,255);delay(50);
    solid_color(128,0,0);delay(50);
    solid_color(128,128,0);delay(50);
    solid_color(0,128,0);delay(50);
    solid_color(128,0,128);delay(50);
    solid_color(0,128,128);delay(50);
    solid_color(0,0,128);delay(50);
  }
  
void party_chase(){
    chase(55,0,0,50);
    chase(0,255,0,50);
    chase(0,0,255,50);
    chase(255,255,0,50);
    chase(0,255,255,50);
    chase(255,0,255,50);
    chase(192,192,192,50);
    chase(128,128,128,50);
    chase(128,0,0,50);
    chase(128,128,0,50);
    chase(0,128,0,50);
    chase(128,0,128,50);
    chase(0,128,128,50);
    chase(0,0,128,50);
}
void party_swing(){
    bar_swing(255,255,255);
    bar_swing(255,0,0);
    bar_swing(255,128,0);
    bar_swing(255,255,0);
    bar_swing(128,255,0);
    bar_swing(0,255,0);
    bar_swing(0,255,128);
    bar_swing(0,255,255);
    bar_swing(0,128,255);
    bar_swing(0,0,255);
    bar_swing(128,0,255);
    bar_swing(255,0,255);
    bar_swing(255,0,128);
}

void chase(uint8_t r, uint8_t g, uint8_t b, uint8_t wait){
  for(uint16_t x = 0; x < 6; x++){
      if(interupted == 1) {
        return;
      }
      for(uint16_t i = 0; i < ledCount; i++){
        if(interupted == 1) {
           return;
        }
        colors[i].red = 0;
        colors[i].green = 0;
        colors[i].blue = 0;
      }
      colors[order[x]].red = g;
      colors[order[x]].green = b;
      colors[order[x]].blue = r;
      colors[8].red = 255;
      colors[8].green = 255;
      colors[8].blue = 255;
      colors[7].red = 255;
      colors[7].green = 255;
      colors[7].blue = 255;
      ledStrip.write(colors, ledCount, brightness);
      if(interupted == 1) {
            return;
      }
  delay(wait);
  }
}
void bar_swing(uint8_t r, uint8_t g, uint8_t b){
  memset(colors, 0, sizeof(colors));
  ledStrip.write(colors, ledCount, brightness); 
  colors[0].red = r;
  colors[0].green = g;
  colors[0].blue = b;
  colors[4].red = r;
  colors[4].green = g;
  colors[4].blue = b;
  colors[3].red = r;
  colors[3].green = g;
  colors[3].blue = b;
  colors[8].red = 255;
  colors[8].green = 255;
  colors[8].blue = 255;
  colors[7].red = 255;
  colors[7].green = 255;
  colors[7].blue = 255;
  if(interupted == 1) {
    return;
  }
  ledStrip.write(colors, ledCount, brightness); 
  delay(250);
  memset(colors, 0, sizeof(colors));
  ledStrip.write(colors, ledCount, brightness); 
  colors[1].red = r;
  colors[1].green = g;
  colors[1].blue = b;
  colors[4].red = r;
  colors[4].green = g;
  colors[4].blue = b;
  colors[6].red = r;
  colors[6].green = g;
  colors[6].blue = b;
  colors[8].red = 255;
  colors[8].green = 255;
  colors[8].blue = 255;
  colors[7].red = 255;
  colors[7].green = 255;
  colors[7].blue = 255;
  if(interupted == 1) {
    return;
  }
  ledStrip.write(colors, ledCount, brightness); 
  delay(250);
  memset(colors, 0, sizeof(colors));
  ledStrip.write(colors, ledCount, brightness); 
  colors[2].red = r;
  colors[2].green = g;
  colors[2].blue = b;
  colors[4].red = r;
  colors[4].green = g;
  colors[4].blue = b;
  colors[5].red = r;
  colors[5].green = g;
  colors[5].blue = b;
  colors[8].red = 255;
  colors[8].green = 255;
  colors[8].blue = 255;
  colors[7].red = 255;
  colors[7].green = 255;
  colors[7].blue = 255;
  if(interupted == 1) {
    return;
  }
  ledStrip.write(colors, ledCount, brightness); 
  delay(250);
}
