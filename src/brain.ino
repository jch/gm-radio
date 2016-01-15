#define txPin 2
#define rxPin 3
#define DEFAULT_STATE true

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

  Serial.print("> ");
  Serial.println(msg);
}

String listen_for_message() {
  // pulse_start and pulse_end will either be 0 or > 0 and store microseconds
  unsigned long pulse_start = 0;
  unsigned long pulse_end = 0;
  // listen_start is a milliseconds timestamp used in a timeout mechanism to
  //  exit listen_for_message after 1 second if no message is received
  unsigned long listen_start = millis();
  // expected_micros_to_next_pulse is set after each bit is recorded and is
  // helpful for determining when a message is over
  int expected_micros_to_next_pulse = 1000;
  // msg is updated with each bit and then returned when the message is complete
  String msg = "< ";
  // signal_state is the value of the signal, the default is HIGH, or true, and
  // a pulse starts when signal_state is false and ends when signal_state is
  // true again
  boolean signal_state = DEFAULT_STATE;
  // keep_listening controls the loop in this function and is set to false when
  // the message being received is complete
  boolean keep_listening = true;

  // start a loop and keep looping until keep_listening is false or 1 second has
  // passed since this function was called
  while(keep_listening && millis() - listen_start < 1000) {
    // get the state of the signal, true or false
    signal_state = digitalRead(rxPin);

    // if the signal is the default state do this stuff
    if(signal_state == DEFAULT_STATE) {
      // if pulse_start has been set to a microseconds timestamp
      if(pulse_start > 0) {
        // get the width of the pulse in microseconds by subtracting the
        // pulse_start timestamp from the current microseconds timestamp
        int pulse_width = micros() - pulse_start;

        // if the pulse_width is between 50 and 120 microseconds
        if(pulse_width >= 50 && pulse_width <= 120) {
          // record a 0
          msg.concat("0");
          // set pulse_end to the current microseconds timestamp
          pulse_end = micros();
          // update expected_micros_to_next_pulse to the largest expected value
          expected_micros_to_next_pulse = 960;
        // if the pulse width is between 500 and 700 microseconds
        } else if(pulse_width > 500 && pulse_width < 700) {
          // record a 1
          msg.concat("1");
          // set pulse_end to the current microseconds timestamp
          pulse_end = micros();
          // update expected_micros_to_next_pulse to the largest expected value
          expected_micros_to_next_pulse = 510;
        // if the pulse is not a 0 or 1 send a debug message to Serial so we can
        // go WTF
        } else {
          Serial.print("what is this? ");
          Serial.println(pulse_width);
        }

        // reset pulse_start to 0 so the next pulse can be recorded
        pulse_start = 0;

      // if pulse_end isn't 0 and the current microseconds timestamp - pulse_end
      // is greater than our our expected_micros_to_next_pulse then the message
      // must be over so do these things
      } else if(pulse_end != 0 && micros() - pulse_end > expected_micros_to_next_pulse) {
        // print the message to serial so we can see it
        Serial.println(msg);
        // set keep_listening to false so that the function can end
        keep_listening = false;
      }
    // if the signal_state is not in the default state and pulse_start has not
    // been updated yet do this stuff
    } else if(pulse_start == 0) {
      // set pulse_start to the microseconds timestamp
      pulse_start = micros();
    }
  }

  // return that string!
  return msg;
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
