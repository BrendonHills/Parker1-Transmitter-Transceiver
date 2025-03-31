/*
  ==================================================
   The Parker1 Transceiver by Daimon Tilley, G4USI. 
  ==================================================
      standing on the shoulders of:
      The Tuna Tin S by Bob Fontana AK3Y, as amended by Carol Milazzo KP4MD.
      Their code was the basis of this code, but I have changed it very substantially. All
      that remains of their original code is aspects of the display, encoder menu and band array functions.

  Revised 8/2/25 by Daimon Tilley, G4USI - Improved the CQ memory, such that it repeats every 4 seconds until left paddle or straight key is pressed
  Revised 3/2/2025 by Daimon Tilley, G4USI - Improved and simplified Sidetone code for a 700Hz tone and improved key speed detection.
  Revised 30/1/25 by Daimon Tilley, G4USI - THE PARKER 1 IS NOW A FULL 10 BAND TRANSCEIVER! IT ALSO WON THE 2024 G-QRP CLUB CONSTRUCTION COMPETITION!!!!
    -- Clock 2 now enables as Quadrature output for integrated QRPLabs RX module. 
    -- Changed Si5351 library from Etherkit to Si5351mcu to remove clicks from receiver on encoder movement
    -- Hardware changes include adding QRPLabs RX module, a Hi-Per-Mite CW filter, Audio Amp and feeding CW sidetone
       into the audio filter. Also added a volume control, Wide/Narrow switch to bypass filter and send RX audio direct to amplifier
       and finally included a headphone jack. 
  Revised 10/7/24 by Daimon Tilley, G4USI - rig now completed and boxed.
    -- Added to the display with a supply voltage display using Pin A2 and a 20k/10k voltage divider
    -- Added to the display with a readout of the Keyer WPM setting
  Revised 1/7/24 by Daimon Tilley, G4USI as a 160-10m Portable Transmitter with battery and L-Match ATU,
    -- Corrected Spot / Key switches
    -- Turned off internal RX clock
  Revised 14/3/2024 by Daimon Tilley, G4USI - Vers. 2.1
    -- Amended use of CW OUT pin to use to activate PNP transistor as High Side switch on 5v PA bias supply.
       This pin is now kept HIGH on RX to switch this transistor off. On TX pin is pulled LOW to provide bias supply.
  Revised 9/3/2024 by Daimon Tilley, G4USI - Vers. 2.0
    -- Bugfix to frequency setting which prevented sinewave! (SetFreq increased *100ULL).
  Revised 23/2/24 by Daimon Tilley, G4USI - Vers 1.9
    -- During automated CQ call, pressing the dit lever, or external or internal straight key 
      will cease the auto CQ. NOTE however, that this is not done using an interrupt pin, 
      so the key must be held so it is recognised as closed between characters. 
      A slower auto CQ will require the key to be held closed for longer than a faster one. 
      In practice this is fine.
    -- Added a welcome-beep on power up
  Revised 20/2/24 by Daimon Tilley, G4USI - Vers 1.8
    -- Reduced code by 14 lines by creating two new functions - enableTX() and enableRX()
  Revised 20/2/24 by Daimon Tilley, G4USI - Vers 1.7
    -- Amended to remove the 630m and 6m bands
    -- LPF filters adjusted to match and are now grouped as follows:
        160m          - Pin 8
        80 / 60m      - Pin 9
        40 / 30m      - Pin 10
        20 / 17m      - Pin 11
        15/12/10m     - Pin 12
        All pins HIGH except for band in use which is LOW
  Revised 20/2/24 by Daimon Tilley, G4USI - Vers 1.6
    -- Amended to recognise both short and long press of Rotary Encoder -
       - Short press (<1 secs) functions as before (Frequency and Band Steps)
       - Long press (>1 secs) AND release of encoder now initiates stored CW CQ message
       - Spare button now reverts to internal straight key
  Revised 19/2/24 by Daimon Tilley, G4USI - Vers 1.5
    -- Removed keying code from PA3HCM and added simple memory keyer, based on CW Beacon by K6HX 
        but significantly amended to allow variable speeds set by SpeedPot and amended to work with paddle, 
        a straight key and a memory button.
        Spare key use changed from internal straight key to CW message send
  Revised 19/2/24 by daimon Tilley, G4USI - Vers 1.4
    -- Spot and Spare buttons on 3d printed fascia were wrong, so now amended to Spot = 6 and Spare = 7. 
    -- Spare button on PCB now acts as internal straight key for emergency / no external key available
  Revised 18/2/24 by Daimon Tilley, G4USI - Vers 1.3
    -- Amended so that band change relay switching occurs only when band selected, rather than changing when cycling through bands
    -- Amended so that after a band change occurs, tuning step default is 100Hz
    -- Recognises if straight key plugged in at start-up and if so, omits keying software. If you wish to change to paddle,
        then power down, plug in paddle and re-boot, and vice verca. There is no option to change from software. Straight key recognition
        or otherwise occurs just once in setup on booting. A straight key must be wired to a mono audio jack plug for this to work.
  Revised 18/2/24 by Daimon Tilley, G4USI - Vers. 1.2
    -- Re-named NanoVFO to reflect bespoke VFO PCB
    -- Added Clk2 for RX with options for either 700Hz positive offset or 4x freq + 700Hz for Quadrature RX, i.e. QRPLabs RX module
    -- Removed EEPROM facility as not required
    -- Removed alternate Freq Step indication as uneccessary
  Revised 10/2/24 by Daimon Tilley, G4USI
    -- Improved reading of key speed pot
   Revised 1/12/23 by Daimon Tilley, G4USI
     -- Replaced Adafruit Si5351 library and associated code, with Etherkit library and code to simplify code and maths
   Revised 10/7/23 by Daimon Tilley, G4USI
     -- Added simple CW keying code, based on code by PA3HCM
     -- Added sidetone from Pin 5 (Just connect speaker between Pin5 and Gnd - use a 120 ohm resistor to limit current)
   Revised 13/5/23 by Daimon Tilley, G4USI
    -- Amended start-up frequency step to 0.1kHz
    -- Amended Transmit/Receive Pin from Pin 8 to Pin 7
    -- Enable switching of QRP Labs LPF relay kit, allowing 5 bands (in my case 160-20m inclusive) to
        be switched from band changes, by enabling Pins 8 - 12 respectively. Pin is HIGH to turn off relays
        and LOW to activate the correct relay for the band.
   Revised 12/5/23 by Daimon Tilley G4USI
     -- Physically changed board layout to allow PTT line-out switching
   Revised 12/3/2022 by Daimon Tilley G4USI
     -- amended dial correction factor and fixed a 60m channel mis-type
   Revised 25/2/2022 by Daimon Tilley G4USI
     -- amended 60m Channels for UK use
     -- amended QRP calling channels for IARU Region 1
   Revised 01 January 2022 by Carol Milazzo KP4MD
     -- added 630m & 6m
     -- cleaned up code and moved band constants into arrays
     -- new PLL constants improve freq tracking on 60m and 12m
   Revised 15 April 2021 by Carol Milazzo KP4MD
     -- added 160m, 17m and 12m
     -- corrected Calibration Offset for all bands
   Revised 09 April 2021 by Carol Milazzo KP4MD
     -- added default QRP frequencies and band switching for 80, 40, 60, 30, 20, 15 and 10 meters
     -- added band indicator to display
   Revised 27 March 2021 Revisions below by Bob Fontana AK3Y
     -- added provision for 5 fixed frequency CW channels on 60 meters
     -- fixed EEProm code so that header changes to Band will trigger rewrite
   Revised 24 February 2021
     -- added timing loop to EEPROM write; current settings will not be written to nonvolatile
     -- memory until DELAY_TIME has been exceeded.  This prevents too many writes to EEPROM which
     -- can only handle around 100,000 writes
   Revised 21 February 2021
     -- added EEPROM storage of last used frequency and tuning rate (loads upon reboot)
     -- Note: if only tuning rate is changed, data is not stored
   Revised 12 December 2020
     -- added an additional tuning increment (10Hz, 100Hz and 1000Hz)
     -- pressing switch toggles from 1000 --> 100 --> 10 --> 1000 Hz
   Revised 10 December 2020
     -- added 30 meters to frequency bands
     -- modified display to indicate tuning rate
   Revised 3 March 2020 -- added Transmit/Receive function to kill synthesizer output on Receive
   Revised 25 February 2020 -- added Calibration Offset
     -- Note: calibration offset should be computed for each band of operation

   12 February 2020
*/

