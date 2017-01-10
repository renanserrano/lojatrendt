#include <Servo.h>
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  myservo1.attach(3);
  myservo2.attach(5);
  myservo3.attach(6);
  myservo4.attach(9);
  myservo5.attach(10);
  myservo6.attach(11);
  myservo1.write(10);
  myservo2.write(10);
  myservo3.write(10);
  myservo4.write(10);
  myservo5.write(10);
  myservo6.write(10);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    inputString.trim();
    // Serial.println(inputString);
    // Serial.println(inputString.length());
    if (inputString == "111111") {
      // Serial.println("gotcha1");
      myservo1.write(150);
      
    } else if (inputString == "222222") {
      myservo2.write(150);

    }  else if (inputString == "333333") {
      myservo3.write(150);

    }  else if (inputString == "444444") {
      myservo4.write(150);

    }  else if (inputString == "555555") {
      myservo5.write(150);

    }  else if (inputString == "666666") {
      myservo6.write(150);

    } else if (inputString == "fecharportas") {
      myservo1.write(10);
      myservo2.write(10);
      myservo3.write(10);
      myservo4.write(10);
      myservo5.write(10);
      myservo6.write(10);
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
  hardware serial RX.  This routine is run between each
  time loop() runs, so using delay inside loop can delay
  response.  Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


