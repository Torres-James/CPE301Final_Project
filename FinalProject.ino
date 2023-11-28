#include <LiquidCrystal.h>
#include <Stepper.h>
#include <dht.h>     //humidity and temp library
#include <RTClib.h>  //clock library

//lkl

//definitions
#define RDA 0x80
#define TBE 0x20
//stepper motor
const int stepsPerRevolution = 2038;
#define stepperPin1 8
#define stepperPin2 10
#define stepperPin3 9
#define stepperPin4 11
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
//fan motor
#define MOTOR_PIN 6
//humidity and temp sensor
dht DHT;
#define TEMP_THRESHOLD 26
#define WATER_THRESHOLD 900
#define DHT11_PIN 7
//LED pins
#define RED_LED 3
#define YELLOW_LED 4
#define GREEN_LED 5
#define BLUE_LED 6


volatile unsigned char* port_b = (unsigned char*)0x25;
volatile unsigned char* ddr_b = (unsigned char*)0x24;
volatile unsigned char* pin_b = (unsigned char*)0x23;
volatile unsigned char* port_k = (unsigned char*)0x108;
volatile unsigned char* ddr_k = (unsigned char*)0x107;
volatile unsigned char* pin_k = (unsigned char*)0x106;
volatile unsigned char* port_e = (unsigned char*)0x2E;
volatile unsigned char* ddr_e = (unsigned char*)0x2D;
volatile unsigned char* pin_e = (unsigned char*)0x2C;
volatile unsigned char* port_d = (unsigned char*)0x2B;
volatile unsigned char* ddr_d = (unsigned char*)0x2A;
volatile unsigned char* pin_d = (unsigned char*)0x29;
volatile unsigned char* port_h = (unsigned char*)0x102;
volatile unsigned char* ddr_h = (unsigned char*)0x101;
volatile unsigned char* pin_h = (unsigned char*)0x100;
volatile unsigned char* my_ADMUX = (unsigned char*)0x7C;
volatile unsigned char* my_ADCSRB = (unsigned char*)0x7B;
volatile unsigned char* my_ADCSRA = (unsigned char*)0x7A;
volatile unsigned int* my_ADC_DATA = (unsigned int*)0x78;
volatile unsigned char* portADCDataRegisterHigh = (unsigned char*)0x79;
volatile unsigned char* myUCSR0A = (unsigned char*)0x00C0;
volatile unsigned char* myUCSR0B = (unsigned char*)0x00C1;
volatile unsigned char* myUCSR0C = (unsigned char*)0x00C2;
volatile unsigned int* myUBRR0 = (unsigned int*)0x00C4;
volatile unsigned char* myUDR0 = (unsigned char*)0x00C6;
volatile unsigned char* myTCCR1A = (unsigned char*)0x80;
volatile unsigned char* myTCCR1B = (unsigned char*)0x81;
volatile unsigned char* myTCCR1C = (unsigned char*)0x82;
volatile unsigned char* myTIMSK1 = (unsigned char*)0x6F;
volatile unsigned int* myTCNT1 = (unsigned int*)0x84;
volatile unsigned char* myTIFR1 = (unsigned char*)0x36;

// rtc clock
// RTC_DS1307 RTC;
const int RS = 31, EN = 33, D4 = 35, D5 = 37, D6 = 39, D7 = 41;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

//posible states
enum states { START,
              DISABLED,
              IDLE,
              ERROR,
              RUNNING };
void setup() {
  // Serial.begin(9600);
  adc_init();
  // Serial.begin(9600);
  setPortOutput(ddr_b, 7);
  lcd.begin(16, 2);
}

void loop() {
  // updateLCD();
  adc_init();
  int waterLevel = getWaterLevel();
  // Serial.print("\n");
  // Serial.println(waterLevel);
  // delay(1000);
  //  WRITE_LOW_P(port_b, 7);
  //  delay(100);
  //  WRITE_HIGH_P(port_b,7);
  //  delay(100);
}
void updateLCD() {
  int chk = DHT.read11(DHT11_PIN);
  float f = (DHT.temperature * (9 / 5)) + 32;
  lcd.setCursor(0, 0);
  lcd.print("Temp = ");
  lcd.print(f);
  lcd.print(" F");
  Serial.println(DHT.temperature);
  lcd.setCursor(0, 1);
  lcd.print("Humidity = ");
  lcd.print(DHT.humidity);
}
void WRITE_LOW_P(volatile unsigned char* port, unsigned char pin_num) {
  *port &= ~(0x01 << pin_num);
}
void WRITE_HIGH_P(volatile unsigned char* port, unsigned char pin_num) {

  *port |= (0x01 << pin_num);
}
void setPortOutput(volatile unsigned char* ddr, unsigned char pin_num) {
  *ddr |= 0x01 << pin_num;
}
void setPortInput(volatile unsigned char* ddr, unsigned char pin_num) {
  *ddr &= ~(0x01 << pin_num);
}

