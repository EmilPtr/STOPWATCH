#define DP_4 A0
#define CP_4 A1
#define LP_4 A2

#define DP_2 9
#define CP_2 8
#define LP_2 7

#define DIG_SEC_1 A5
#define DIG_SEC_2 A4

#define DIG_MSEC_1 10
#define DIG_MSEC_2 11

#define DIG_MIN_1 12
#define DIG_MIN_2 13

#define START 2
#define STOP 3
#define RESET 4

unsigned long offset = 0;
unsigned long prevMillis = 0;

bool counting = true;

const uint8_t digitMap[10] = {
  0b11000000,  // 0
  0b11111001,  // 1
  0b10100100,  // 2
  0b10110000,  // 3
  0b10011001,  // 4
  0b10010010,  // 5
  0b10000010,  // 6
  0b11111000,  // 7
  0b10000000,  // 8
  0b10010000   // 9
};

void writeHC595(uint8_t data, int dataPin, int clockPin, int latchPin) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);
}

void setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  
}

void loop() {
  unsigned long millisec = millis()-offset;
  int mins = millisec / 60000;
  int secs = (millisec % 60000) / 1000;
  int msec = (millisec % 1000) / 10; // FIX: 0–99 instead of 0–999

  char msecstr[3]; 
  char secsstr[3]; 
  char minsstr[3];

  sprintf(msecstr, "%02d", msec);
  sprintf(secsstr, "%02d", secs);
  sprintf(minsstr, "%02d", mins);

  // ---- MSEC 1 ----
  writeHC595(digitMap[msecstr[0]-'0'], DP_2, CP_2, LP_2);
  digitalWrite(DIG_MSEC_1, HIGH);
  delay(2);
  digitalWrite(DIG_MSEC_1, LOW);

  // ---- MSEC 2 ----
  writeHC595(digitMap[msecstr[1]-'0'], DP_2, CP_2, LP_2);
  digitalWrite(DIG_MSEC_2, HIGH);
  delay(2);
  digitalWrite(DIG_MSEC_2, LOW);

  // ---- SEC 1 ----
  writeHC595(digitMap[secsstr[0]-'0'], DP_4, CP_4, LP_4);
  digitalWrite(DIG_SEC_1, HIGH);
  delay(2);
  digitalWrite(DIG_SEC_1, LOW);

  // ---- SEC 2 ----
  writeHC595(digitMap[secsstr[1]-'0'], DP_4, CP_4, LP_4);
  digitalWrite(DIG_SEC_2, HIGH);
  delay(2);
  digitalWrite(DIG_SEC_2, LOW);

  // ---- MIN 1 ----
  writeHC595(digitMap[minsstr[0]-'0'], DP_4, CP_4, LP_4);
  digitalWrite(DIG_MIN_1, HIGH);
  delay(2);
  digitalWrite(DIG_MIN_1, LOW);

  // ---- MIN 2 ----
  writeHC595(digitMap[minsstr[1]-'0'], DP_4, CP_4, LP_4);
  digitalWrite(DIG_MIN_2, HIGH);
  delay(2);
  digitalWrite(DIG_MIN_2, LOW);

  if (digitalRead(STOP) && counting) {
    counting = false;
  }

  if (digitalRead(START) && !counting) {
    counting = true;
  }

  if (!counting) {
    offset += millis()-prevMillis;
  }
  
  if (digitalRead(RESET)) {
    offset = millis();
  }

  prevMillis = millis();
}
