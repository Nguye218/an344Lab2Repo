/*******************************************************************************
* EECE344 Lab 2
* This lab is a simple interface that allows the user to input a 32 bit address
* for the low address and a 32 bit address for the high address, if both are valid
* then it will go into a another module called MemChkSum() and sum the two 32
* bit values. and returns the user with the sum value. and reset the program
* for new values.
*
* Andy Nguyen, 10/18/2021
*******************************************************************************/
#include "MCUType.h"               /* Include header files                    */
#include "BasicIO.h"
#include "K65TWR_ClkCfg.h"
#include "MemoryTools.h"


#define STRNGLEN 9  		    	 //makes the 9th character an error//

static INT8C InStrngH[STRNGLEN];	 //high input
static INT8C InStrngL[STRNGLEN];	 //low input

static INT32U AddressH; 			 //high address
static INT32U AddressL;			 //low address
static INT16U Answer;				 //check sum answer place holder

typedef enum {GET_LOW_ADDR, GET_HIGH_ADDR} ADDR_STATES_T;
ADDR_STATES_T CurrentState;


typedef enum {VALID, INVALID}ENTRY_T;
ENTRY_T Event;



void main(void){




    K65TWR_BootClock();             /* Initialize MCU clocks                  */

    BIOOpen(BIO_BIT_RATE_115200);   /* Initialize Serial Port                 */

    BIOPutStrg("\n\rHello \n\r");


    while(1){


    	switch (CurrentState){

    	case GET_LOW_ADDR:

            BIOPutStrg("Enter a Low Address: 0x");     						 //UI message

    		if(BIOGetStrg(STRNGLEN,InStrngL)){								// string length check
    			Event = INVALID;
    			BIOPutStrg("Invalid entry. String length exceeded.\n\r");
    			CurrentState = GET_LOW_ADDR;

    		}

    		else if(BIOHexStrgtoWord(InStrngL, &AddressL)!=0){				//check if string is valid
    			Event = INVALID;
    			BIOPutStrg("Invalid entry, Enter another one in the proper range.\n\r");
    			CurrentState = GET_LOW_ADDR;

    		}

    		else{
    			Event = VALID;												//valid case go to high case
    			BIOPutStrg("Valid Entry.\n\r");

    			CurrentState = GET_HIGH_ADDR;

    		}

    		break;



    	case GET_HIGH_ADDR:

    		BIOPutStrg("Enter a High address: 0x");

    		if(BIOGetStrg(STRNGLEN,InStrngH)){								//invalid if string length is exceeded
    			Event = INVALID;
    			BIOPutStrg("Invalid entry. String length exceeded.\n\r");
    			CurrentState = GET_HIGH_ADDR;

    		}

    		else if(BIOHexStrgtoWord(InStrngH, &AddressH)!=0){
    			Event = INVALID;
    			BIOPutStrg("Invalid entry, Enter another one in the proper range.\n\r");
    			CurrentState = GET_HIGH_ADDR;

    		}


    		else if (AddressH < AddressL){
    			Event = INVALID;
    			BIOPutStrg("Invalid entry, High Address is not greater then Low Address.\n\r");
    			CurrentState = GET_LOW_ADDR;
    		}



    		else{
    			Event = VALID; 												//valid state

    			BIOPutStrg("CS: ");
    			BIOOutHexWord(AddressL);
    			BIOPutStrg(" - ");
    			BIOOutHexWord(AddressH);
    			BIOPutStrg(" = ");

    			Answer = MemChkSum((INT8U*)AddressH,(INT8U*)AddressL);		//sets answer equal to the value of the memchksum.

    			BIOOutHexWord(Answer);

    			BIOPutStrg("\n\r");


    			CurrentState = GET_LOW_ADDR;								//returns to low adder state

    		}

    		break;

    	default:															//default case
    		break;

    	}
   }
}
