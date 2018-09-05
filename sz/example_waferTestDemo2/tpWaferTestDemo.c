/***************************************************************************
   
                     +-------------------------------+
                     |                               |
                     |     SSSSSSSSS   ZZZZZZZZZ     |
                     |    SSSSSSSSSS   ZZZZZZZZZZ    |
                     |     SSSS             ZZZZ     |
                     |       SSSS         ZZZZ       |
                     |         SSSS     ZZZZ         |
                     |    SSSSSSSSSS   ZZZZZZZZZZ    |
                     |    SSSSSSSSS     ZZZZZZZZZ    |
                     |                               |
                     |     T E S T S Y S T E M E     |
 --------------------|                               |---------------------
                     +-------------------------------+

***************************************************************************/

#define  PROGRAM  "demo"
#define  VERSION  "1.0"

/***************************************************************************
*
*  Device :
*
****************************************************************************
*
*  Version  Index  Date      Author           Comment
* --------------------------------------------------------------------------
*  1.00     001    05.12.01  ks               created
***************************************************************************/


/**************************************************************************/
/* Include files                                                          */
/**************************************************************************/
#include <math.h>
#include "tpWaferTestDemo.h"
#include "/home/weber/sz/progs/prober/enhancedSerial.h"      
                                            /* communication wafer prober */

/**************************************************************************/
/* Constant definitions and macros					  */
/**************************************************************************/

/**************************************************************************/
/* Global Variables                                                       */
/**************************************************************************/

long    firstTestOnWafer, firstTestOnLot;
long    proberControl, proberMessage, runningState, isTestChip;
long    currentXPosition, currentYPosition;

/**************************************************************************/
/* Parameter Index Variables                                              */
/**************************************************************************/



/**************************************************************************/
/* Result Index Variables                                                 */
/**************************************************************************/



/**************************************************************************/
/*                                                                        */
/* General Functions Area                                                 */
/*                                                                        */
/**************************************************************************/

void proberPauseFunction(void);


/**************************************************************************/
/*                                                                        */
/* Teststep Functions Area                                                */
/*                                                                        */
/**************************************************************************/

TS waferTestDemoFunction()
{
    long randNumber;


    randNumber = (long) rand(); /* random number between 0 and 2^15-1 */
	 systemPrintf2("Rand %d\n", randNumber);

CTRL VIS1_1
  SET_VOLTAGE    2
  SET_OUTPUT_RELAY_F    CLOSE
  SET_LINK_FS    CLOSE
END
    doStore(randNumber, waferTestDemoResultId);
    
} /* end of test step 'voltageAt100DegCelTest' */



/**************************************************************************/
/*                                                                        */
/*  Central Routines Area                                                 */
/*                                                                        */
/**************************************************************************/

/**************************************************************************/
/*                                                                        */
/* tpInit                                                                 */
/*                                                                        */
/* This function is called after loading the test program                 */
/* or when the test program is reinitialised.                             */
/* Put your software initialisation in here.                              */
/* The hardware setup should be done in the tpSetupFunction.              */
/*                                                                        */
/**************************************************************************/

long tpInit(void)
{
  long ret = 0;
    
  systemPrintf2("initializing the test program");
    
  tpInfo(PROGRAM,VERSION); 
  /* displays program name and version in the SPACE main window */
  /* see definition in the header (#define PROGRAM ...)         */
    
  /********************************************/
  /* put your software initialisation in here */
  /********************************************/

  return ret;
    
} /* tpInit */


/**************************************************************************/
/*                                                                        */
/* tpSetupFunction                                                        */
/*                                                                        */
/* Function is called, after the test program is loaded and after each    */
/* power down/up of the test system (after tpInit).                       */
/* In this function the general (hardware) setup like the voltmeter       */
/* calibration or the setup of interfaces should be placed                */
/*                                                                        */
/**************************************************************************/

void tpSetupFunction(void)
{
    long r, sub, f, re, o, p, e, a;  /* prober setup variables */
    
    /* 
     * the next three lines are needed for calling the command interpreter 
     * (former onlineCompiler) from the debugger. Don't remove them! 
     */
    if (FALSE) onlineCompiler(); 
    if (FALSE) ci(); 
    if (FALSE) CI();
    
    systemPrintf2("running setup");
    
    
    /*******************************************/
    /*              prober setup               */
    /*******************************************/
    
    systemPrintf2("SETUP PROBER's\n");
    
    firstTestOnWafer = 1; 
    firstTestOnLot   = 1;
    
    if( esSetupProber( DEFAULT_BAUD_RATE ) )
     {
	 proberControl = 0;
     }   
    else
     {   
	 if( ( esGetProberState( &r , &sub , &f , &re , &o , &p , &e , &a ) ) <= 0 )
	     proberControl = 0;
	 else 
	     proberControl = 1;
     }
    
    if( proberControl )
     {
	 systemPrintf2( "prober control: ENABLED\n" );
     }
    else
     {
	 systemPrintf2( "prober control: DISABLED\n" );
     }
    
    /*******************************************/
    
    systemPrintf2("\n");
    systemPrintf2("***************************************\n");
    systemPrintf2("******* TEST can be started now *******\n");
    systemPrintf2("***************************************\n");
   systemPrintf2("\n");
   fflush(stdout);
   
}  /* tpSetupFunction */


