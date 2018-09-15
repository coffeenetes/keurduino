char command;
bool newCommand = false;

const byte heaterStatusPin = A0;
const byte powerStatusPin = A1;
const byte waterStatusPin = A2;
const int powerButtonPin = 3;
const int smallCupButtonPin = 4;
const int medCupButtonPin = 5;
const int largeCupButtonPin = 6;
const bool ledOn = true;
const bool ledOff = false;


void setup() {
    Serial.begin(9600);
    Serial.println("~::Keurig Debugger Ready::~");
}

void loop() {
    recvOneChar();
    actions();
}

void recvOneChar() {
    if (Serial.available() > 0) {
        command = Serial.read();
        newCommand = true;
    }
}

void actions() {
    if (newData == true) {
        switch (receivedChar) {
            case 'h': {
                bool heaterStatus = Status(heaterStatusPin, ledOff);
                Serial.print("Heater Status: ");
                if (heaterStatus) {
                    Serial.println("Off");
                } else {
                    Serial.println("Heating");
                }
                break;
            }
            case 'p': {
                bool powerStatus = Status(powerStatusPin, ledOn);
                Serial.print("Power Status: ");
                if (powerStatus) {
                    Serial.println("On");
                } else {
                    Serial.println("Off");
                }
                break;
            }
            case 'w': {
                bool waterStatus = Status(waterStatusPin, ledOff);
                bool powerStatus = Status(powerStatusPin, ledOn);
                Serial.print("Water Status: ");
                if (powerStatus) {
                    if (waterStatus) {
                        Serial.println("Sufficient");
                    } else {
                        Serial.println("Add Water");
                    }
                } else {
                    Serial.println("Device Off");
                }
                break;
            }
            case 'o': {
                if (!Status(powerButtonPin, ledOn)) {
                    Serial.println("Powering on Keurig");
                    pressButton(powerButtonPin);
                } else {
                    Serial.println("Powering off Keurig");
                    pressButton(powerButtonPin);
                }
                break;
            }
            case 's': {
                Serial.println("Attempting to Brew Small Coffee");
                if (!ReadyToBrew) {
                    Serial.println("Keurig Not Ready");
                } else {
                    Serial.println("Brewing Small Cup.");
                    pressButton(smallCupButtonPin);
                }
                break;
            }
            case 'm': {
                Serial.println("Attempting to Brew Med Coffee");
                if (!ReadyToBrew) {
                    Serial.println("Keurig Not Ready");
                } else {
                    Serial.println("Brewing Med Cup.");
                    pressButton(medCupButtonPin);
                }
                break;
            }
            case 'l': {
                Serial.println("Attempting to Brew Large Coffee");
                if (!ReadyToBrew) {
                    Serial.println("Keurig Not Ready");
                } else {
                    Serial.println("Brewing Large Cup.");
                    pressButton(largeCupButtonPin);
                }
                break;
            }   
        }
        newCommand = false;
    }
}

// takes pin and condition e.g. if desired state is ledOn, pass ledOn
bool Status(int pin, bool desiredState) {
     // Not SUPER accurate, but gets the job done for ADC math in this case.
     // keurig power to leds is somewhere between 4-5V, thus adc < 4 when on
    int rawInput = analogRead(pin);
    float adc = (rawInput * (5.0/1023.0));
    //Serial.print("Analog Read Value: ");
    //Serial.print(rawInput);
    //Serial.print("| Voltage: ");
    //Serial.println(adc);
    if (adc < 4.0) {
        if (desiredState == ledOn) {
            return true;
        } else {
            return false;
        }
    } else {
        if (desiredState == ledOff) {
            return true;
        } else {
            return false;
        }       
    } 
}

// Simply checks if all states are good and coffee can be brewed
bool ReadyToBrew() {
    if (Status(powerStatusPin, ledOn) && 
        Status(heaterStatusPin, ledOff) && 
        Status(waterStatusPin, ledOff)) {
        return true;
    } else {
        return false;
    }
}

// quick pulse to simulate button press
void pressButton(int pin) {
    digitalWrite(pin, HIGH);
    delay(100);
    digitalWrite(pin, LOW);
}