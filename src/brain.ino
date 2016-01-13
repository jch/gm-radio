#define txPin 2
#define rxPin 3

String response;

void write_one() {
  digitalWrite(txPin, HIGH);
  delayMicroseconds(648);
  digitalWrite(txPin, LOW);
  delayMicroseconds(352);
}

void write_zero() {
  digitalWrite(txPin, HIGH);
  delayMicroseconds(92);
  digitalWrite(txPin, LOW);
  delayMicroseconds(908);
}

void send_message(char *msg) {
  for(unsigned int i = 0; i < strlen(msg); i++) {
    char bit = msg[i];

    if(bit == '0') {
      write_zero();
    } else if(bit == '1') {
      write_one();
    }
  }
}

String listen_for_message() {
  unsigned long pulse_start = 0;
  unsigned long pulse_end = 0;
  unsigned long listen_start = millis();
  int expected_micros_to_next_pulse = 1000;
  String message = "";
  boolean input = true;
  boolean keep_listening = true;

  while(keep_listening && millis() - listen_start < 1000) {
    input = digitalRead(rxPin);

    if(input) {
      if(pulse_start > 0) {
        int pulse_width = micros() - pulse_start;

        if(pulse_width >= 50 && pulse_width <= 120) {
          message.concat("0");
          pulse_end = micros();
          expected_micros_to_next_pulse = 960;
        } else if(pulse_width > 500 && pulse_width < 700) {
          message.concat("1");
          pulse_end = micros();
          expected_micros_to_next_pulse = 510;
        } else {
          Serial.print("what is this? ");
          Serial.println(pulse_width);
        }

        pulse_start = 0;

      } else if(pulse_end != 0 && micros() - pulse_end > expected_micros_to_next_pulse) {
        Serial.println(message);
        pulse_end = 0;
        keep_listening = false;
      }
    } else if(pulse_start == 0) {
      pulse_start = micros();
    }
  }

  return message;
}

void setup() {
  Serial.begin(9600);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  digitalWrite(txPin, LOW);

  // delay(4000);
  //
  // char cd_changer_hello[] = "1100001101001000000100";
  // String head_response1 = "111011110100110010";
  // String head_response2 = "111001110100110011";
  // String ideal_head_response = "111001101110000100";
  // char yeah[] = "110000110100000011";
  // boolean unmatched = true;
  //
  // send_message(cd_changer_hello);
  //
  // while(unmatched) {
  //   response = listen_for_message();
  //   Serial.println(response);
  //
  //   if(response == head_response1) {
  //     unmatched = false;
  //     send_message(cd_changer_hello);
  //   } else if(response == head_response2) {
  //     unmatched = false;
  //     send_message(cd_changer_hello);
  //   } else if(response == ideal_head_response) {
  //     unmatched = false;
  //     send_message(yeah);
  //   }
  // }
}

void loop() {
  char three[] = "10100011010010010110";
  send_message(three);
  delay(1000);
}