/**************************************************************************/
/*                                                                        */
/* tpStartPollFunction                                                    */
/*                                                                        */
/* Function is called while waiting for START (BUTTON)                    */
/*                                                                        */
/* The following return values are allowed                                */
/*                                                                        */
/*   NOTHING_HAPPENED  -   Do NOT run the test                            */
/*   RUN_TEST         -   Run the test                                    */
/*   END_OF_WAFER     -   End of Wafer                                    */
/*   END_OF_LOT       -   End of Lot                                      */
/*   EXIT_TP          -   Leaving the TP                                  */
/*   DUMMY_TEST       -   dummy test                                      */
/*   RETEST           -   starts the retest                               */
/*   RUN_NO_TEST      -   ignore the remote ctrl start                    */
/*                                                                        */
/**************************************************************************/


long UL_startPollFunction(void)
{
    time_t timeStamp;
    static long gotOWC = 0;
    static long counter = 0;
    long ret = RUN_NO_TEST;
    
    if(!proberControl || IS_simMode())
	return NOTHING_HAPPENED;
    proberMessage = esAskForStart(&gotOWC);
    
    do
     {
	 switch (proberMessage)
	  {
	      /*START MESSAGES*/
	    case ES_START_MSG :
		return RUN_TEST;
		
	    case ES_RETEST_MSG :
		return RETEST;
		
	    case ES_TEST_AGAIN_MSG :
		return RUN_TEST;
		
	    case ES_TEST_FIRST_DIE_MSG :
		timeStamp = time(0);
		systemPrintf2("FIRST DIE OF WAFER at %s\n", ctime(&timeStamp)+11);
		return RUN_TEST;
		
		/*OTHER MESSAGES*/

		/* if PATTERN_COMPLETE message is enabled */
		/* ES_EOW_MSG was sent twice              */
	    case ES_EOW_MSG :
		if (runningState == 1)
		 {
		     runningState = 2;               
		     timeStamp = time(0);
		     systemPrintf2("PATTERN COMPLETE at %s\n", ctime(&timeStamp)+11);
		     return END_OF_WAFER;            
		 }
		else
		 {
		     runningState = 2;               
		     timeStamp = time(0);
		     systemPrintf2("END OF WAFER at %s\n", ctime(&timeStamp)+11);
		     return END_OF_WAFER;            
		 }
		break;                    
		
	    case ES_BEGIN_CASSETTE_MSG :
		systemPrintf2("BEGIN OF CASSETTE\n");
		break;
		
	    case ES_END_CASSETTE_MSG :
		systemPrintf2("END OF CASSETTE\n");
		return END_OF_LOT;
		break;
		
	    case ES_WAFER_BEGIN_MSG :
		runningState = 1;          /* added by Klaus Schneider 06/19/00 */
		timeStamp = time(0L);      /* EOW message occurs two times      */
		/* because of END_OF_PATTERN and END_OF_WAFER */
		systemPrintf2("BEGIN OF WAFER at %s\n", ctime(&timeStamp)+11);
	        break;	
	    case ES_PAUSE_CONTINUE_MSG :
		systemPrintf2("PROBER SET TO PAUSE\n");
		proberPauseFunction();    /* added by Klaus Schneider 10/05/01 */
		break;
		
	    case ES_CONTINUE_MSG :
		break;
		
	    case ES_TEST_MICRO_SITE_MSG :
		isTestChip = 1;
		return RUN_TEST;
		break;
		
	    case ES_ALARM_MSG :
	    case ES_ATTENTION_MSG :
	    case ES_START_PROBING_MSG :
	    case ES_TEST_COMPLETE_MSG :
	    case ES_PAUSE_PENDING_MSG :
	    case ES_PERFORM_CONTI_TEST_MSG :
	    case ES_UGLY_DIE_REPORT_MSG :
	    case ES_MAP_RECEIVED_MSG :
	    case ES_MAP_TRANSMITTED_MSG :
		break;
	  }
     }
    while ((proberMessage = esAskForUnsolicitedMessages()) > 0);
    
    return NOTHING_HAPPENED;
    
} /* UL_startPollFunction */