#include <Wire.h>              // Include I2L Library for Communications with OLED Display and Synthesizer
//#include <si5351.h>            // Include Etherkit library
#include "si5351mcu.h"
#include <Adafruit_SSD1306.h>  // Include OLED Display Library

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels
#define XT_CAL_F 0     //++++++Si5351 calibration factor, adjust to get exatcly 10MHz. Starts at 330000 ++++ Increasing this value will decreases the frequency and vice versa.

//CW Keyer==========================================================================================================================================
const int paddleDot = 0;    // Connects to the dot lever of the paddle
const int paddleDash = 1;   // Connects to the dash lever of the paddle
const int sidetonePin = 5;  // Audio output
const int biasPin = 13;     // Activates PNP transistor as High side swith to apply +5v bias to PA
const int switchPin = A1;   // To activate TX / RX relay
const int intKey = 7;       // Using "Spare" button on pcb as internal straight key
int keySpeed = A0;          // Attached to center pin of potmeter, allows you to set the keying speed.
int speed;                  // Converts Speed Pot reading to a WPM value
int voltageDivider;         // Reads one third of the supply voltage via a 20k / 10k voltage divider
float voltage;              // Converts the voltage divider reading to actual voltage for display
bool straightKey = false;   //Used to store flag to identify if straight key used at start up

