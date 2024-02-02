#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

#define HIT 5
#define STAND 17
#define TFT_DC 2
#define TFT_CS 15

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
const int cardWidth = 60;
const int cardHeight = 80;
const int borderWidth = 2;
const uint16_t cardBack = tft.color565(164, 29, 19); 
enum cardSymbol {DIAMOND, HEART, CLUB, SPADE};
enum cardNum { TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, J, Q, K, A};
const char* cardNames[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
int chit = 0;
int isWin = 0;

struct Card{
  cardSymbol symbol;
  cardNum num;
};

struct Person{
  Card cards[10];
  int nr_cards;
};

Person dealer, player;

void setup() {
  dealer.nr_cards = 0;
  player.nr_cards = 0;
  pinMode(HIT, INPUT_PULLUP);
  pinMode(STAND, INPUT_PULLUP);
  tft.begin();
  initblackjack();
}

void backgroundGame() {
  tft.fillScreen(ILI9341_DARKGREEN);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2); 
  tft.setCursor(85, 10);
  tft.print("DEALER");
  tft.setCursor(100, 180);
  tft.print("YOU");
  
  tft.drawRect(50,50,cardWidth, cardHeight, ILI9341_WHITE);
  
  tft.drawRect(135,50,cardWidth, cardHeight, ILI9341_WHITE);
  tft.fillRect(135 + borderWidth, 50 + borderWidth, cardWidth - 2 * borderWidth, cardHeight - 2 * borderWidth, cardBack);
 
  tft.drawRect(140,45,cardWidth, cardHeight, ILI9341_WHITE);
  tft.fillRect(140 + borderWidth, 45 + borderWidth, cardWidth - 2 * borderWidth, cardHeight - 2 * borderWidth, cardBack);

  tft.drawRect(145,40,cardWidth, cardHeight, ILI9341_WHITE);
  tft.fillRect(145 + borderWidth, 40 + borderWidth, cardWidth - 2 * borderWidth, cardHeight - 2 * borderWidth, cardBack);

  tft.drawRect(88,220,cardWidth, cardHeight, ILI9341_WHITE);
}

void drawDiamond(int x, int y, cardNum c){
  tft.fillRect(x + borderWidth, y + borderWidth, cardWidth - 2 * borderWidth, cardHeight - 2 * borderWidth, ILI9341_WHITE );
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(4); 
  tft.setCursor(x + cardWidth / 2 - 10, y + cardHeight / 2 - 10);
  tft.print((char)4);

  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(2); 
  tft.setCursor(x + 5, y + 5);
  tft.print(cardNames[c]);
}

void drawHeart(int x, int y, cardNum c){
  tft.fillRect(x + borderWidth, y + borderWidth, cardWidth - 2 * borderWidth, cardHeight - 2 * borderWidth, ILI9341_WHITE);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(4); 
  tft.setCursor(x + cardWidth / 2 - 10,y + cardHeight / 2 - 10);
  tft.print((char)3);

  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(2); 
  tft.setCursor(x + 5, y + 5);
  tft.print(cardNames[c]);
}

void drawClub(int x, int y, cardNum c){
  tft.fillRect(x + borderWidth, y + borderWidth, cardWidth - 2 * borderWidth, cardHeight - 2 * borderWidth, ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(4); 
  tft.setCursor(x + cardWidth / 2 - 10,y + cardHeight / 2 - 10);
  tft.print((char)5);

  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2); 
  tft.setCursor(x + 5, y + 5);
  tft.print(cardNames[c]);
}

void drawSpade(int x, int y, cardNum c){
  tft.fillRect(x + borderWidth, y + borderWidth, cardWidth - 2 * borderWidth, cardHeight - 2 * borderWidth, ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(4); 
  tft.setCursor(x + cardWidth / 2 - 10,y + cardHeight / 2 - 10);
  tft.print((char)6);

  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2); 
  tft.setCursor(x + 5, y + 5);
  tft.print(cardNames[c]);
}

