#include <LiquidCrystal_I2C.h>

#define RXD2 16
#define TXD2 17
#define BUTTON_PIN 15

LiquidCrystal_I2C lcd(0x27, 16, 2);
HardwareSerial sim800(2);

String phoneNumber = "+2348026331296";

void setup() {
  Serial.begin(115200);

  sim800.begin(9600, SERIAL_8N1, RXD2, TXD2);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("GSM Intercom");
  lcd.setCursor(0,1);
  lcd.print("Initializing...");

  delay(3000);

  sendCommand("AT");
  sendCommand("ATE0");  // Echo off

  lcd.clear();
  lcd.print("System Ready");
}

void loop() {

  // 📞 BUTTON PRESSED → MAKE CALL
  if (digitalRead(BUTTON_PIN) == LOW) {
    lcd.clear();
    lcd.print("Dialing...");

    delay(500);

    sim800.print("ATD");
    sim800.print(phoneNumber);
    sim800.println(";");

    lcd.setCursor(0,1);
    lcd.print("Calling...");

    delay(10000); // call duration

    sendCommand("ATH"); // hang up

    lcd.clear();
    lcd.print("Call Ended");

    delay(2000);

    lcd.clear();
    lcd.print("System Ready");
  }

  // Debug (optional)
  while (sim800.available()) {
    Serial.write(sim800.read());
  }
}

void sendCommand(String cmd) {
  sim800.println(cmd);
  delay(1000);
}