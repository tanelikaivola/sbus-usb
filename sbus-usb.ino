// This was tested on: Arduino 1.6.8 with Teensyduino
//
// Tools -> Board: Teensy 3.1 / 3.2
// Tools -> USB Type: "Serial + Keyboard + Mouse + Joystick"
// Tools -> CPU Speed: 72 MHz optimized

#define SBUSSERIAL Serial1
int _channels[18];

void setup()
{
  Serial.begin(115200);
  Serial.println("SBUS Status");
  SBUSSERIAL.begin(100000, SERIAL_8E1_RXINV);
  Joystick.useManualSend(true);
}

static byte mode = 0;
static byte mode_old = 0;


void loop() {
  static byte buffer[25];

  do { // Wait for the start of the frame
    while (!SBUSSERIAL.available()) {}
  } while (SBUSSERIAL.read() != 0xf);

  for (int i = 1; i < 25; i++) {
    while (!SBUSSERIAL.available()) {}
    buffer[i] = SBUSSERIAL.read();
  }
  if (buffer[24] != 0) {
    Serial.println("Frame failed");
    return;
  }

//  Serial.println("Frame ok");

  _channels[0]  = ((buffer[1]        | buffer[2]  << 8)                     & 0x07FF);
  _channels[1]  = ((buffer[2]  >> 3  | buffer[3]  << 5)                     & 0x07FF);
  _channels[2]  = ((buffer[3]  >> 6  | buffer[4]  << 2 | buffer[5] << 10)   & 0x07FF);
  _channels[3]  = ((buffer[5]  >> 1  | buffer[6]  << 7)                     & 0x07FF);
  _channels[4]  = ((buffer[6]  >> 4  | buffer[7]  << 4)                     & 0x07FF);
  _channels[5]  = ((buffer[7]  >> 7  | buffer[8]  << 1 | buffer[9] << 9)    & 0x07FF);
  _channels[6]  = ((buffer[9]  >> 2  | buffer[10] << 6)                     & 0x07FF);
  _channels[7]  = ((buffer[10] >> 5  | buffer[11] << 3)                     & 0x07FF);
  _channels[8]  = ((buffer[12]       | buffer[13] << 8)                     & 0x07FF);
  _channels[9]  = ((buffer[13] >> 3  | buffer[14] << 5)                     & 0x07FF);
  _channels[10] = ((buffer[14] >> 6  | buffer[15] << 2 | buffer[16] << 10)  & 0x07FF);
  _channels[11] = ((buffer[16] >> 1  | buffer[17] << 7)                     & 0x07FF);
  _channels[12] = ((buffer[17] >> 4  | buffer[18] << 4)                     & 0x07FF);
  _channels[13] = ((buffer[18] >> 7  | buffer[19] << 1 | buffer[20] << 9)   & 0x07FF);
  _channels[14] = ((buffer[20] >> 2  | buffer[21] << 6)                     & 0x07FF);
  _channels[15] = ((buffer[21] >> 5  | buffer[22] << 3)                     & 0x07FF);

  ((buffer[23])      & 0x0001) ? _channels[16] = 2047 : _channels[16] = 0;
  ((buffer[23] >> 1) & 0x0001) ? _channels[17] = 2047 : _channels[17] = 0;

  byte pos=0;
  Joystick.X(_channels[pos++] >> 1);
  Joystick.Y(_channels[pos++] >> 1);
  Joystick.Z(_channels[pos++] >> 1);
  Joystick.Zrotate(_channels[pos++] >> 1);
  Joystick.sliderLeft(_channels[pos++] >> 1);
  Joystick.sliderRight(_channels[pos++] >> 1);
  Joystick.send_now();

  mode = map(_channels[8],0,2000,0,3);
  if(mode != mode_old) {
    mode_old = mode;
    //Mouse.click();
    Keyboard.print("l");
    Serial.print("Mode: ");
    Serial.println(mode, DEC);
  }

/*  for(int i=0;i<18;i++) {
    Serial.print("Channel ");
    Serial.print(i,DEC);
    Serial.print(":");
    Serial.println(_channels[i], DEC);
  }*/
}
