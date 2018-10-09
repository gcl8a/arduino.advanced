#include <Adafruit_PN532.h>

//Defines the pins for the RFID reader. Note that pins 2 & 3 are used by the shield and cannot be used for anything else.
Adafruit_PN532 nfc(2, 3);

//This will be the ID of the tag you will use to deactivate your system
//You'll have to edit these for your specific tag
uint8_t targetID[] = {0x7B, 0x46, 0x20, 0x2D};

enum ALARM_STATE {ALARM_OFF, ALARM_ARMED, ALARM_INTRUDER};
ALARM_STATE currentState = ALARM_OFF;

const int buttonPin = 7;
const int ledPin = 13;

void setup(void) 
{
  Serial.begin(115200);
  Serial.println("Hello!");

  if(!SetupRFIDShield()) //sets up the RFID
  {
    Serial.print("Didn't find PN53x board");
    while(1) {}
  }

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  Serial.println("setup() complete");
}

void loop(void)
{
  if(CheckArmingButton()) HandleArmingButton();
  if(CheckIfLaserBroken()) HandleLaserBroken();
  if(CheckForRFID()) HandleRFID();
}

void HandleArmingButton(void)
{
  //here we implement our state machine
  switch(currentState)
  {
    case ALARM_OFF:
      //you'll have to put in the code to arm
      Serial.println("Arming!");
      digitalWrite(ledPin, HIGH);
      currentState = ALARM_ARMED;
      break;
    //nothing happens for the other cases -- pushing the button has no effect
    default:
      break;
  }
}

void HandleLaserBroken(void)
{
  //here we implement our state machine
  switch(currentState)
  {
    case ALARM_ARMED:
      //add code to sound the alarm;
      break;
    default: 
      //nothing happens for any of the other states
      break;
  }
}

void HandleRFID(void)
{
  //here we implement our state machine
  switch(currentState)
  {
    case ALARM_ARMED:
    case ALARM_INTRUDER:
      Serial.println("Correct tag detected. Disarming.");
      digitalWrite(ledPin, LOW);
      currentState = ALARM_OFF;
      //you're job to code here
      break;
    default:
      break;
  }
}

bool SetupRFIDShield(void)
{
  //start the RFID reader and check that it's alive
  nfc.begin(); 
  if (!nfc.getFirmwareVersion()) 
  {
    return false;
  }
  
  // configure board to read RFID tags
  nfc.SAMConfig();

  return true;
}

int prevButtonState = 1;
bool CheckArmingButton(void)
{
  bool retVal = false;
  
  int currButtonState = digitalRead(buttonPin);
  if(prevButtonState != currButtonState)
  {
    delay(10);
    if(currButtonState == 0) retVal = true;
  }
  prevButtonState = currButtonState;

  return retVal;
}

bool CheckForRFID(void) 
{
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 20);
    
  if (success) 
  {
    Serial.println("Tag found!");
    if(uidLength == 4)
    {
      if(targetID[0] == uid[0] && targetID[1] == uid[1] && targetID[2] == uid[2] && targetID[3] == uid[3])
      {
        return true;
      }
    }
  }

  return false;
}

bool CheckIfLaserBroken(void)
{
  bool returnValue = false;

  //add code here...
  
  return returnValue;
}

