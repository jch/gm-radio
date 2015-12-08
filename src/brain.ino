#define rxPin 10

int val = 0;
int start = 0;
int end = 0;
double loopCount = 0;

void setup() {
  Serial.begin(9600);
  pinMode(rxPin, INPUT);
}

void loop() {
  val = digitalRead(rxPin);
  loopCount++;

  if(val == 0 && start == 0) {
    start = micros();
  } else if (val == 1 && start !=0) {
    end = micros();
  }

  if (start != 0 && end != 0) {
    if (loopCount < 1000) {
      if ((end - start) < 200) {
        Serial.print(0);
      } else {
        Serial.print(1);
      }
    } else {
      Serial.println("");
    }

    loopCount = 0;
    start = 0;
    end = 0;
  }
}