long tpStartPollFunction(void)
{
    if( proberControl )
     {
	 return UL_startPollFunction();
     }
    else
     {
	 return NOTHING_HAPPENED;
     }
}


void proberPauseFunction(void)
{
    systemPrintf2("prober set to pause"); 
    if( firstTestOnWafer == 0 )
     {
	 
     }
    
}


/**************************************************************************/
/*                                                                        */
/* tpStartFunction                                                        */
/*                                                                        */
/* Function is called, when a new test run is started                     */
/*									  */
/* Only here a change of the test mode (normal test, dummy test, retest)  */
/* is allowed !                                                           */
/*                                                                        */
/* CAUTION:                                                               */
/* The setting of statistical values and any counters is NOT allowed !    */
/*                                                                        */
/**************************************************************************/

void tpStartFunction(void)
{
    long                  xCoord, yCoord;
    
    if ( proberControl )
     {
	 esAskForCoordinates( &xCoord , &yCoord);
     }
    
    currentXPosition = (long) xCoord;
    currentYPosition = (long) yCoord;
    
    
    if( firstTestOnLot == 1 )
     {
	 firstTestOnLot = 0;
     }
    
    if( firstTestOnWafer == 1 )
     {
	 firstTestOnWafer = 0;
     }
} /* tpStartFunction */



/**************************************************************************/
/*                                                                        */
/* tpFailFunction                                                         */
/*                                                                        */
/* Function is called at the end of a test run, when the test is FAIL     */
/*                                                                        */
/* Put a hardware reset of the used instruments in here to ensure a       */
/* defined state of the hardware after "stop at fail"                     */
/*                                                                        */
/**************************************************************************/

void tpFailFunction(void)
{
    
} /* tpFailFunction */


/**************************************************************************/
/*                                                                        */
/* tpPassFunction                                                         */
/*                                                                        */
/* Function is called at the end of a test run, when the test is PASS     */
/*                                                                        */
/**************************************************************************/

void tpPassFunction(void)
{
    
} /* tpPassFunction */


/**************************************************************************/
/*                                                                        */
/* tpEndFunction                                                          */
/*                                                                        */
/* Function is called at the end of a test run, regardless of the test    */
/* result.                                                                */
/*                                                                        */
/* Set your hardware to a defined (and secure) state here (HW set down)   */
/*                                                                        */
/**************************************************************************/

void tpEndFunction(void)
{ 
  long bin;
  if (proberControl)
	{
	  bin = tpGetActualHwBin();  /* get HW bin from definition part */
	  /* for actual test run             */
	  esSendBinToProber(1,&bin); /* send HW bin for actual test run */
	  /* to wafer prober                 */
	}
    
} /* tpEndFunction */


/**************************************************************************/
/*                                                                        */
/* tpEndOfWaferFunction                                                   */
/*                                                                        */
/* Function is called if the tpStartPollFunction returns END_OF_WAFER     */
/*                                                                        */
/**************************************************************************/

void tpEndOfWaferFunction(void)
{
    systemPrintf2("end of wafer");
    
} /* tpEndOfWaferFunction */


/**************************************************************************/
/*                                                                        */
/* tpEndOfLotFunction                                                     */
/*                                                                        */
/* Function is called if the tpStartPollFunction returns END_OF_LOT       */
/*                                                                        */
/**************************************************************************/

void tpEndOfLotFunction(void)
{
  systemPrintf2("end of lot");
    
} /* tpEndOfLotFunction */


/*************************************************************************/
/*                                                                       */
/* tpPostSeqChangedFunction                                              */
/*                                                                       */
/* This function is called after a sequencer has been changed.           */
/*                                                                       */
/*************************************************************************/

void tpPostSeqChangedFunction(void)
{
    systemPrintf2("sequencer was changed");
    
} /* tpPostSeqChangedFunction */


/**************************************************************************/
/*                                                                        */
/* tpRelease                                                              */
/*                                                                        */
/* Function is called before the test program is unloaded or the test     */
/* system is reinited                                                     */
/*                                                                        */
/* Put a complete hardware reset of the used instruments in here.         */
/*                                                                        */
/**************************************************************************/

long tpRelease(void)
{
    long ret = 0;
    
    systemPrintf2("releasing the test program");
    
    return ret;
    
} /* tpRelease */


/**************************************************************************/
/*                                                                        */
/* Main function of the test program                                      */
/*                                                                        */
/**************************************************************************/

void main(int argc, char *argv[])
{
    tpmSetEnv(argc, argv);
    
    /*
    ** reading sysParFile
    ** replace NULL with your own sysParFile (optional)
    */
    tpmMain(argc,argv,NULL);
    
} /* main */


/**************************************************************************/
/***************** end of test program ************************************/
/**************************************************************************/
