/*
 * ampel-ino
 * 
 * für Vincent :-*
*/

#define TRAFFIC_BUTTON    2
#define BLUE_BUTTON       3
#define FLASHLIGHT_BUTTON 4
#define RED_LED           6
#define YELLOW_LED        7
#define GREEN_LED         8
#define BLUE_LED          9
#define WHITE_LED         5
#define BLUE2_LED         10

#define BUTTON_PRESSED  LOW
#define BUTTON_RELEASED HIGH

void setup() {
  pinMode(TRAFFIC_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  pinMode(FLASHLIGHT_BUTTON, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BLUE2_LED, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);

  digitalWrite(RED_LED, HIGH);
}

void loop() {
  trafficLight();
  flashBlue();
  switcher();
}


static bool lastButtonState = BUTTON_RELEASED;
static unsigned long switchTime = 0;
void switcher() {
  if(millis() > switchTime + 250 )
  {
    if (digitalRead(FLASHLIGHT_BUTTON) == BUTTON_PRESSED &&
        lastButtonState == BUTTON_RELEASED)
    {
      if(digitalRead(WHITE_LED) == LOW)
      {
        digitalWrite(WHITE_LED, HIGH);
      } else {
        digitalWrite(WHITE_LED, LOW);
      }
      switchTime = millis();
    }
    lastButtonState = digitalRead(FLASHLIGHT_BUTTON);  
  }
}


static int flashSequence[] = { 1, 0, 1, 0, 2, 0, 2, 0 };
static unsigned long flashTime = 0;
static int flashIndex = 0;
void flashBlue() {
  if (digitalRead(BLUE_BUTTON) == BUTTON_PRESSED)
  {
    if(millis() > flashTime + 100)
    {
      switch(flashSequence[flashIndex])
      {
        case 1:
          digitalWrite(BLUE_LED, HIGH);
          digitalWrite(BLUE2_LED, LOW);
          break;

        case 2:
          digitalWrite(BLUE_LED, LOW);
          digitalWrite(BLUE2_LED, HIGH);
          break;

        case 0:
          digitalWrite(BLUE_LED, LOW);
          digitalWrite(BLUE2_LED, LOW);
          break;

        default:
          break;
      }
      if( flashIndex < (sizeof(flashSequence)/sizeof(int))-1 ) {
        flashIndex++;
      } else {
        flashIndex = 0;
      }
      flashTime = millis();
    }
  }
  else
  {
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(BLUE2_LED, LOW);
    flashIndex = 0;
  }
}


enum states {
  wait_for_button,
  red,
  red_yellow,  
  wait_for_green,
  green,
  yellow,
  wait_for_red,
};

void trafficLight() {
  static states state = wait_for_button;
  static unsigned long ts = 0;

  switch ( state )
  {
    case wait_for_button:
      if (digitalRead(TRAFFIC_BUTTON) == BUTTON_PRESSED) {
        if( digitalRead(RED_LED) == HIGH )
        {
          state = red_yellow;
        }
        else
        {
          state = yellow;
        }
        ts = millis();
      }
      break;

    case red_yellow:
      digitalWrite(RED_LED, HIGH);
      digitalWrite(YELLOW_LED, HIGH);
      state = wait_for_green;
      break;      

    case wait_for_green:
      if(millis() > ts + 1000)
      {
       state = green;
      }
      break;
      
    case green:
      digitalWrite(RED_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      state = wait_for_button;
      break;

    case yellow:
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(YELLOW_LED, HIGH);
      state = wait_for_red;
      break;
      
    case wait_for_red:  
      if(millis() > ts + 2000)
      {
        state = red;
      }
      break;
      
    case red:        
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      state = wait_for_button;
      break;

    default:
      break;
  }
}