//For memory / beacon function ===================================================================================================================
const int timeInterval = 4000;
unsigned long timeNow = 0;
struct t_mtab {
  char c, pat;
};

struct t_mtab morsetab[] = {
  { '.', 106 },
  { ',', 115 },
  { '?', 76 },
  { '/', 41 },
  { 'A', 6 },
  { 'B', 17 },
  { 'C', 21 },
  { 'D', 9 },
  { 'E', 2 },
  { 'F', 20 },
  { 'G', 11 },
  { 'H', 16 },
  { 'I', 4 },
  { 'J', 30 },
  { 'K', 13 },
  { 'L', 18 },
  { 'M', 7 },
  { 'N', 5 },
  { 'O', 15 },
  { 'P', 22 },
  { 'Q', 27 },
  { 'R', 10 },
  { 'S', 8 },
  { 'T', 3 },
  { 'U', 12 },
  { 'V', 24 },
  { 'W', 14 },
  { 'X', 25 },
  { 'Y', 29 },
  { 'Z', 19 },
  { '1', 62 },
  { '2', 60 },
  { '3', 56 },
  { '4', 48 },
  { '5', 32 },
  { '6', 33 },
  { '7', 35 },
  { '8', 39 },
  { '9', 47 },
  { '0', 63 }
};
#define N_MORSE (sizeof(morsetab) / sizeof(morsetab[0]))

int DOTLEN;
int DASHLEN;

// for LPF switching ==========================================================================================================================
bool bandChanged = false;  //flag if band changed
const int Pin160m = 8;     // selects appropriate relay board pin
const int Pin80m = 9;
const int Pin40m = 10;
const int Pin20m = 11;
const int Pin15m = 12;


