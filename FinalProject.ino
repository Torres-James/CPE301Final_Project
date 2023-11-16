#define WRITE_HIGH_PE(pin_num) *port_e |= (0x01 << pin_num);
#define WRITE_LOW_PE(pin_num) *port_e &= ~(0x01 << pin_num);
#define WRITE_HIGH_PD(pin_num) *port_d |= (0x01 << pin_num);
#define WRITE_LOW_PD(pin_num) *port_d &= ~(0x01 << pin_num);
#define WRITE_HIGH_PK(pin_num) *port_k |= (0x01 << pin_num);
#define WRITE_LOW_PK(pin_num) *port_k &= ~(0x01 << pin_num);
#define WRITE_HIGH_PH(pin_num) *port_h |= (0x01 << pin_num);
#define WRITE_LOW_PH(pin_num) *port_h &= ~(0x01 << pin_num);

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

void setup(){

}

void loop(){
 // ff
}
void set_PE_as_output(unsigned char pin_num) {
  *ddr_e |= 0x01 << pin_num;
}
void set_PD_as_output(unsigned char pin_num) {
  *ddr_d |= 0x01 << pin_num;
}
void set_PK_as_output(unsigned char pin_num) {
  *ddr_k |= 0x01 << pin_num;
}
void set_PH_as_output(unsigned char pin_num) {
  *ddr_h |= 0x01 << pin_num;
}
