#include <Keypad.h>
#define XG 10
#define XR 11
#define OG 12
#define OR 13


//board
const int BRC = 3;
char board[BRC][BRC]; 
char turn = 'X';

//4x4 keypad setup
const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


void setup(){
  //clear board;
  for (int i = 0; i < BRC; i++)
    for (int j = 0; j < BRC; j++)
      board[i][j] = '-';
      
  //turn on X and turn off O
  digitalWrite(XG, HIGH);
  digitalWrite(OG, LOW);
  
  Serial.begin(9600);
}
  
void loop(){
  //define intput and it's pos
  int input = 0;
  int row = 0;
  int col = 0;
  
  //get input until valid
  while (!input) {
    char customKey = customKeypad.getKey();
    //get input until nothing entered
    while (!customKey) {
      customKey = customKeypad.getKey();
    }
    //check if valid when entered
    input = customKey-'0';
    //get col and row
    if (input % BRC == 0){
      row = input/BRC-1;
      col = BRC-1;
    }
    else{
      row = input/BRC;
      col = input%BRC-1;
    }
    if (board[row][col] == '-')
      board[row][col] = turn;
    else {
      input = 0;
      //Serial.println("invalid");
      for(int i = 1; i <= 3; i++){
        if (turn == 'X'){
          digitalWrite(XR, HIGH);
          delay(200);
          digitalWrite(XR, LOW);
          delay(200);
        }
        else {
          digitalWrite(OR, HIGH);
          delay(200);
          digitalWrite(OR, LOW);
          delay(200);
        }
      }
    }
  }

  //move to the selected space and write down the letter
  for (int i = 0; i < BRC; i++){
    for (int j = 0; j < BRC; j++){
      Serial.print(board[i][j]);
      Serial.print(' ');
    }
    Serial.println();
  }
  Serial.println();

  //check if won
  bool won = false;
  bool con = true;
  //check horizontally
  for (int i = 0; i < BRC; i++){
    con = true;
    for (int j  = 1; j < BRC; j++)
      if (board[i][j] != board[i][j-1] || board[i][j] == '-')
        con = false;
    won |= con;
  }
  //check vertically
  for (int i = 0; i < BRC; i++){
    con = true;
    for (int j  = 1; j < BRC; j++)
      if (board[j][i] != board[j-1][i]  || board[j][i] == '-')
        con = false;
    won |= con;
  }
  //check diagonally left  
  con = true;
  for (int i = 1; i < BRC; i++)
    if (board[i][i] != board[i-1][i-1] || board[i][i] == '-')
      con = false;
  won |= con;
  //check diagonally right
  con = true;
  int j = BRC-2;
  for (int i = 1; i < BRC; i++){
    if (board[j][i] != board[j+1][i-1] || board[j][i] == '-')
      con = false;
    j--;
  }
  won |= con;
  //if won end the game
  while (won) {
    if (turn == 'X'){
      digitalWrite(XG, HIGH);
      digitalWrite(OR, HIGH);
      delay(200);  
      digitalWrite(XG, LOW);
      digitalWrite(OR, LOW);
      delay(200);  
    }
    else{
      digitalWrite(XR, HIGH);
      digitalWrite(OG, HIGH);
      delay(200);  
      digitalWrite(XR, LOW);
      digitalWrite(OG, LOW);
      delay(200);  
    }
  }

  //check if draw
  bool draw = true;
  for (int i = 0; i < BRC; i++)
    for (int j = 0; j < BRC; j++)
      if (board[i][j] == '-')
        draw = false;
  while (draw){
    digitalWrite(XG, HIGH);
    digitalWrite(OG, HIGH);
    delay(200);
    digitalWrite(XG, LOW);
    digitalWrite(OG, LOW);
    delay(200);
  }
  
  //change turns
  if (turn == 'X'){
    turn = 'O';
    digitalWrite(XG, LOW);
    digitalWrite(OG, HIGH);
  }
   else{
    turn = 'X';
    digitalWrite(OG, LOW);
    digitalWrite(XG, HIGH);
   }
}