// KY040 Rotary Encoder Module connections and variables ========================================================================================
const int PinCLK = 2;               // CLOCK (Interrupt line)  IMPORTANT! Decouple Pins 2 and 4 with 0.68uF caps to ground
const int PinSW = 3;                // Push Button Switch
const int PinDT = 4;                // Encoder DT Signal
const int SHORT_PRESS_TIME = 1000;  // 1 second
const int LONG_PRESS_TIME = 1000;   // 1 second
int lastState = HIGH;               // the previous state from the input pin
int currentState = HIGH;            // the current reading from the input pin
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;

// Spot button ==================================================================================================================================
const int SPOT_Pin = 6;  // Add SPOT facility by turning on Si5351 but not keying PA stage

/* =============================================================================================================================================
    Define Bands of Operation, Initial Frequencies and Calibration Offset, if set
    NOTE: For 60 meters, there are only 5 frequencies allowable for the CW carrier
      5332, 5348, 5358.5, 5373 and 5405 kHz
      To start up for the first time on the nth channel, set Freq1 = n and Band = 60
      The Rotary Encoder will now be used to cycle through the 5 channels
      The push button steps through band switching and frequency step size
*/
// Define Frequency Bands - Band 0 is reserved for future use
int BandArray[11] = { 0, 160, 80, 60, 40, 30, 20, 17, 15, 12, 10 };
int LPFPinArray[11] = { 0, 8, 9, 9, 10, 10, 11, 11, 12, 12, 12 };  //+++++for LPF Filter Pins.
// Default QRP Calling Frequencies - 60m is Channel 4 but default to 1 as only 4 channels
long FreqArray[11] = { 0, 1836000, 3560000, 1, 7030000, 10116000, 14060000, 18086000, 21060000, 24906000, 28060000 };
long ChanArray[6] = { 0, 5259000, 5260000, 5261000, 5262000, 5263000 };
long cal = XT_CAL_F;

int Band = 4;                  // Initializes to 40m on startup - the "0" in BandArray does not count!
long Freq1 = FreqArray[Band];  // Initializes to QRP Calling frequency/Channel on the startup Band
int deltaFreq = 100;           // Initialize tuning rate to 100Hz (use 1 for 60 meter operation)

long oldFreq = 0;
int SPOT_State = 0;  //for SPOT switch

Si5351mcu Si;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 4);

//==============================================================================================================================================
void isr() {  // Interrupt Service Routine runs if Rotary Encoder CLK pin falls
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 5) {

    if (!digitalRead(PinDT)) {  // If encoder rotated CCW

      if (deltaFreq == 0) {  // If deltaFreq is zero, save Freq1 and decrement Band
        FreqArray[Band] = Freq1;
        Band -= 1;
        if (Band < 1) Band = 10;
        Freq1 = FreqArray[Band];  // Load saved Freq1 for new band
        bandChanged = true;       //Set flag to indicate band has changed
      } else {                    // If deltaFreq is not zero
        if (Band == 3) {          // If Band is 60m decrement Channel
          Freq1 -= 1;
          if (Freq1 < 1) Freq1 = 5;
        } else Freq1 -= deltaFreq;  // If Band is not 60m decrement Frequency
      }
    } else {  // If encoder rotated CW

      if (deltaFreq == 0) {  // If deltaFreq is zero, save Freq1 and increment Band
        FreqArray[Band] = Freq1;
        Band += 1;
        if (Band > 10) Band = 1;
        Freq1 = FreqArray[Band];  // Load saved Freq1 for new band
        bandChanged = true;       //Set flag to indicate band has changed
      } else {                    // If deltaFreq is not zero
        if (Band == 3) {          // If Band is 60m increment Channel
          Freq1 += 1;
          if (Freq1 > 5) Freq1 = 1;
        } else Freq1 += deltaFreq;  // If Band is not 60m increment Frequency
      }
    }
  }
  lastInterruptTime = interruptTime;
}
//==============================================================================================================================================
void Splash()  // Splash screen displayed at power on/reset
{
  const char* text1 = "The Parker 1 Tcvr";
  const char* text2 = "160 - 10m by G4USI";
  const char* text3 = "2024 G-QRP Winner";

  display.clearDisplay();  // Clears the graphics display buffer.
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(display.width() / 2 - strlen(text1) * 6 / 2, 0);  // Each character is approximately 6 pixels wide in default type font
  display.println(text1);
  display.setCursor(display.width() / 2 - strlen(text2) * 6 / 2, 9);
  display.println(text2);
  display.setCursor(display.width() / 2 - strlen(text3) * 6 / 2, 18);
  display.println(text3);

  display.display();                // Need this command to refresh screen
  tone(sidetonePin, 700);
  delay(50);
  noTone(sidetonePin);
}
//=============================================================================================================================================
void SetFreq(int Band, long Freq) {
  if (Band == 3) {  // If Band is 60m retrieve Channel Frequency
    Freq = ChanArray[Freq1];
  }
  
  Si.setFreq(0, Freq); //si5351mcu
  //For desired offset on RX output Clk2
  //long RXFreq = Freq + DESIRED OFFSET;
  //For Quadrature RX i.e. QRPLabs RX Module, offset is chosen offset * 4
  long RXFreq = (Freq *4) -2000;
  Si.setFreq(2, RXFreq);
  Si.enable(2);
}

