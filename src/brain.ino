#define rxPin 4
#define txPin 5

int val = 0;
int start = 0;
int start2 = 0;
int end = 0;
int end2 = 0;
int loopCount = 0;
int in_message = 0;

int bit_counter = 0;
int message_start = 0;
int message_end = 0;

void setup() {
  Serial.begin(9600);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  digitalWrite(txPin, LOW);
}

void write_one() {
  delayMicroseconds(10);
  digitalWrite(txPin, HIGH);
  delayMicroseconds(400);
  digitalWrite(txPin, LOW);
  delayMicroseconds(90);
}

void write_zero() {
  delayMicroseconds(10);
  digitalWrite(txPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(txPin, LOW);
  delayMicroseconds(390);
}

void write_nothing() {
  delay(20);
  Serial.println("");
}

void send_message(char *message) {
  Serial.println("start of message");
  for(unsigned int i = 0; i < strlen(message); i++) {
    char bit = message[i];
    if(bit == '0') {
      write_zero();
    } else if(bit == '1') {
      write_one();
    } else if(bit == '|') {
      write_nothing();
    }
  }
  Serial.println("\nend of message");
}

void listen(double delay_microseconds) {
  unsigned int t = micros();
  unsigned int loopCount = 0;
  unsigned int start = 0;
  unsigned int end = 0;

  Serial.print("listening for ");
  Serial.println(delay_microseconds);

  while(micros() < t + delay_microseconds) {
    int val = digitalRead(rxPin);
    loopCount++;

    if(val == 0 && start == 0) {
      start = micros();
    } else if (val == 1 && start !=0) {
      end = micros();
    }

    in_message = loopCount < 100;

    if (start != 0 && end != 0) {
      if (in_message) {
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

  Serial.println("done listening");
}

void loop() {
  // ************************** WRITE
  // delay(2000);
  // listen(100000);
  // char message1[] = "1000011010011|100001101001000000100|";
  // send_message(message1);
  // listen(29000000);
  // // delayMicroseconds(29000); // 58 characters * 500 micros for 3rd radio message
  // char message2[] = "|10000110100100010";
  // send_message(message2);
  // delay(2000000);

  // ************************** READ
  val = digitalRead(rxPin);
  loopCount++;

  if(val == 0 && start == 0) {
    start = micros();
  } else if (val == 1 && start !=0) {
    end = micros();
  }

  in_message = loopCount < 1000;

  if (start != 0 && end != 0) {
    if (in_message) {
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

  // ************ trying to calculate distance between bits
  // val = digitalRead(rxPin);
  //
  // if(val == 0 && start == 0) {
  //   start = micros();
  // } else if (val == 1 && start !=0) {
  //   end = micros();
  // }
  //
  // if(val == 1 && start2 == 0) {
  //   start2 = micros();
  // } else if (val == 0 && start2 !=0) {
  //   end2 = micros();
  // }
  //
  // if(start != 0 && end != 0 && start2 != 0 && end2 != 0) {
  //   int up = end - start;
  //   int down = end2 - start2;
  //   Serial.print("Up: ");
  //   Serial.println(up);
  //   Serial.print("Down: ");
  //   Serial.println(down);
  //   Serial.print("Total: ");
  //   Serial.println(up + down);
  //
  //   start2 = 0;
  //   end2 = 0;
  //   start = 0;
  //   end = 0;
  // }

  // Divide total message time by number of bits to get micros per bit
  //
  // val = digitalRead(rxPin);
  // loopCount++;
  //
  // if(val == 0 && start == 0) {
  //   start = micros();
  // } else if (val == 1 && start !=0) {
  //   end = micros();
  // }
  //
  // in_message = loopCount < 100;
  //
  // if (start != 0 && end != 0) {
  //   if (in_message) {
  //     if ((end - start) < 200) {
  //       // Serial.print(0);
  //     } else {
  //       // Serial.print(1);
  //     }
  //     bit_counter++;
  //     message_end = micros();
  //   } else {
  //     Serial.println("");
  //
  //     if(message_start == 0) {
  //       message_start = micros();
  //     } else {
  //       Serial.print("microseconds per bit: ");
  //       Serial.println((message_end - message_start) / bit_counter);
  //       // Serial.print("total: ");
  //       // Serial.println(message_end - message_start);
  //       // Serial.print("bit counter: ");
  //       // Serial.println(bit_counter);
  //       bit_counter = 0;
  //       message_start = 0;
  //       message_end = 0;
  //     }
  //   }
  //
  //   loopCount = 0;
  //   start = 0;
  //   end = 0;
  // }
}
