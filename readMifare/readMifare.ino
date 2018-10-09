#include <Adafruit_PN532.h>

//Defines the pins for the RFID reader. Note that pins 2 & 3 are used by the shield and cannot be used for anything else.
Adafruit_PN532 nfc(2, 3);

//This will be the ID of the tag you will use to deactivate your system
//You'll have to edit these for your specific tag
uint8_t targetID[] = {0x7B, 0x46, 0x20, 0x2D};

enum ALARM_STATE {ALARM_OFF, ALARM_ARMED, ALARM_INTRUDER};
ALARM_STATE currentState = ALARM_ARMED;

void setup(void) 
{
  Serial.begin(115200);
  Serial.println("Hello!");

  if(!SetupRFIDShield()) //sets up the RFID
  {
    Serial.print("Didn't find PN53x board");
    while(1) {}
  }

  Serial.println("setup() complete");
}

void loop(void)
{
  //here we implement our state machine
  switch(currentState)
  {
    case ALARM_OFF:
      //you'll have to put in the code to arm
      break;
      
    case ALARM_ARMED:
      if(CheckForRFIDTag(targetID)) 
      {
        Serial.println("Correct tag detected. Disarming.");
        DisarmSystem();
      }
      else if(CheckIfLaserBroken())
      {
        SoundAlarm();
      }
      break;
      
    case ALARM_INTRUDER:
      //you're job to code here
      break;
  }
}

void ArmSystem(void)
{
  //...  
}

void DisarmSystem(void)
{
  //...
}

void SoundAlarm(void)
{
  //...
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

bool CheckForRFIDTag(uint8_t* targetID) 
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


