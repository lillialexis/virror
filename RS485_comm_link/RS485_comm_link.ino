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

#if MODE == 0
//teensy 3.6
  #define MASTER
  #define CHAR_TIME 11
#endif
#if MODE == 1
//arduino nano slave
  #define NODE
  #define NODE_NUM 1
  #define ID_H '0' //must be ascii
  #define ID_L '1'
  #define CHAR_TIME 7
#endif

#define BAUD_USART 115200
#define BAUD_RS485 115200 //1000000
#define RS485_READ 0 //setup constants used in configuring the direction pin for the RS485 network
#define RS485_WRITE 1

#ifdef MASTER //*****************************************************************
//485 buffer vars
#define RS485RX_PIN 9
#define RS485TX_PIN 10
#define RS485DIR_PIN 11
#define RS_BUFFER_SIZE 10 //rs485 buffer size
byte bufRs[RS_BUFFER_SIZE]; //rs485 input buffer
int posRs = 0; //rs485 buffer index


char c = 0;   // for incoming serial data
#define SEND_TIMEOUT 55
unsigned long sendTime; //rs485 sent period timeout
bool sendActive = false;
int dataCount = 0;
int longCount = 0;
bool dataRecv = false;
#define IN_BUFFER_SIZE 10 //rs232 input buffer size
byte bufIn[IN_BUFFER_SIZE]; //rs232 input buffer
int posIn = 0; //rs232 input buffer index
#define NUM_SENSORS 14
#define SENSOR_POINTS 80 //number of samples to be taken accross a horizontal range
int scanData[NUM_SENSORS][SENSOR_POINTS];

void setup() {
  // initialize serial with RS485 transceiver
  Serial.begin(BAUD_USART); //computer
  Serial.println("Ready");
  Serial2.begin(BAUD_RS485);  //sensors
  pinMode(RS485DIR_PIN,OUTPUT); //
  digitalWrite(RS485DIR_PIN, RS485_READ); //initialize dir pin to read
  
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
    c = Serial.read();  // read the incoming character from computer
    Serial.print(c);
    if (c == '\n' || c == '\r'){ //if the end char
      digitalWrite(RS485DIR_PIN, RS485_WRITE); //initialize dir pin to write
      sendTime = micros() + SEND_TIMEOUT; //make new expiration for data watchdog
      switch(bufIn[0]) {
        case 'p':
        case 'P':
        Serial2.print(2);
        Serial2.print('P');
        Serial2.print((char)bufIn[1]);
        Serial2.print((char)bufIn[2]);
        Serial2.println(3);
        Serial.print("Ping");
        Serial.print((char)bufIn[1]);
        Serial.println((char)bufIn[2]);
        break;
        
        case 'r':
        case 'R':
        Serial2.print(2);
        Serial2.print('R');
        Serial2.print((char)bufIn[1]);
        Serial2.print((char)bufIn[2]);
        Serial2.println(3);
        Serial.print("Read");
        Serial.print((char)bufIn[1]);
        Serial.println((char)bufIn[2]);
        break;
        
        case 'D': 
        case 'd': 
        Serial.print("Data: "); 
        Serial.print(dataCount);
        Serial.print(", "); 
        Serial.println(longCount);
        break;
      }
      posIn = 0; //reset buffer index
    } else { //not the end char
      bufIn[posIn++] = c; //store to buffer
    }
    if (posIn == IN_BUFFER_SIZE) {posIn--;}
  }
  
  while (Serial2.available()) {
    // read the incoming character from sensors
    c = Serial2.read(); //Serial2.read?
    switch (c) {
    case 2:
      Serial.print('*'); 
      dataCount = longCount = 0;
      break;
    case 'Y':
      Serial.print("Ready"); 
      break;
    case 'N':
      Serial.print("Wait"); 
      break;
    case 3:
      Serial.print('\n'); 
      dataRecv = true;
    default:
      Serial.print(c);
      dataCount++;
      if (dataCount == 3) {
        dataCount = 0;
        longCount++;
        if (longCount == 80) {
          Serial.print('*');
        }
      }
    }   
  }

      // check if RS485 message Tx timeout
  if (micros() > sendTime) {
    digitalWrite(RS485DIR_PIN, RS485_READ);
  }

  if (dataRecv) {//a data packet was recieved, ask for another
//    PB9data(); //request new data from sensor
//    dataTime = millis() + DATA_TIMEOUT; //make new expiration for data watchdog
    dataRecv = false; //clear flag for self
  }
  
}

