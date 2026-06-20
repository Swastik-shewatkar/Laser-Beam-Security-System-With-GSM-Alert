#define RXD2 16
#define TXD2 17

int relay = 26;
int ldr   = 34;

const int threshold = 6;
const unsigned long relayTime = 20000;

bool relayRunning = false;
bool beamWasCut = false;

unsigned long startTime = 0;

String phoneNumber = "+91xxxxxxxxxx";

void makeCall() {

  Serial.println("Calling...");

  Serial2.print("ATD");
  Serial2.print(phoneNumber);
  Serial2.println(";");

}

void setup() {

  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  pinMode(relay, OUTPUT);
  pinMode(ldr, INPUT);

  digitalWrite(relay, HIGH);   // Relay OFF (active LOW)

  Serial.println("System Ready");

  delay(5000);

  Serial2.println("AT");
  delay(1000);

}

void loop() {

  int ldrValue = analogRead(ldr);
  Serial.println(ldrValue);

  // Detect beam cut
  if (ldrValue >= threshold && !beamWasCut) {

    beamWasCut = true;
    relayRunning = true;
    startTime = millis();

    digitalWrite(relay, LOW);   // Relay ON

    Serial.println("Beam Cut - Alarm ON");

    makeCall();                 // GSM Call
  }

  // Turn OFF relay after time
  if (relayRunning && millis() - startTime >= relayTime) {

    digitalWrite(relay, HIGH);  // Relay OFF
    relayRunning = false;

    Serial2.println("ATH");     // Hang up call

    Serial.println("Alarm OFF");
  }

  // Reset when beam restored
  if (ldrValue <= 1 && !relayRunning) {
    beamWasCut = false;
  }

  delay(50);
}
