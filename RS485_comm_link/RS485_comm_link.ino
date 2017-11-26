/*
 * Nov 25 2017
 * 
 * RS485 Adapter connected to Pro-Mini pins:
 * RO --------> RXI PIN 1
 * DE = DI ---> D8 IO Pin 8
 * DI <-------- TXO-PIN 0
 * 
 * RS485 Adapter connected to Teensy 3.x pins:
 * RO --------> RXI PIN 9
 * DE = DI ---> D8 IO Pin 11
 * DI <-------- TXO-PIN 10
 * 
 */
#define MODE 0
#define ID_H '0'
#define ID_L '1'

#if MODE == 0
//teensy 3.6
  #define MASTER
  #define CHAR_TIME 11
#endif
#if MODE == 1
//arduino nano
  #define NODE
  #define CHAR_TIME 7
#endif

#define BAUD_USART 115200
#define BAUD_RS485 115200//1000000
#define RS485_READ 0
#define RS485_WRITE 1


#ifdef MASTER //*****************************************************************
#define RS485RX_PIN 9
#define RS485TX_PIN 10
#define RS485DIR_PIN 11

char c = 0;   // for incoming serial data
unsigned long sendTime; //rs485 sent period timeout
bool sendActive = false;
#define IN_BUFFER_SIZE 10 //required size
byte bufIn[IN_BUFFER_SIZE]; //rs485 output buffer
int posIn = 0; //rs485 buffer index
#define NUM_SENSORS 16
#define SENSOR_POINTS 80
int scanData[NUM_SENSORS][SENSOR_POINTS];
#define RS_BUFFER_SIZE 10 //required size
byte bufRs[RS_BUFFER_SIZE]; //rs485 input buffer
int posRs = 0; //rs485 buffer index

void setup() {
  // initialize serial with RS485 transceiver
  Serial.begin(BAUD_USART);
  Serial2.begin(BAUD_RS485); 
  pinMode(RS485DIR_PIN,OUTPUT);
  digitalWrite(RS485DIR_PIN, RS485_READ);
}

void loop() {
  /**
  * Master message processing 
  * 2(start char)
  * Message format {2, CMD, ID_H, ID_L, 3)
  * CMD's: R(read) P(ping) 
  * ID number to respond to
  * 3(end char)
  **/
  // send data only when you receive data:
  while ( Serial.available() ) {
    // read the incoming character
    c = Serial.read();
    if (c == '\n'){
      switch(bufIn[0]) {
        case 'p':
        case 'P':
        Serial2.print(2);
        Serial2.print('P');
        Serial2.print(bufIn[1]);
        Serial2.print(bufIn[2]);
        Serial2.print(3);
        break;
        case 'r':
        case 'R':
        Serial2.print(2);
        Serial2.print('R');
        Serial2.print(bufIn[1]);
        Serial2.print(bufIn[2]);
        Serial2.print(3);
        break;
      }
      posIn = 0; 
    } else {
      bufIn[posIn++] = c;
    }
  }
  if (Serial2.available() > 0) {
    // read the incoming character
    c = Serial.read();
    if (c == 0x3){
      switch(bufRs[1]) {
        case 
      }
    } else {
      bufIn[posRs++] = c;
    }
    Serial.print(c);
  }
}
       
#endif

#ifdef NODE //*****************************************************************
#define RS485DIR_PIN 8
#include <SoftwareSerial.h>

//sensor data path
SoftwareSerial PB9(3, 2); // RX, TX

char c = 0;   // for incoming serial data
unsigned long sendTime; //rs485 sent period timeout
bool sendActive = false;
//must ask for each sample from sensor
#define DATA_TIMEOUT 150
unsigned long dataTime; //data request timeout,
pbd6
byte buf[350]; //sensor input buffer
int pos = 0; //input buffer index
bool dataRecv = false; //triggers request for another data set
bool bufEval = false; //ready to process flag
bool wait4proc = false; //buffer process hold for master send

#define OUT_BUFFER_SIZE 165 //required size
byte bufOut[OUT_BUFFER_SIZE]; //rs485 output buffer
int posOut = 0; //rs485 buffer index
bool outReady = false; //output buffer ready
bool dataValid = false; //data validity pass