//input and ascii and return the hex value
byte charToByte (byte c) {
  switch (c) {
    case 'A':
    case 'a':
    return 0xA;
    case 'B':
    case 'b':
    return 0xB;
    case 'C':
    case 'c':
    return 0xC;
    case 'D':
    case 'd':
    return 0xD;
    case 'E':
    case 'e':
    return 0xE;
    case 'F':
    case 'f':
    return 0xF;
  }
  if (c >= '0' && c <= '9') {
    c -= 0x30; //shift from ascii
  } else {c=0;} //default
  return c;
}
      
#endif

#ifdef NODE //*****************************************************************
#define PB9_RX_PIN 2
#define PB9_TX_PIN 3
#define RS485_TX_PIN 0
#define RS485_RX_PIN 1
#define RS485_DIR_PIN 8
#define LED_PIN 13
#include <SoftwareSerial.h>

//sensor data path
SoftwareSerial PB9(PB9_RX_PIN, PB9_TX_PIN); // RX, TX

#define RS485_PING_TIME 8 //us
#define RS485_DATA_TIME 166 //us
unsigned long sendTime; //rs485 send period timeout
bool sendActive = false; //performs data lock durring master data send

#define DATA_TIMEOUT 150 //not recv data from the sensor in too long
unsigned long dataTime; //data request watchdog,

//a pair of buffers are fliped (used alternately),
// one RS485_Tx the other RS232 input from sensor
bool activeBuf = false; 
#define PB9_BUFFER_SIZE 328
byte buf[2][PB9_BUFFER_SIZE]; //flipping sensor input buffers, 
unsigned int pos[2] = {0,0}; //input buffer index
bool dataRecv = false; //triggers request for another data set
bool bufEval[2] = {false, false}; //triggers data validation test, 0:sensor 1:485in
//bool wait4proc = false; //buffer process hold for master send
//bool dataValid[2] = {false, false}; //data in 485out was valid on inspection
bool outReady = false; //bufOut has been filled and ready to send

//#define OUT_BUFFER_SIZE 165 //required size
//byte bufOut[OUT_BUFFER_SIZE]; //rs485 output buffer
//int posOut = 0; //rs485 buffer index
//byte chkOut = 0; checksum used when generating messages
//bool outReady = false; //output buffer ready

char bufHost[7]; //rs485 input buffer
char posHost = 0; //rs485 input index


void setup() {
  // initialize serial with RS485 transceiver
  PB9.begin(57600);
  pinMode(RS485DIR_PIN,OUTPUT);
  digitalWrite(RS485DIR_PIN, RS485_READ);
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(BAUD_RS485);
  PB9init();
  dataTime = millis() + DATA_TIMEOUT;
  
}

void loop() {
  /*origionally part of the PB9 configurator interaction, seems not manditory.
   * probably checking to see that the sensor is still the same, auto recognize
  if (millis() > verTime + 600) {
    verTime = millis();
    PB9version();
  }*/
    
  checkOnPB9(); // receive data from sensor
    
  //data recv watchdog timer
  if (millis() > dataTime ) {  //somethings gone wrong, probably a comm error
    sendTimeout(); // missed a communication from the PB9, tell host
    dataRecv = true; //force reieve, causes a new data request
  }
  
  if (dataRecv) {//a data packet was recieved, or watchdog, ask for another
    dataRecv = false; //clear flag for self
    PB9data(); //request new data from sensor
    dataTime = millis() + DATA_TIMEOUT; //make new expiration for data watchdog
  }
    
  checkOnHost(); //check Rx on 485 link
  
  // check if RS485 message Tx timeout
  if (millis() > sendTime) { //if timer expired
    digitalWrite(RS485DIR_PIN, RS485_READ); //clear the Tx config line
  }

  //check if actively waiting to finish sending scan info to master
  if (sendActive) { 
    sendActive = primeTxBuffer(); //send data, store if more work to do
  } //self clearing, returns false when output emptied

  checkOnLED();
}