void drawInvalid(int x, int y){
  tft.fillRect(x + borderWidth, y + borderWidth, cardWidth - 2 * borderWidth, cardHeight - 2 * borderWidth, cardBack);
}

void initdealerCards(){
  randomSeed(analogRead(0));
  cardSymbol firstSymbol = generateRandSymbol();
  cardNum firstNum = generateRandNum();

  switch (firstSymbol) {
      case DIAMOND:
        dealer.cards[dealer.nr_cards++] = {DIAMOND, firstNum};
        drawDiamond(50,50, firstNum);
        break;
      case HEART:
        dealer.cards[dealer.nr_cards++] = {HEART, firstNum};
        drawHeart(50,50, firstNum);
        break;
      case CLUB:
        dealer.cards[dealer.nr_cards++] = {CLUB, firstNum};
        drawClub(50,50, firstNum);
        break;
      case SPADE:
        dealer.cards[dealer.nr_cards++] = {SPADE, firstNum};
        drawSpade(50,50, firstNum);
        break;
    }

  cardSymbol secondSymbol = generateRandSymbol();
  cardNum secondNum = generateRandNum();

   switch (secondSymbol) {
      case DIAMOND:
        dealer.cards[dealer.nr_cards++] = {DIAMOND, secondNum};
        drawInvalid(62,62);
        break;
      case HEART:
        dealer.cards[dealer.nr_cards++] = {HEART, secondNum};
        drawInvalid(62,62);
        break;
      case CLUB:
        dealer.cards[dealer.nr_cards++] = {CLUB, secondNum};
        drawInvalid(62,62);
        break;
      case SPADE:
        dealer.cards[dealer.nr_cards++] = {SPADE, secondNum};
        drawInvalid(62,62);
        break;
    }
}

void initplayerCards(){
  randomSeed(analogRead(0));
  cardSymbol firstSymbol = generateRandSymbol();
  cardNum firstNum = generateRandNum();

  switch (firstSymbol) {
      case DIAMOND:
        player.cards[player.nr_cards++] = {DIAMOND, firstNum};
        drawDiamond(88,220, firstNum);
        break;
      case HEART:
        player.cards[player.nr_cards++] = {HEART, firstNum};
        drawHeart(88,220, firstNum);
        break;
      case CLUB:
        player.cards[player.nr_cards++] = {CLUB, firstNum};
        drawClub(88,220, firstNum);
        break;
      case SPADE:
        player.cards[player.nr_cards++] = {SPADE, firstNum};
        drawSpade(88,220, firstNum);
        break;
    }

  cardSymbol secondSymbol = generateRandSymbol();
  cardNum secondNum = generateRandNum();

  switch (secondSymbol) {
      case DIAMOND:
        player.cards[player.nr_cards++] = {DIAMOND, secondNum};
        drawDiamond(100,232, secondNum);
        break;
      case HEART:
        player.cards[player.nr_cards++] = {HEART, secondNum};
        drawHeart(100,232, secondNum);
        break;
      case CLUB:
        player.cards[player.nr_cards++] = {CLUB, secondNum};
        drawClub(100,232, secondNum);
        break;
      case SPADE:
        player.cards[player.nr_cards++] = {SPADE, secondNum};
        drawSpade(100,232, secondNum);
        break;
    }
}

void initgame(){
  initdealerCards();
  initplayerCards();
}

cardSymbol generateRandSymbol(){
  int randomSymbol = random(4);
  return static_cast<cardSymbol>(randomSymbol);
}

cardNum generateRandNum(){
  int randomNum = random(13);
  return static_cast<cardNum>(randomNum);
}

void initblackjack(){
 backgroundGame();
 initgame();
}