char bufHost[5]; //rs485 input buffer
char posHost = 0; //rs485 input index

#define RS485_PING_TIME 8 //us
#define RS485_DATA_TIME 166 //us

void setup() {
  // initialize serial with RS485 transceiver
  PB9.begin(57600);
  pinMode(RS485DIR_PIN,OUTPUT);
  digitalWrite(RS485DIR_PIN, RS485_READ);
  Serial.begin(BAUD_RS485);
  PB9init();
  dataTime = millis() + DATA_TIMEOUT;
  bufOut[160]='\0';
}

void loop() {
  /*origionally part of the PB9 configurator interaction, seems not manditory.
  if (millis() > verTime + 600) {
    verTime = millis();
    PB9version();
  }*/
  
  //data watchdog timer
  if (millis() > dataTime ) {  //somethings gone wrong, probably a comm error
    PB9data(); //ask for data from sensor
    dataTime = millis() + DATA_TIMEOUT; //reset watchdog start time for data
    pos = 0; //reset input message buffer, may be a partial
  }
  

  // receive data from sensor
  while (PB9.available()) { //while recieve chars available
    c = PB9.read(); // read the incoming character
    buf[pos++] = c; //store char into inputt buffer
    
    if ( bufEval ) { //triggered last pass 
      //if here, the value was 0x3 last time and this char is the checksum
      bufEval = false;  //clear flag
      wait4proc = procMsg(); //compress raw into output buffer, true if failed by sending;
      dataRecv = true; 
    }

  if (dataRecv) {//a data packet was recieved, ask for another
    PB9data(); //request new data from sensor
    dataTime = millis() + DATA_TIMEOUT; //make new expiration for data watchdog
    dataRecv = false; //clear flag for self
  }
  
  //trigger evaluation next time when cheksum byte
  if (c == 0x3) { //detect data EOM char
      bufEval = true; //end of message stream next byte
    }
  }
  
  if (wait4proc) { //is the sensor buffer processing outstanding because of send hold
    wait4proc = procMsg(); //compress raw into output buffer, true if failed
  }
  
  /**
  * Master message processing 
  * 2(start char)
  * Message format {2, CMD, ID_H, ID_L, 3, Checksum)
  * CMD's: R(read) P(ping) 
  * ID number to respond to
  * 3(end char)
  **/
  while (Serial.available()) { 
    c = Serial.read();  //get char from host
    bufHost[posHost++] = c; //store to buffer
    
    if ( c == 3 ) {  //check for end char
      if(bufHost[2]==ID_H &&bufHost[3]==ID_L){  //if message is for me using ID
        if (bufHost[1]=='R' || bufHost[1]=='r') { //if read
          sendTime = millis() + RS485_DATA_TIME;
          sendScan();
        }
        if (bufHost[1]=='P' || bufHost[1]=='p') { //if ping
          //sendPing();
          sendTime = millis() + RS485_PING_TIME;
          digitalWrite(RS485DIR_PIN, RS485_WRITE);
          Serial.print(0x2);
          if (outReady) {
            Serial.print('Y');
          } else {
            Serial.print('N');
          }
          Serial.print(ID_H);
          Serial.print(ID_L);
          Serial.print(0x3);
          
        }
        posHost = 0; //message was consumed, reset index
      }
      posHost=0; //clear 
    }
  }
  
  // check if RS485 message Tx timeout
  if (millis() > sendTime) {
    digitalWrite(RS485DIR_PIN, RS485_READ);
  }

  //check if actively waiting to finish sending scan info to master
  if (sendActive) { 
    sendActive = primeTxBuffer(); //send data, store if more work to do
  } //self clearing, returns false when output emptied
}

void sendScan() {
  //begining of message transfer
  outReady = false; //master has most recent information
  posOut = 0; //reset index for tx
  digitalWrite(RS485DIR_PIN, RS485_WRITE); //set the RS485 chip to transmit to the bus
  sendActive = primeTxBuffer(); //send data, create data hold if more work to do
}