#define LED_CNT 20
unsigned int x;
void checkOnLED() {
  if (x > LED_CNT) {digitalWrite(LED_PIN, HIGH);}
  else {digitalWrite(LED_PIN, LOW);}
  if (x > (LED_CNT * 2)) {x=0;}
  else {x++;}
}

void sendPing (void) {
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

void sendTimeout (void) {
  sendTime = millis() + RS485_PING_TIME;
  digitalWrite(RS485DIR_PIN, RS485_WRITE);
  Serial.print(0x2);
  Serial.print('T');
  Serial.print(ID_H);
  Serial.print(ID_L);
  Serial.print(0x3);
}

//begins the sending of the buffer
void sendScan() {
  //begining of message transfer
  outReady = false; //master has most recent information
  pos[!activeBuf] = 0;
  digitalWrite(RS485DIR_PIN, RS485_WRITE); //set the RS485 chip to transmit to the bus
  sendTime = millis() + RS485_DATA_TIME; // set a timer to lower this pin once the data has been transmitted
  sendActive = primeTxBuffer(); //send data, create data hold if more work to do
}

// pushes data into the buffer, exit when full or finished
// returns true when the out buffer is not at end
bool primeTxBuffer () {
  /** RS-485 message structure
    //Pos {  0,   1,    2,    3,  4,        5,        6,        7,        8,        9, ...      164, 165] 
    //Data[0x2, 'D', ID_H, ID_L, DV, Data0_LL, Data0_LH, Data0_HL, Data0_HH, Data1_LL, ... Data79_H, 0x3]
    **/
  while(Serial.availableForWrite()) {
    switch (pos[!activeBuf]) {
    case 0:
      Serial.print(2);
      break;
    case 1:
      Serial.print('D');
      break;
    case 2: //overlay id high
      Serial.print(ID_H);
      break;
    case 3: //overlay id low
      Serial.print(ID_L);
      break;
    case 4: //skip 2 slots
      Serial.print('V'); //indicates the data is valid. PB9 checksum is not understood so all data is assumed valid
      pos[!activeBuf] = 6; //cause the next buffer send to be location 6, hacky way to just use the input buffer and slightly change the structure
      break;
    case PB9_BUFFER_SIZE:
      return  false; //reached end of buffer, no more work to do
      break;
    default:
      Serial.print(buf[!activeBuf][pos[!activeBuf]++]);
      break;
    }
  } 
  return true; //filled Tx buffer, more work to do
}

void checkOnHost (void) {
/**
  * Master message processing 
  * 2(start char)
  * Message format {2, CMD, ID_H, ID_L, 3, chk)
  * CMD's: R(read) P(ping) 
  * ID number to respond to
  * 3(end char)
  * Checksum Xor of all
  **/
  while (Serial.available()) { 
    char c = Serial.read();  //get char from host
    switch (c) {
    case 2: //begining of message
      posHost = 0; //reset index of char buffer
      break;
    case 3: //end of message
      if(bufHost[1]==ID_H && bufHost[2]==ID_L){  //if message is for me using ID
        switch (bufHost[0]) {
        case 'R':
        case 'r':  //if read data out to master
          sendScan();
          break;
        case 'P'
        case 'p'://if ping
          sendPing();
          break;
        }
      }
      for (int x = 0 ;x >= 0; x++) {
        bufHost[x] = '\0';
      }
      posHost=0;
    default: //some other char to store
      bufHost[posHost++] = c; //store to buffer
    } 
  }
}

void checkOnPB9(void) { 
/* Sensor Data: {2,D,0,B,5,A, d, d, ... d, d, 3, n}
    * message is 328bytes in total
    * 0x2D0B5A start the data set
    * each sample is 4 sent bytes, all ascii values 
    * 320 data bytes, 80 samples
    * infinity seems to be 0xFF40
    * 0x3n ends data, n is checksum 
    **/

    
  while (PB9.available()) { //while recieve chars available
    char c = PB9.read(); // read the incoming character
    if (c == 2) { //beginning of messsage string
      pos[activeBuf] = 0; //reset input buffer index
    }
      
    buf[activeBuf][pos[activeBuf]++] = c; //store char into input buffer
    
    if ( bufEval[activeBuf] ) { 
      //if here, the value was 0x3 last time and this c char is the checksum
      //look at checksum, triggered last pass 
      //checdksum not built yet
      bufEval[activeBuf] = false;  //clear self flag
      //should verify checksumm before switching buffers
      activeBuf != activeBuf; //switch between buffers
      outReady = true; //have new information for master
      dataRecv = true; //mark reieve of data packet, handles scan watchdog timer expiration
    }

    //trigger evaluation next time when cheksum byte
    if (c == 3) { //detect data EOM char
      bufEval[activeBuf] = true; //end of message stream next byte
    }
    
  }
} 

/**
 * checsum not implemented
 
byte createChecksum (byte * vals, unsigned int num) {
    //verify checksum for data integrity
    byte checksum = 0; //clear to start
    for (int x = 0; x < num; x++) { 
      checksum ^= vals[x]; //Xor all the data
    }
    return checksum;
 }

bool checksumTest (byte * vals, unsigned int num) {
  byte chk = createChecksum ( vals, num-1);
  if  (chk == vals[num-1]) { 
    return true;
  } else {
    return false;
  }
}
*/

/* not needed because of dual buffers
//look through input buffer and move to output or discard version or other info
bool procMsg (void) {
  if (sendActive) { //is there a memory hold in effect
    return true; //cause this routine to run later after the current RS485 send is not complete
  }
  
  if (buf [1] == 'D') {  //make output buffer
    
    posOut = 0;//reset index in output buffer
    //chkOut = 0;//reset checsum
    
    
    addToBufOut(2); //start message
    addToBufOut('D'); //data message
    addToBufOut(ID_H); //unit ID
    addToBufOut(ID_L; //unit ID

//section commented out because no longer nessesary to compress the data    
    /** RS-485 message structure
    //Pos {  0,   1,    2,    3,  4,       5,       6,       7, ...      164, 165] 
    //Data[0x2, 'D', ID_H, ID_L, DV, Data0_L, Data0_H, Data1_L, ... Data79_H, 0x3]
    **
//    if (dataValid) { //DV, data validity check
//      bufOut[posOut++] = 'V'; //can confirm data from sensor
//    } else {
//      bufOut[posOut++] = 'M'; // Missing "take with a grain of salt"
//    }
//    
//    //data is 160 bytes
//    for (int x = 6; x < 326; x += 4) { //compress the data
//      bufOut[posOut++] = (charToByte(buf[x+1]) << 4) | charToByte(buf[x]);
//      bufOut[posOut++] = (charToByte(buf[x+3]) << 4) | charToByte(buf[x+2]);
//    }

    for (int x = 6; x < 326; x++) { //move the data
      addToBufOut(buf[x]);
    }

    addToBufOut(3); //end message
    
    //mark that new data is ready
    outReady = true;
  }
  return false;
}

void addToBufOut(byte val) {
  bufOut[posOut++] = val;  //include char in buffer
//not worring about checksum yet
  //chkOut ^= val; //add to checksum
}*/

/*/input and ascii and return the hex value
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
}*/

/**
 * PB9 driver
 * 
 */
char getScan[4] = {2,68,3,69}; //sent at the end of each data burst
char getVersion[4] = {2,86,3,87}; // sent every 0.6s to keep alive???
//char chgRate[8] = {2, 72, 38, 68, 88, 52, 48, 3};
char chgRate[8] = {130, 200, 166, 196, 216, 180, 176, 131}; //adjusted to look like 7n1
char startScan[15] = {2,54,4,2,86,  3,87,2,38,70,   70,70,70,3,39};
 
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
   sendArrayToSensor(getScan,4);
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