void U0init(int U0baud) {
  unsigned long FCPU = 16000000;
  unsigned int tbaud;
  tbaud = (FCPU / 16 / U0baud - 1);
  // Same as (FCPU / (16 * U0baud)) - 1;
  *myUCSR0A = 0x20;
  *myUCSR0B = 0x18;
  *myUCSR0C = 0x06;
  *myUBRR0 = tbaud;
}
unsigned char U0kbhit() {
  return *myUCSR0A & RDA;
}
unsigned char U0getchar() {
  return *myUDR0;
}
void U0putchar(unsigned char U0pdata) {
  while ((*myUCSR0A & TBE) == 0)
    ;
  *myUDR0 = U0pdata;
}
void adc_init() {
  // setup the A register
  *my_ADCSRA |= 0b10000000;  // set bit   7 to 1 to enable the ADC
  *my_ADCSRA &= 0b11011111;  // clear bit 6 to 0 to disable the ADC trigger mode
  *my_ADCSRA &= 0b11110111;  // clear bit 5 to 0 to disable the ADC interrupt
  *my_ADCSRA &= 0b11111000;  // clear bit 0-2 to 0 to set prescaler selection to slow reading
  // setup the B register
  *my_ADCSRB &= 0b11110111;  // clear bit 3 to 0 to reset the channel and gain bits
  *my_ADCSRB &= 0b11111000;  // clear bit 2-0 to 0 to set free running mode
  // setup the MUX Register
  *my_ADMUX &= 0b01111111;  // clear bit 7 to 0 for AVCC analog reference
  *my_ADMUX |= 0b01000000;  // set bit   6 to 1 for AVCC analog reference
  *my_ADMUX &= 0b11011111;  // clear bit 5 to 0 for right adjust result
  *my_ADMUX &= 0b11100000;  // clear bit 4-0 to 0 to reset the channel and gain bits
}

unsigned int adc_read(unsigned char adc_channel_num) {
  // clear the channel selection bits (MUX 4:0)
  *my_ADMUX &= 0b11100000;
  // clear the channel selection bits (MUX 5)
  *my_ADCSRB &= 0b11110111;
  // set the channel number
  if (adc_channel_num > 7) {
    // set the channel selection bits, but remove the most significant bit (bit 3)
    adc_channel_num -= 8;
    // set MUX bit 5
    *my_ADCSRB |= 0b00001000;
  }
  // set the channel selection bits
  *my_ADMUX += adc_channel_num;
  // set bit 6 of ADCSRA to 1 to start a conversion
  *my_ADCSRA |= 0x40;
  // wait for the conversion to complete
  while ((*my_ADCSRA & 0x40) != 0)
    ;
  // return the result in the ADC data register
  return *my_ADC_DATA;
}
void configTimer(unsigned int freq) {

  // calc period
  double period = 1.0 / double(freq);
  // 50% duty cycle
  double half_period = period / 2.0f;
  // clock period def
  double clk_period = 0.0000000625;
  // calc ticks
  unsigned int ticks = half_period / clk_period;
  // stop the timer
  *myTCCR1B &= 0xF8;
  // set the counts
  *myTCNT1 = (unsigned int)(65536 - ticks);
  // start the timer
  *myTCCR1A = 0x0;
  *myTCCR1B |= 0b00000001;
  // wait for overflow
  while ((*myTIFR1 & 0x01) == 0)
    ;  // 0b 0000 0000
  // stop the timer
  *myTCCR1B &= 0xF8;  // 0b 0000 0000
  // reset TOV
  *myTIFR1 |= 0x01;
}
int getWaterLevel() {
  unsigned char channel = 0;
  unsigned int value_b;

  value_b = adc_read(channel);
  int thousand, hundred, ten, one;
  thousand = (value_b / 1000) % 10 + '0';
  hundred = (value_b / 100) % 10 + '0';
  ten = (value_b / 10) % 10 + '0';
  one = (value_b / 1) % 10 + '0';
  // U0putchar(thousand);
  // U0putchar(hundred);
  // U0putchar(ten);
  // U0putchar(one);
  // U0putchar("\n");
  return ((thousand - 48) * 1000) + ((hundred - 48) * 100) + ((ten - 48) * 10) + (one - 48);
}
void DisabledState() {
}

void IdleState() {
}

void RunningState() {
}

void ErrorState() {
}



// turn fan motor on or off
void setFanMotor(bool on) {
  if (on) {
    WRITE_HIGH_P(port_h, MOTOR_PIN);
  } else {
    WRITE_LOW_P(port_h, MOTOR_PIN);
  }
}
