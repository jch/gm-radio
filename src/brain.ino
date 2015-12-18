#define rxPin 4
#define offState 1
#define onState 0

int val = 0;
double bit_start_timestamp = 0;
double bit_end_timestamp = 0;
double message_start_timestamp = 0;
int bit_counter = 0;
double last_bit_timestamp = 0;

void setup() {
  Serial.begin(9600);
  pinMode(rxPin, INPUT);
}

boolean is_bit_start() {
  return val == onState && bit_start_timestamp == 0;
}

boolean is_bit_end() {
  return val == offState && bit_start_timestamp !=0;
}

boolean is_bit_complete() {
  return val == offState && bit_start_timestamp != 0 && bit_end_timestamp != 0;
}

boolean is_zero() {
  return (bit_end_timestamp - bit_start_timestamp) < 200;
}

boolean is_one() {
  return (bit_end_timestamp - bit_start_timestamp) > 200;
}

boolean is_message_boundary() {
  return last_bit_timestamp != 0 && (micros() - last_bit_timestamp) > 5000;
}

void loop() {
  val = digitalRead(rxPin);

  if(is_bit_start()) {
    if (is_message_boundary()) {
      Serial.print(" ");
      Serial.print((last_bit_timestamp - message_start_timestamp) / bit_counter);

      bit_counter = 0;
      message_start_timestamp = micros();

      Serial.println();
      last_bit_timestamp = 0;
    }
    bit_start_timestamp = micros();
  } else if (is_bit_end()) {
    bit_end_timestamp = micros();
  }

  if (is_bit_complete()) {
    if (is_zero()) {
      Serial.print(0);
    } else if (is_one()) {
      Serial.print(1);
    }

    bit_start_timestamp = 0;
    bit_end_timestamp = 0;
    last_bit_timestamp = micros();
    bit_counter++;
  }
}
