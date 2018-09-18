// ******************************************************************
//
//    File: App_STD105_Standard.c
//    Date: 2014-01-09
// Version: 1.05
//
// Purpose:
//
// This is the standard app for TWN4 readers, which is installed
// as default app on TWN4 readers. This script can be run on any
// type of TWN4 reader without modification.
// 
// Feel free to modify this program for your specific purposes!
//
// V1:
// ------
// - Initial release
//
// V2: 
// ---
// - Support configuration via AppBlaster
//
// V1.03: 
// ------
// - Changed version numbering scheme
//
// V1.04: 
// ------
// - Configure tags being searched for only if LFTAGTYPES or
//   HFTAGTYPES requests a specific type. Otherwise, default setup
//   by firmware is used.
// - Marked as template for AppBlaster
// - Support ICLASS option
// - Change style how options are evaluated more convenient
//
// V1.05: 
// ------
// - NFCP2P Demo Support
//
// V1.06:
// ------
// - Adaption to reworked NFC SNEP service
//
// ******************************************************************

#include "twn4.sys.h"
#include "apptools.h"

#define MAXIDBYTES  10
#define MAXIDBITS   (MAXIDBYTES*8)

byte ID[MAXIDBYTES];
int IDBitCnt;
int TagType;

byte LastID[MAXIDBYTES];
int LastIDBitCnt;
int LastTagType;

// ******************************************************************
// ****** Section containing configurable behaviour *****************
// ******************************************************************




int main(void)
{
	// Show the startup message
    if (GetHostChannel() == CHANNEL_COM1)
    {
        // A V24 device is writing the version at startup
        HostWriteVersion();
        HostWriteChar('\r');
    }
	// Adjust parameters   
	//ConfigSetParameters();
	// Set tag types
    	
    // Init LEDs
    LEDInit(REDLED | GREENLED);
    // Turn on green LED
    LEDOn(GREENLED);
    // Turn off red LED
    LEDOff(REDLED);
    // Make some noise at startup at low volume
    SetVolume(30);
    BeepLow();
    BeepHigh();
    // Continue with maximum  volume
    SetVolume(100);
    
    // No transponder found up to now
    LastTagType = NOTAG;


	while (true)
	{
        // Search a transponder		
		if (SearchTag(&TagType,&IDBitCnt,ID,sizeof(ID)))
		{			
				if (TagType != LastTagType || IDBitCnt != LastIDBitCnt || !CompBits(ID,0,LastID,0,MAXIDBITS)||TestTimer())
				{
					byte NewID[2];
					byte NewID20[3];
					byte NewSC[1];
					int IDLength = (IDBitCnt/8)-1;
					CopyBits(LastID,0,ID,0,MAXIDBITS);
					LastIDBitCnt = IDBitCnt;
					LastTagType = TagType;
					if(IDBitCnt==26){
					CopyBits(NewID,0,ID,IDBitCnt-17,16);
					CopyBits(NewSC,0,ID,1,8);
					// Yes! Sound a beep
					BeepHigh();
					// Turn off the green LED
					LEDOff(GREENLED);
					// Let the red one blink, start with on-state
					LEDOn(REDLED);
					LEDBlink(REDLED,500,500);
					//HostWriteChar(0x02);
					HostWriteDec(NewSC, 8,7);
					HostWriteDec(NewID, 16,5);
					}
					else if(IDBitCnt==35){
					CopyBits(NewID,0,ID,IDBitCnt-21,20);
					CopyBits(NewSC,0,ID,IDBitCnt-29,8);
					// Yes! Sound a beep
					BeepHigh();
					// Turn off the green LED
					LEDOff(GREENLED);
					// Let the red one blink, start with on-state
					LEDOn(REDLED);
					LEDBlink(REDLED,500,500);
					//HostWriteChar(0x02);
					HostWriteDec(NewSC, 8,5);
					HostWriteDec(NewID20, 20,7);
					}
					// Send Enter
					HostWriteChar('\r'); 
					// Send ETX
					//HostWriteChar(0x03);
					StartTimer(2000);
				}
		}
			
        if (TestTimer())
        {
            LEDOn(GREENLED);
            LEDOff(REDLED);
            LastTagType = NOTAG;
        }
    }
}
