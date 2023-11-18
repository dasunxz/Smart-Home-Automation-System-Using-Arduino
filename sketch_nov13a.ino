#include <DHT.h>
#include <SoftwareSerial.h>

SoftwareSerial sim900A(7, 8);  // RX, TX pins on Arduino

#define DHTPIN 2    // Pin where the DHT11 is connected
#define DHTTYPE DHT11   // DHT 11
#define Fan 4

bool state = true;

DHT dht(DHTPIN, DHTTYPE);
const int potPin = A0;  // Pin connected to the potentiometer

void setup() {
  Serial.begin(9600);

  sim900A.begin(9600);

  delay(2000);
  dht.begin();
  pinMode(Fan,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
}

void loop() {
  int potValue = analogRead(potPin);  // Read the analog value from the potentiometer
  int val = map(potValue,0,1023,0,40);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else if(temperature > val){
    Serial.print("\tFan Is On...!");
    digitalWrite(Fan,HIGH);
    if(state == true){
      sendSMS("+94725815118", "Temprature is Grather than Set Value.Fan is On!");  // Replace with the recipient's phone number
      state = false;
    }
  }else{
    digitalWrite(Fan,LOW);
    state = true;
  }

  Serial.print("Set Temprature Value: ");
  Serial.print(val);

  Serial.print("\t Temperature: ");
  Serial.println(temperature);

  delay(100);  // Add a delay to make the readings more readable


  if (Serial.available() > 0) {
    char data = Serial.read(); // Read the incoming data
    Serial.print("Received: ");
    Serial.println(data);

    if (data == '1'){
      digitalWrite(9,HIGH);
    }
    if(data == '2'){
      digitalWrite(9,LOW);
    }
    if (data == '5'){
      digitalWrite(11,HIGH);
    }
    if(data == '6'){
      digitalWrite(11,LOW);
    }
  }

}

void sendSMS(String phoneNumber, String message) {
  sim900A.println("AT+CMGF=1");  // Set SMS mode to text
  delay(1000);

  sim900A.print("AT+CMGS=\"");
  sim900A.print(phoneNumber);
  sim900A.println("\"");

  delay(1000);

  sim900A.print(message);
  delay(100);
  sim900A.write(26);  // Send Ctrl+Z to indicate the end of the message
  delay(1000);

  Serial.println("SMS Sent!");
  
}