//==============================================================================================================================================
void Display_Freq(unsigned long freq) {  // Display Frequency
  unsigned int f_int, f_frac;
  char buf[50];


  if (Band == 3) freq = ChanArray[Freq1];  // If Band is 60m display the frequency instead of the channel number
  f_int = freq / 1000;
  f_frac = (freq - (long)f_int * 1000) / 10;

  display.clearDisplay();  // Clears the graphics display buffer.

  String voltageDisplay = String(voltage) + " V   ";
  String speedDisplay = String(speed) + " WPM";
  String voltAndSpeedDisplay = voltageDisplay + speedDisplay;
  display.setTextSize(1);
  display.setCursor(display.width() / 2 - voltAndSpeedDisplay.length() * 6 / 2, 0);  //centre text
  display.print(voltAndSpeedDisplay);
 
  display.setTextSize(2);
  if (f_int < 1000) {
    display.setCursor(display.width() / 2 - 24, 10);
  } else {
    if (f_int < 10000) {
      display.setCursor(display.width() / 2 - 36, 10);
    } else {
      display.setCursor(display.width() / 2 - 48, 10);
    }
  }
  if (f_frac < 10) {
    snprintf(buf, sizeof(buf), "%u.%d%d", f_int, 0, f_frac);
  } else {
    snprintf(buf, sizeof(buf), "%u.%d", f_int, f_frac);
  }
  display.print(buf);

  display.setTextSize(1);

  if (deltaFreq == 0) {  // If deltaFreq is zero display "Step Band"
    display.setCursor(display.width() / 2 - strlen("Step Band") * 6 / 2, 25);
    display.print("Step Band");
  } else {  // If deltaFreq is not zero display Channel Number or Step Size

    if (Band == 3) {  // If Band is 60m Display Channel Number
      display.setCursor(display.width() / 2 - strlen("Channel #") * 6 / 2, 25);
      display.print("Channel ");
      display.print(Freq1);
    }

    else {  // If Band is not 60m Display Step size
      switch (deltaFreq) {

        case 10000:
          display.setCursor(24, 25);
          display.print("Step ^");
          break;
        case 1000:
          display.setCursor(36, 25);
          display.print("Step ^");
          break;
        case 100:
          display.setCursor(60, 25);
          display.print("Step ^");
          break;
        case 10:
          display.setCursor(72, 25);
          display.print("Step ^");
          break;
      }
    }
  }
  display.display();
}
//===============================================================================================================================================
// Remove or comment out this section if used with a built in receiver ??OPPORTUNITY FOR BAND CYCLING??