void hit(){
  cardSymbol newSymbol = generateRandSymbol();
  cardNum newNum = generateRandNum();

  switch (newSymbol) {
      case DIAMOND:
        player.cards[player.nr_cards++] = {DIAMOND, newNum};
        drawDiamond(100 + 12 *chit,232 + 12*chit, newNum);
        break;
      case HEART:
        player.cards[player.nr_cards++] = {HEART, newNum};
        drawHeart(100 + + 12*chit,232 + 12*chit, newNum);
        break;
      case CLUB:
        player.cards[player.nr_cards++] = {CLUB, newNum};
        drawClub(100 + 12*chit,232 + 12*chit, newNum);
        break;
      case SPADE:
        player.cards[player.nr_cards++] = {SPADE, newNum};
        drawSpade(100 + 12*chit,232 + 12*chit, newNum);
        break;
    }
}

int calculateSum(Person p){
  int sum = 0;
  int aces = 0;
  for (int i = 0; i < p.nr_cards; i++){
    cardNum num = p.cards[i].num;
    if(num == A) 
      aces++;
    else if(num == J || num == Q || num == K) 
      sum += 10;
    else 
      sum += num + 2;
  }

   for (int i = 0; i < aces; i++) {
    if (sum + 11 <= 21)
      sum += 11;
    else 
      sum += 1;
  }
  return sum;
}

void dealerContinue(){
  switch (dealer.cards[1].symbol) {
      case DIAMOND:
        drawDiamond(62,62,dealer.cards[1].num);
        break;
      case HEART:
        drawHeart(62,62,dealer.cards[1].num);
        break;
      case CLUB:
        drawClub(62,62,dealer.cards[1].num);
        break;
      case SPADE:
        drawSpade(62,62,dealer.cards[1].num);
        break;
  }

  int cstand = 1;
  while(calculateSum(dealer) < 17){
    delay(500);
    cstand++;
    cardSymbol newSymbol = generateRandSymbol();
    cardNum newNum = generateRandNum();
    switch (newSymbol) {
      case DIAMOND:
        drawDiamond(50 + cstand * 12, 50 + cstand * 12, newNum);
        break;
      case HEART:
        drawHeart(50 + cstand * 12, 50 + cstand * 12, newNum);
        break;
      case CLUB:
        drawClub(50 + cstand * 12, 50 + cstand * 12, newNum);
        break;
      case SPADE:
        drawSpade(50 + cstand * 12, 50 + cstand * 12, newNum);
        break;
    }
    dealer.cards[dealer.nr_cards++] = {newSymbol, newNum};
  }

  if(calculateSum(dealer) > 21)
    tft.setTextColor(ILI9341_RED);
  else 
    tft.setTextColor(ILI9341_WHITE);
  
  tft.setTextSize(2); 
  tft.setCursor(10, 10);
  tft.print(calculateSum(dealer));
}

void stand(){
  int playerValue = calculateSum(player);

  if(playerValue > 21)
    tft.setTextColor(ILI9341_RED);
  else 
    tft.setTextColor(ILI9341_WHITE);
  
  tft.setTextSize(2); 
  tft.setCursor(10, 180);
  tft.print(playerValue);

  dealerContinue();
  win();
}

void win(){
  int win = 0;
  int p = calculateSum(player);
  int d = calculateSum(dealer);

  if((p <= 21 && p > d) || d > 21)
    win = 1;
  else if(p == d)
    win = 0;
  else 
    win = -1;

  isWin = 1;

  if(win == 1){
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2); 
    tft.setCursor(200, 180);
    tft.print("W");
  }
  else if(win == 0){
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2); 
    tft.setCursor(200, 180);
    tft.print("D");
  }
  else{
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2); 
    tft.setCursor(200, 180);
    tft.print("L");
  }

}

void resetAll(){
  isWin = 0;
  dealer.nr_cards = 0;
  player.nr_cards = 0;
  chit = 0;
  initblackjack();
}

void loop() {
  if(isWin == 0){
    if(digitalRead(HIT) == LOW){
      chit++;
      hit();
      delay(200);
    }

    if(digitalRead(STAND) == LOW){
      stand();
      delay(200);
    }
  }
  else{
    delay(3000);
    resetAll();
  }
}