// pushes data into the buffer, exit when full or finished
// returns true when the out buffer is not at end
bool primeTxBuffer () {
  while(Serial.availableForWrite()) {
    Serial.print(bufOut[posOut++]);
    if (posOut >= OUT_BUFFER_SIZE) {
      return  false; //no more work to do
    } //more chars in out buffer to send
  } //no more room in the TX buffer
  return true; //filled Tx buffer, more work to do
}

//look through input buffer and move to output or discard version or other info
    /** Sensor Data: {2,D,0,B,5,A, d, d, ... d, d, 3, n}
    * message is 328bytes in total
    * 0x2D0B5A start the data set
    * each sample is 4 sent bytes, all ascii values 
    * 320 data bytes, 80 samples
    * infinity seems to be 0xFF40
    * 0x3n ends data, n is sucessive Xor'd checksum 
    **/
bool procMsg (void) {
  if (sendActive) { //is there a memory hold in effect
    return true; //cause this routine to run later after the current RS485 send is complete
  }
  
  if (buf [1] == 'D') {
    //verify checksum for data integrity
    byte checksum = 0; //clear to start
    for (int x = 0; x < 327; x++) { 
      checksum ^= buf[x]; //Xor all the data
    }
    if  (checksum == buf[237]) { //the most recent message is valid T/F
      dataValid = true;
    } else {
      dataValid = false;
    }
    
    //make output buffer
    posOut = 0;//reset index

    //Pos {  0,   1,    2,    3,   4,       5,       6,       7, ...      164, 165] 
    //Data[0x2, 'D', ID_H, ID_L, DV, Data0_L, Data0_H, Data1_L, ... Data79_H, 0x3]
    
    bufOut[posOut++] = 2; //start message
    bufOut[posOut++] = 'D'; //data message
    bufOut[posOut++] = ID_H; //unit ID
    bufOut[posOut++] = ID_L; //unit ID
    
    if (dataValid) { //DV, data validity 
      bufOut[posOut++] = 'V'; //can confirm data from sensor
    } else {
      bufOut[posOut++] = 'M'; // Missing "take with a grain of salt"
    }
    
    //data is 160 bytes
    for (int x = 6; x < 326; x += 4) { //compress the data
      bufOut[posOut++] = (charToByte(buf[x+1]) << 4) | charToByte(buf[x]);
      bufOut[posOut++] = (charToByte(buf[x+3]) << 4) | charToByte(buf[x+2]);
    }
    bufOut[posOut++] = 3; //end message
    
    //mark that new data is ready
    outReady = true;
  }
  return false;
}

//input and ascii and return the hex value
byte charToByte (byte c) {
  switch (c) {
    case 'A':
    return 0xA;
    case 'B':
    return 0xB;
    case 'C':
    return 0xC;
    case 'D':
    return 0xD;
    case 'E':
    return 0xE;
    case 'F':
    return 0xF;
  }
  if (c >= '0' && c <= '9') {
    c -= 0x30; //shift from ascii
  } else {c=0;} //default
  return c;
}


const char getData[4] = {2,68,3,69}; //sent at the end of each data burst
const char getVersion[4] = {2,86,3,87}; // sent every 0.6s to keep alive???
//char chgRate[8] = {2, 72, 38, 68, 88, 52, 48, 3};
const char chgRate[8] = {130, 200, 166, 196, 216, 180, 176, 131}; //adjusted to look like 7n1
const char startScan[15] = {2,54,4,2,86,  3,87,2,38,70,   70,70,70,3,39};

void PB9init (void) {
  sendArrayToSensor(chgRate,8);//  change PB9 baud rate and bit configuration
  delay(500);
  PB9.begin(38400);
  sendArrayToSensor(startScan,15);
  delay(125);
  PB9version();
  delay(500);
  PB9.end(); //dump buffers
  PB9.begin(38400);
  PB9data(); //ask for first data
}

void PB9data (void) {
   sendArrayToSensor(getData,4);
}

void PB9version (void) {
   sendArrayToSensor(getVersion,4);
}

void sendArrayToSensor (char * message, int len) {
  for (uint8_t x = 0; x < len; x++) {
    PB9.print( message[x] ); //
  }
}



  

#endif