void Check_SPOT_Status() {
  SPOT_State = digitalRead(SPOT_Pin);
  if (SPOT_State == LOW) {                 //Spot button pressed
    Si.enable(0);
  } else {                                 // Spot button not pressed
    Si.disable(0);
  }
}

//================================================================================================================================================
// routine for keying if paddle connected, i.e. straightKey=false
void dash() {
  enableTX();
  tone(sidetonePin, 700);
  delay(DASHLEN);
  noTone(sidetonePin);
  enableRX();
  delay(DOTLEN);
}

void dit() {
  enableTX();
  tone(sidetonePin, 700);
  delay(DOTLEN);
  noTone(sidetonePin);
  enableRX();
  delay(DOTLEN);
}

//=============================================================================================================================================
//routines for memory keyer

void send(char c) {
  int i;
  if (c == ' ') {
    delay(7 * DOTLEN);
    return;
  }
  for (i = 0; i < N_MORSE; i++) {
    if (morsetab[i].c == c) {
      unsigned char p = morsetab[i].pat;

      while (p != 1) {
        if (p & 1)
          dash();
        else
          dit();
        p = p / 2;
      }
      delay(2 * DOTLEN);
      return;
    }
  }
  //if we drop off the end, then we send a space
}

void sendmsg(char* str) {
  while (*str) {
    if ((!digitalRead(paddleDot)) || (!digitalRead(intKey))) {
      break;
    }
    send(*str++);
  }
}

//=============================================================================================================================================
//routine for keying if straight key in use, i.e. straightKey=true
void STRAIGHTKEYER() {

  enableTX();                                                    // Key down
  while ((!digitalRead(paddleDot)) || (!digitalRead(intKey))) {  //stay here until key released
    tone(sidetonePin, 700);
  }
  noTone(sidetonePin);
  enableRX();
}
//=============================================================================================================================================
//Enable TX
void enableTX() {
  Si.disable(2);
  digitalWrite(switchPin, LOW);  // Switch relay to TX position
  Si.enable(0);
  digitalWrite(biasPin, HIGH);
}

//==============================================================================================================================================
//Enable RX
void enableRX() {
  noTone(sidetonePin);
  digitalWrite(biasPin, LOW);  // Key up
  Si.disable(0);
  digitalWrite(switchPin, HIGH);  // turn off relay (RX position)
  Si.enable(2);
}

