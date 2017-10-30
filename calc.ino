
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
void evaluate (char []);
int precedence(char);
double calculate(char , double , double );
char checknumber(char);


const int left = 9;
const int right = 8;
const int enter = 7;
const int del = 6;
int pos = 15;
int x = 0;
int leftState = 0;
int rightState = 0;
int enterState = 0;
int delState = 0;
int lastLeftState = 0;
int lastRightState = 0;
int lastEnterState = 0;
int lastDelState = 0;
char output[16] = "_______________0";
#include <LiquidCrystal.h> //Load Liquid Crystal Library
LiquidCrystal lcd(12,11,5,4,3,2);  //Create Liquid Crystal Object called LCD
// defines variables
void calc(char input[16]);
void setup() {
pinMode(left, INPUT); // Sets the trigPin as an Output
pinMode(right, INPUT); // Sets the echoPin as an Input
pinMode(enter, INPUT);
pinMode(del, INPUT);
Serial.begin(9600); // Starts the serial communication
  
    lcd.begin(16,2); //Tell Arduino to start your 16 column 2 row LCD
    lcd.clear();
    lcd.print(output);
}

void loop() {
  leftState = digitalRead(left);
  rightState = digitalRead(right);
  enterState = digitalRead(enter);
  delState = digitalRead(del);
  char arr[15] = "0123456789+-*/S";
  
  if (leftState != lastLeftState) {
    if(leftState == LOW) {
      x -= 1;
      if(x < 0) {
        x = 14;
      }
      output[pos] = arr[x];
      lcd.clear();
      lcd.print(output);
    } 
    delay(100);
   }
   lastLeftState = leftState;

   if (rightState != lastRightState) {
    if(rightState == LOW) {
      x += 1;
      if(x > 14) {
        x = 0;
      }
      
      output[pos] = arr[x];
      lcd.clear();
      lcd.print(output);
    } 
    delay(100);
   }
   lastRightState = rightState;

   if (enterState != lastEnterState) {
    if(enterState == LOW) {
      if(output[15] == 'S') {
        calc(output);
        for(int i = 0; i < 15; i++) {
          output[i] = '_';
        }
        output[15] = '0';
      } else {
        for(int i = 0; i < 15; i++) {
          output[i] = output[i+1];
        }
        output[15] = '0';
        x = 0;
        
        lcd.clear();
        lcd.print(output);
      }
    } 
    delay(100);
   }
   lastEnterState = enterState;

   if (delState != lastDelState) {
    if(delState == LOW) {
      for(int i = 15; i >=1; i--) {
        output[i] = output[i-1];
      }
      output[0] = '_';
      if(output[15]>='0' && output[15]<='9') {
        x = output[15] - '0';
      } else if(output[15]=='+') {
        x = 10;
      } else if(output[15]=='-') {
        x = 11;
      } else if(output[15]=='*') {
        x = 12;
      } else if(output[15]=='/') {
        x = 13;
      }
      lcd.clear();
      lcd.print(output);
    } 
    delay(100);
   }
   lastDelState = delState;
}

void calc(char input[16]) {
  input[15] = '_';
  evaluate(input);
}


void evaluate(char expr[])
{
    expr[15] = '_';
    Serial.print("X");
    Serial.print(expr);
    double numbers[5]; int nsi = 0;
    char operators[5]; int osi = 0;
    char numbuf[16]; int nbi = 0;
    char ch; int  i = 0;
    
    while (i<16) {
        ch = expr[i];
        if(ch == '_' || ch == 'S'){
          i++;
          continue;
        }
        if (checknumber(ch)) 
        {
            numbuf[nbi++] = ch;
            if (!checknumber(expr[i + 1])) 
            {
                Serial.println();
                numbuf[nbi] = '\0'; nbi = 0;
                Serial.println(numbuf);
                numbers[nsi++] = strtod(numbuf, NULL);
                //sscanf(numbuf, "%lf", &numbers[nsi++]); 
            }
        }
        else
        {
            while ((osi > 0) && (precedence(ch) <= precedence(operators[osi - 1])))
            {
                Serial.println(numbers[nsi-2]);
                Serial.println(numbers[nsi-1]);
                numbers[nsi - 2] = calculate(operators[osi - 1], numbers[nsi - 2], numbers[nsi - 1]);
                osi--; nsi--;
            }
            operators[osi++] = ch;          
        }
        i++;
    }
    while (osi > 0) {
        numbers[nsi - 2] = calculate(operators[osi - 1], numbers[nsi - 2], numbers[nsi - 1]);
        osi--; nsi--;
    }

    // sprintf(expr, "%16.0f", (numbers[0]));
    //memcpy(expr, &numbers[0], sizeof(numbers[0]));
    lcd.clear();
    lcd.print(numbers[0]);
    Serial.print("Result");
    Serial.print(numbers[0]);
    //return numbers[0];
}

char checknumber(char ch) 
{
    if ((ch >= '0' && ch <= '9') || ch == '.') return 1; else return 0;
}
int precedence(char ch)
{
    int precedence;
    switch (ch) 
    {
    case '+':
    case '-':
        precedence = 0;
        break;
    case '*':
    case '/':
        precedence = 1;
        break;
    case '^':
        precedence = 2;
    }
    return precedence;
}

double calculate(char moperator, double num1, double num2) 
{
    Serial.println();
    Serial.println(num1);
    Serial.println(num2);
    Serial.println(moperator);
    double result;
    switch (moperator)
    {
    case '+':
        result = num1 + num2;
        break;
    case '-':
        result = num1 - num2;
        break;
    case '*':
        
        result = num1 * num2;
        break;
    case '/':
        result = num1 / num2;
        break;
    case '^':
        result = pow(num1, num2);
        break;
    default:
        printf("Invalid Operator\n");
        exit(-1);
    }
    return result;
}