//=============================================================================================================================================
void setup(void) {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize OLED Display
 
  Si.init(25000000L);
  Si.reset();
  Si.correction(cal);
  Si.disable(0);
  Si.disable(1);
  Si.enable(2);
  Si.setPower(0, 0);
  Si.setPower(1, 0);
  Si.setPower(2, 0);

  // initialise LPF Relay Board Pins
  pinMode(Pin160m, OUTPUT);
  pinMode(Pin80m, OUTPUT);
  pinMode(Pin40m, OUTPUT);
  pinMode(Pin20m, OUTPUT);
  pinMode(Pin15m, OUTPUT);
  digitalWrite(Pin160m, HIGH);  //turn off all LPF relays
  digitalWrite(Pin80m, HIGH);
  digitalWrite(Pin40m, HIGH);
  digitalWrite(Pin20m, HIGH);
  digitalWrite(Pin15m, HIGH);
  digitalWrite(LPFPinArray[Band], LOW);  //activate relay for start-up band

  // initialise pins for CW Keying
  pinMode(paddleDot, INPUT_PULLUP);
  pinMode(paddleDash, INPUT_PULLUP);
  pinMode(intKey, INPUT_PULLUP);
  pinMode(sidetonePin, OUTPUT);
  pinMode(biasPin, OUTPUT);       // to key the PA bias to ground via PNP transistor
  pinMode(switchPin, OUTPUT);     // to disconnect RX port from antenna on TX
  digitalWrite(biasPin, LOW);     // Start with key up
  digitalWrite(switchPin, HIGH);  //  Start with switch transistor 'off' (RX)

  // identify if key or paddle connected
  if (!digitalRead(paddleDash))  // If the dash lever is shorted to ground we must have a straight key in use...
  {
    straightKey = true;
  } else  //If dash lever is pulled-up to 5v, we have a paddle.....
  {
    straightKey = false;
  }

  // initialise rotary encoder pins and spot
  pinMode(PinSW, INPUT_PULLUP);  // forgot pull-up resistor on nanovfo board
  pinMode(PinCLK, INPUT);
  pinMode(PinDT, INPUT);
  pinMode(SPOT_Pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), isr, FALLING);  // Pins 2 and 3 are interrupts on Arduino Nano

  Splash();  // Display Splash screen
  delay(3000);
  display.clearDisplay();
  display.display();
}
//================================================================================================================================================
void loop() {

  Display_Freq(Freq1);

  if (Freq1 != oldFreq) {
    Display_Freq(Freq1);
    SetFreq(Band, Freq1);
    oldFreq = Freq1;
  }

  Check_SPOT_Status();  //check if SPOT button pressed

  // Get keyer speed
  speed = analogRead(keySpeed);         // Read the keying speed from potmeter
  speed = map(speed, 0, 1023, 35, 10);  //was 57,17
  DOTLEN = (1200 / speed);
  DASHLEN = (3 * (1200 / speed));

  // Get Battery voltage
  voltageDivider = analogRead(A2);                  // Read the level of one third of supply voltage via voltage divider
  voltage = ((voltageDivider * (4.2 / 1023.0)) * 3);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 12.6V) for a 3S battery

  // Keyer code

  if (!digitalRead(intKey))  // If the internal straight key is presssed..
  {
    STRAIGHTKEYER();
  }

  if (straightKey == false)  //Paddle connected
  {

    if (!digitalRead(paddleDot))  // If the dot lever is presssed..
    {
      dit();  // ... send a dot at the given speed
    }

    if (!digitalRead(paddleDash))  // If the dash lever is pressed...
    {
      dash();  // ... send a dash at the given speed
    }
  }

  if (straightKey == true)  //Straight key in use
  {
    if (!digitalRead(paddleDot))  // If the dot lever is presssed..
    {
      STRAIGHTKEYER();
    }
  }

  // Check if Rotary Encoder pressed
  currentState = digitalRead(PinSW);

  if (lastState == HIGH && currentState == LOW) {  // button is pressed
    pressedTime = millis();
  } else if (lastState == LOW && currentState == HIGH) {  // button is released
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

    if (pressDuration < SHORT_PRESS_TIME) {
      if (Band == 3) {                   // If Band is 60m
        deltaFreq = !deltaFreq;          // toggle deltaFreq between 0 and 1
      } else if (bandChanged == true) {  //do band filter switching here after a band has been selected
        deltaFreq = 100;                 // set intial tune step to 100Hz on band change
        for (int i = 8; i <= 12; i++) {
          digitalWrite(i, HIGH);
        }
        digitalWrite(LPFPinArray[Band], LOW);
        bandChanged = false;
      } else {                // If Band is not 60m and band has not been changed, toggle frequency steps
        switch (deltaFreq) {  // increment frequency step
          case 0: deltaFreq = 10000; break;
          case 10: deltaFreq = 0; break;
          case 100: deltaFreq = 10; break;
          case 1000: deltaFreq = 100; break;
          case 10000: deltaFreq = 1000; break;
        }
      }
    }

    if (pressDuration > LONG_PRESS_TIME) {
     while ((digitalRead(paddleDot)) && (digitalRead(intKey))) { //repeat CQ until left paddle or straight / internal key is pressed
      if (millis() - timeNow >= timeInterval) { //timeInterval set to 4 seconds
        sendmsg("CQ CQ CQ DE G4USI G4USI G4USI K");
        timeNow = millis();
      }
    }
  }
  }
  // save the the last state
  lastState = currentState;
}
