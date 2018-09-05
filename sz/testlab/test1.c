/**************************************************************************

                                 ##
                                 ##
                       ####   #####  ####
             #  ###  ##   # ##   ## ##   # # ###   ####
       ####  ###  ## ##   # ##   ## ##   # ###  # ##  ##  ####
      ##  ## ##      #####  ##   ## #####  ##   # ##  ## ##   #
      ##     ##      ##     ##   ## ##     ##   # ##     #####
      ##  ## ##      ##   # ##   ## ##   # ##   # ##  ## ##
       ####  ##       ####   #####   ####  ##   #  ####   ####


                        #########  #########
                       ##########  ##########
                        ####            ####
                          ####        ####
                            ####    ####
                       ##########  ##########
                       #########    #########

**************************************************************************/

#define  PROGRAM  "template"
#define  VERSION  "1.00"

#define  USE_SPACE_VERSION 701

/***************************************************************************
*
*  Device :
*
****************************************************************************
*
*  Version  Index  Date      Author       Comment
* --------------------------------------------------------------------------
*  1.00     001              SW             created
*
***************************************************************************/


/**************************************************************************
**                                                                        
** Declaration Area                                           
**                                                                        
***************************************************************************/

/**************************************************************************
** Include files                                                          
***************************************************************************/

#include </home/weber/sz/test1/ADC_TEST_NEW.h> /*DSS OUTPUT_CONVERTER WM*/
#include <tp.h>
#include <szDsp.h>
#include <dssSeq.h>/*for BI DSS_Sequencer*/
#include <dss.h>/*for DSS MI*/
#include <dssMem.h>/*for BI DSS_MEMORY*/
#include <dspTools.h>/*for DSP_OS*/
//#include <sdpVO.h>/*for serial Data Processor*/

/**************************************************************************
** Constant definitions and macros
**************************************************************************/


/**************************************************************************
** Global Variables                                                       
***************************************************************************/

double sigFrequ = 97.6562;


/**************************************************************************
** Parameter Index Variables                                              
***************************************************************************/
static long  	debCycleParId;	/* cycle mode for dss */

long    	testButtonParId, numberParId, doShortParId;


static long 	voltageParId;			   /*VIS Voltage*/

static long   	sigFreqParId;                      /* HRSG signal frequency */
static long   	sigAmplParId;                      /* HRSG signal amplitude */
static long   	sigDcBaseParId;                  /* HRSG signal DC baseline */
static long   	lpfParId;        /* HRSG and HRSD low pass filter frequency */
static long	clkFreqParId;			/*clock Frequency*/
static long     incrementParId;                           /* HRSG increment */
static long	sigRangeParId;			/*HRSG Range*/


/**************************************************************************
** Result Index Variables                                                 
***************************************************************************/

/*long   i1ResId, i2ResId, impedanceResId,
              i1_1ResId, i2_1ResId, imp_1ResId,
              i1_2ResId, i2_2ResId, imp_2ResId,
              i1_3ResId,
              i1_4ResId,
              i1_5ResId;*/
	      

static long  	voltageResId;    
static long  	voltageResId2;    
static long	sigFreqResId;
  
     

/**************************************************************************
**                                                                        
** General Functions Area                                                 
**                                                                        
***************************************************************************/

/* add your self defined functions here */

static void executePattern (char *startLabel, long *vectCount, long *failCount)  
{
   if (bPar(debCycleParId))  
   {
      CTRL DSS1 
	 CLEAR_FAIL
	 SET_START_LABEL	startLabel
	 FLUSH 
	 DO_START 
	 END;
      
      CTRL DSS1 	 
	 SET_CYCLE_MODE    AUTO    
	 END;
      
      CTRL DSS_SEQ1
	 DO_START
	 END;
      
      getAnswer("STOP DSS CYCLE MODE"); 
      
      CTRL DSS_SEQ1
	 DO_STOP
	 END;
      
      CTRL DSS1 	 
	 SET_CYCLE_MODE    SINGLE    
	 END;
     
      *failCount=0;
      *vectCount=0;   
   } 
   else  
   {
      CTRL DSS1
      	 CLEAR_FAIL                   
	 SET_START_LABEL	startLabel
	 FLUSH 
	 DO_START 
	 FLUSH    
	 GET_FAIL_COUNT 	failCount  
	 GET_VECT_COUNT 	vectCount   
	 END;      
   }
   

} /* executePattern */










/**************************************************************************
**                                                                        
** Test Step Functions Area                                                
**                                                                        
***************************************************************************/


TS powerStart()
{

   double voltage;                                                              
   
   voltage = 5.0;
                                                        
	/* CTRL VIS2_2

	END
                   
	CTRL SOURCES
  		DO_FOR_INST 	VIS2_2
  		SET_SGT_SOFT    	TRUE
  		SET_CGT_SOFT    	TRUE
  		SET_OUTPUT_RELAY_F    ON
  		SET_OUTPUT_RELAY_S    ON
	END*/

/**************************SETUP_POWER_SUPPLY***************************************************/

	/******************CURRENT 200mA**********************/
/*	
  	CTRL SOURCES
     		DO_FOR_INST 		VIS2_2
     		SET_VOLTAGE    		10
     		SELECT_CLAMP_RANGE    	0.2
     		SET_CLAMP_P    		0.2
     		SET_CLAMP_N		0.0
     		SET_SGT_SOFT    	TRUE
     		SET_CGT_SOFT    	TRUE
     		SET_MGT_SOFT    	FALSE
     		CONNECT    	FORCE_HIGH 	TO COLUMN1
     		CONNECT    	SENSE_HIGH	TO COLUMN1
     		CONNECT    	FORCE_LOW	TO COLUMN2
     		CONNECT    	SENSE_LOW	TO COLUMN2
   	END;
*/	
	

   	/******************CURRENT 1000mA*********************/
   
	CTRL MPCM1
		DO_FOR_ROW_F    ROW5
		ADD_COLUMN    COLUMN1
		DO_FOR_ROW_S    ROW5
		ADD_COLUMN    COLUMN1
		DO_FOR_ROW_F    ROW6
		ADD_COLUMN    COLUMN2
		DO_FOR_ROW_S    ROW6
		ADD_COLUMN    COLUMN2
	END
	CTRL SOURCES
  		DO_FOR_INST    		VIS51_1
  		SELECT_DRIVE_RANGE    51
  		SELECT_CLAMP_RANGE    0.5
  		SET_CLAMP_P    0.7
  		SET_VOLTAGE    5
  		SET_SGT_SOFT    TRUE
  		SET_CGT_SOFT    TRUE
     		SET_MGT_SOFT    FALSE
	END;
	
	
	doStore(voltage, voltageResId);
	

	systemPrintf2("Power activated..."); 

}

TS powerStop() {

	double voltage;                                                              
   
	voltage = 0.05;

	CTRL VIS51_1
  		SET_SGT_SOFT    FALSE
	END

	doStore(voltage, voltageResId2);

	systemPrintf2("Power deactivated..."); 

}

TS triangleStart()
{

	
/********************MEM_DECL**************************************************************************/

	FILE *fp;
	
	long 	vectCount, failCount;
	
	long tmpVar;
	long numberOfVectors = 20;
	long bitIdx, vectorIdx;
	
	char driveChar, senseChar;
	
	
	static unsigned short	*memValueSenseLow;   
	static unsigned short	*memValueSenseHigh;   
	static unsigned short	*memValueSenseCtrl;   
	
	

	memValueSenseLow = ( unsigned short * ) calloc( numberOfVectors , sizeof( unsigned short ) );
	memValueSenseHigh = ( unsigned short * ) calloc( numberOfVectors , sizeof( unsigned short ) );
	memValueSenseCtrl = ( unsigned short * ) calloc( numberOfVectors , sizeof( unsigned short ) );
	
	


/***********************SETUP CLOCK_SOURCES************************************************************/
	
	/*CTRL CLOCK_SOURCE1
		
		DO_FOR_DIVIDER 		CHANNEL_B
		DO_FOR_INPUT		CLOCK
		SELECT_INPUT_SOURCE	DIVIDER_CHANNEL_A_SQUARE
		SET_DIVIDER_RATIO	223
		SELECT_MODE		NO_FUNCTION

	END;
		
		
	CTRL CLOCK_SOURCE1
		DO_FOR_DIVIDER 		CHANNEL_A
		DO_START
			
		DO_FOR_DIVIDER		CHANNEL_B
		DO_START
	END;
	




/************************SETUP_DSP_MASTER---WAVE_GENERATION********************************************/
  	
	/*CTRL DSP_MASTER
     		DO_FOR_INST    	 	HRSG1
		SELECT_WAVE    	 	"SINE"
     		CONNECT   	 	OUTPUT_POSITIVE		TO    	CB1_HF
     		CONNECT    	 	OUTPUT_NEGATIVE		TO    	CB_GND_SENSE
     		SET_AMPLITUDE   	dPar(sigAmplParId)        //1.490
		SELECT_RANGE	 	dPar(sigRangeParId)	  //1.250
     		SET_DC_BASELINE      	dPar(sigDcBaseParId)	  // 0
     		CONNECT    		CLOCK			TO   	DIVIDER_CHANNEL_B_SQUARE 
     		SET_FREQUENCY    	dPar(sigFreqParId)	  //1013.5
		SET_CLOCK_FREQUENCY     dPar(clkFreqParId)	  //41513
  		SET_LPF_FREQUENCY   	dPar(lpfParId)	          //2000
		SET_INCREMENT    	lPar(incrementParId)	  //25
		SET_MIN_SAMPLES_PER_WAVE    40
		DO_START    
	END;
	
	doStore(dPar(sigFreqParId,sigFreqResId);
	
	printf("DSP activated...\n");   */

   
   
   
   
   
   
   
   
/*****************************SETUP_DSS****************************************************************/
	
	
   tpBreakpoint(tpGetActualTestStep(), "functionTest before pattern execution");
      
   executePattern ("LABEL_WAIT", &vectCount, &failCount);
 
   tpBreakpoint(tpGetActualTestStep(), "functionTest after pattern execution");



      

	 
      

     
	
	
/********************************SETUP DSS_MEM*********************************************************/
	
	CTRL DSS_MEM1
		SET_MEM_ADDR		0x300000
		SET_BUFFER_ADDR	memValueSenseLow
		GET_MEMORY			numberOfVectors
	END;

	CTRL DSS_MEM1
		SET_MEM_ADDR		0x400000
		SET_BUFFER_ADDR	memValueSenseHigh
		GET_MEMORY			numberOfVectors
	END;

	CTRL DSS_MEM1
		SET_MEM_ADDR		0x500000
		SET_BUFFER_ADDR	memValueSenseCtrl
		GET_MEMORY			numberOfVectors
	END;
	
	
   bitIdx = 2;
	systemPrintf2("       -> D S - Pin %d", bitIdx+1);

	for (vectorIdx = 12; vectorIdx < numberOfVectors ; vectorIdx++)
		{
		senseChar = '?';




/** Sense BIT print**/

		/*if(		((1<<bitIdx) & ~memValueSenseLow[vectorIdx])&&
					((1<<bitIdx) & memValueSenseHigh[vectorIdx]))
			{
				senseChar = 'H';
			}
		else if(	((1<<bitIdx) & memValueSenseLow[vectorIdx])&&
					((1<<bitIdx) & ~memValueSenseHigh[vectorIdx]))
			{
				senseChar = 'L';
			}
		else if(	((1<<bitIdx) & ~memValueSenseLow[vectorIdx]) &&
					((1<<bitIdx) & ~memValueSenseHigh[vectorIdx]))
			{
				senseChar = 'Z';
			}*/


			systemPrintf2("  -> %c |    %4X", senseChar, \
				(memValueSenseHigh[vectorIdx])); 

		}

	
	systemPrintf2(" ");

/*********************Save recorded Data in File*******************************************************/

	
	systemPrintf2("Writing sample data file.");
		fp=fopen("sampled_data.txt", "w+");
		fprintf(fp, "# Testing...\n");
		fprintf(fp, "# Vector\t SenseHigh\n");

		tmpVar=0;

		for (vectorIdx = 12; vectorIdx < numberOfVectors; vectorIdx++) {
			printf(".");
				if(tmpVar++ == 20) {
					printf("  %d\n",vectorIdx+1);
					tmpVar=0;
				}
			fprintf(fp, "%08x\t %04x\n", vectorIdx, 
			memValueSenseHigh[vectorIdx]);
			/*memValueSenseLow[vectorIdx], memValueSenseHigh[vectorIdx],
			memValueSenseCtrl[vectorIdx]);*/
		}

	printf("\n%d \n", vectorIdx);

	systemPrintf2("Sample data file written and closed.");

	fclose(fp);
	
	free(memValueSenseLow);
	free(memValueSenseHigh);
	free(memValueSenseCtrl);
	

	
	
	
	
/*******************************SETUP_MPIN for Debug Purpose*******************************************/
	
	/*CTRL MPIN
		DO_FOR 	MD8
		REMOVE_FROM    		SENSE
		CONNECT_TO		DRIVE
		SET_VIH    		5
		SET_DR_FORMAT     	LOGIC_ZERO
  		DO_FOR_LINE    		DRIVE
  		SET_DATA_SOURCE    	LOGIC_ONE
 		DO_FOR_LINE    		OUT_CTRL
  		SET_DATA_SOURCE   	LOGIC_ZERO
		SET_DR_FORMAT    	TRANSPARENT
	END;
	
	CTRL MPIN
		DO_FOR MD3
  		REMOVE_FROM    DRIVE
		REMOVE_FROM    SENSE
  		DO_FOR   MS3
  		CONNECT_TO    SENSE
	END;*/	
	
		
	/*SETUP MPINs for digital DATA READ
	
	for (count =0; count <14:count++)
		{	
			CTRL MPIN
				DO_FOR 		MS1 + count
				SET_SE_FORMAT	TRANSPARENT
				DO_FOR_LINE	SENSE_LINE
			END:
		}
	*/
	
	/* connect lines to sense of Mpins
	for (count =0;count <=14; count ++){
		CTRL DSS_MEM1
			DO_FOR_LINE  SENSE_HIGH1 +count
		END
	*/



//tpBreakpoint( tpGetActualTestStep(), "HRSG1: output of wave \n Frequency: 97.6562 kHz " );

 
}

/*TS exampleTest0()
{
  /*
   * This is a small example for a test step without practical meaning.
   * Replace it by your own test steps.
   * It generates two random numbers of the type double with the function
   * randomValue(meanValue,deviation) and calculates a result named 
   * "impedance" using a parameter value.
   * The parameter value is read with the function dPar(parameterParId);
   * The results are stored with the doStore function 
   * ( doStore(result,resultResId) )
   * The variable "result" will be typcasted in a double, whereas the 
   * resultResId has to be defined as a global variable of the type
   * long. 
   * The index variables (parameterParId, resultResId) are necessary for
   * the test program manager to handle the results and the parameters.
   * The values of these index variables are assigned automatically by the
   * test program manager. Therefore the pointers to (=adresses of) these
   * variables have to be declared in the Definition Area.
   
   
   double i1, i2, impedance;
   
  
   i1 = randomValue(0.001,0.0002);
   i2 = randomValue(0.001,0.0005);
   
   if(i2 > 1.1)   /* to avoid division by zero ! 
   {
      impedance = (dPar(voltageParId) / i2);
   }
   else 
   {
      impedance = 99999; 
      /* just a number that can be seen easily in the result display 
   }

   doStore(i1,i1ResId);
   doStore(i2,i2ResId);
   doStore(impedance,impedanceResId);

} /* exampleTest0 */


/* the following test steps are only examples which
   are used to explain how sequencer work 

TS exampleTest1()
{
   double i1_1, i2_1, imp_1;
   
   i1_1 = randomValue(0.001,0.0002);
   i2_1 = randomValue(0.001,0.0005);
   
   if(i2_1 > 0.0)   /* to avoid division by zero ! 
   {
      imp_1 = (dPar(voltageParId) / i2_1 );
   }
   else 
   {
      imp_1 = 99999; 
      /* just a number that can be seen easily in the result display *
   }

   doStore(i1_1, i1_1ResId);
   doStore(i2_1, i2_1ResId);
   doStore(imp_1,imp_1ResId);

} /* exampleTest1 

TS exampleTest2()
{   
   double i1_2, i2_2, imp_2;
   
   i1_2 = randomValue(0.001,0.0002);
   i2_2 = randomValue(0.001,0.0005);
   
   if(i2_2 > 0.0)   /* to avoid division by zero ! 
   {
    imp_2 = (dPar(voltageParId) / i2_2 ); 
   }
   else 
   {
      imp_2 = 99999; 
      /* just a number that can be seen easily in the result display 
   }

   doStore(i1_2, i1_2ResId);
   doStore(i2_2, i2_2ResId);
   doStore(imp_2,imp_2ResId);

} /* exampleTest2 

TS exampleTest3()
{
   double i1_3;
   
   i1_3 = randomValue(0.001,0.0002);
   doStore(i1_3, i1_3ResId);
} /* exampleTest3 

TS exampleTest4()
{
   double i1_4;
   
   i1_4 = randomValue(0.001,0.0002);
   doStore(i1_4, i1_4ResId);
} /* exampleTest4 

TS exampleTest5()
{
   double i1_5;
   i1_5 = randomValue(0.001,0.0008);
   doStore(i1_5, i1_5ResId);
} /* exampleTest5 */


/**************************************************************************
**                                                                        
**  Event Function Area (used in regular test program flow)
**                                                                        
***************************************************************************/


/**************************************************************************
**                                                                        
** tpPreDefinitionFunction                                                
**                                                                        
** This function is called while loading the test program, before scanning 
** the Definition Area, before calling tpInit                             
**                                                                        
***************************************************************************/
void tpPreDefinitionFunction()
{
}  /* end of tpPreDefinitionFunction */


/**************************************************************************
**                                                                        
** tpPreDefPartFunction                                                   
**                                                                        
** This function is called as the first function within the Definition Area 
** of the test program                                               
** It can be used to define global parameters or setups.
** ATTENTION: This function will be called 3 times before test program is  
** loaded completely                                                        
**                                                                        
***************************************************************************/
void tpPreDefPartFunction()
{
}  /* end of tpPreDefPartFunction */


/**************************************************************************
**                                                                        
** tpPostDefPartFunction                                                  
**                                                                        
** This function is called before the end of the Definition Area of the   
** test program                                                           
** ATTENTION: This function will be called 3 times before test program is  
** loaded completely                                                        
**                                                                        
***************************************************************************/
void tpPostDefPartFunction()
{
}  /* end of tpPostDefPartFunction */


/*************************************************************************
** 
** tpPowerUpFunction 
**
** This function is called when power up was detected.
** The sequence of called functions is
** - tpRelease()
** - IS will be closed and opened
** - tpInit()
** - tpPowerUpFunction()
** - tpSetupFunction()
** In this function the general (hardware) setup like the voltmeter       
** calibration or the setup of interfaces should be placed                
**
**************************************************************************/
void tpPowerUpFunction()
{
   /* hardware setup */
   
} /* tpPowerUpFunction */


/*************************************************************************
** 
** tpPowerDownFunction 
**
** This function is called when power down was detected. 
** The sequence of called functions is
** - tpRelease()
** - IS will be closed and opened
** - tpInit()
** - tpPowerDownFunction()
** - tpSetupFunction()
**
**************************************************************************/
void tpPowerDownFunction()
{
      
} /* tpPowerDownFunction */ 


/*************************************************************************
** 
** tpLoadFunction 
**
** This function is called only ONCE when the test program is loaded, 
** before calling tpInit()
** It is recommented to place the software initialization here.
**
**************************************************************************/
void tpLoadFunction()
{
  systemPrintf2("initializing the test program");

  tpInfo(PROGRAM,VERSION); 
  /* displays program name and version in the SPACE main window */
  /* see definition in the header (#define PROGRAM ...) */
  
  /* put your software initialisation in here */

  
  systemPrintf2("done");
   
 
   /* 
   ** depending on simulation mode, the tpPowerUp/DownFunction() can be 
   ** called to set the hardware to a defined (and secure) state  
   */
   if (IS_simMode()) /* power is down */
   {
      tpPowerDownFunction(); 
   }
   else              /* power is up */
   {
       tpPowerUpFunction(); 
   }
     
   printf("\n");
   printf("***************************************\n");
   printf("******* TEST can be started now *******\n");
   printf("***************************************\n");
   printf("\n");
   fflush(stdout);
   
   {

/***********************************************************************
  This code comes from TP_LBRARY: dcVoltageVm   Version 1.00
************************************************************************/
   
   systemPrintf2("calibrating the Voltmeter");

   CTRL VM
      DO_CALIBRATE
   END;
   
   	 /* CTRL DSS1
   		DO_CREATE_DEBUG_WDB     DEBUG
	END;*/
   




   /*systemPrintf2("setting up the DSP");
   CTRL DSP_MASTER
     DO_FOR_INST     HRSG1
     SELECT_WAVE    "TRIANGLE"
     CONNECT    OUTPUT_POSITIVE
     TO    CB1_HF
     CONNECT    OUTPUT_NEGATIVE
     TO    CB_GND_SENSE
     SET_AMPLITUDE    5
     SET_DC_BASELINE    0
     CONNECT    CLOCK
     TO    CLOCK_LINE1
     SET_FREQUENCY     sigFrequ
     SET_CLOCK_FREQUENCY    100000
     SET_LPF_FREQUENCY    20000
     SET_INCREMENT    1
     SET_DC_BASELINE    0
   END

   systemPrintf2("setting up the clock");
   CTRL CLOCK_SOURCE1
     DO_FOR_DDS    CHANNEL_A
     SET_DDS_FREQUENCY1    100000
     DO_FOR_OUTPUT    CLOCK_CON1
     SELECT_OUTPUT_SOURCE    DDS_CHANNEL_A
   END

   systemPrintf2("setting up the power supply");
   CTRL SOURCES
     DO_FOR_INST 	VIS2_2
     SET_VOLTAGE    	10
     SELECT_CLAMP_RANGE    0.2
     SET_CLAMP_P    	0.2
     SET_CLAMP_N		0.0
     SET_SGT_SOFT    	FALSE
     SET_CGT_SOFT    	FALSE
     SET_MGT_SOFT    	FALSE
     CONNECT    	FORCE_HIGH 	TO COLUMN1
     CONNECT    	SENSE_HIGH	TO COLUMN1
     CONNECT    	FORCE_LOW	TO COLUMN2
     CONNECT    	SENSE_LOW	TO COLUMN2
   END*/
}
} /* tpLoadFunction */


/**************************************************************************
**                                                                        
** tpInit                                                                 
**                                                                        
** This function is called after loading the test program,                 
** when the test program is reinitialised and at power up/down of the     
** system. 
** Please, use the tpLoadFunction() instead of this function.
**                                                                        
***************************************************************************/
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

	
	
	
	/* CTRL DSS1
   		DO_CREATE_DEBUG_WDB     DEBUG
	END;*/
   



  	return ret;
	
	/*bool_t correctHwInstalled;
	
	printf("Initialize test program");
	tpInfo("tpName","Version 1,00");
	
	CTRL CB10
		ADD_VALID_CB_TYPE      1
		ADD_VALID_CB_VERSION   0
		ADD_VALID_CB_VERSION   1
		/*ADD_VALID_DUT_BOARD_ID 0x21
		ADD_VALID_DUT_BOARD_ID 128
	END;
	
	CTRL CB10
		GET_HARDWARE_STATE &correctHwInstalled
	END;
	
	printf ("%b", corr
	
	/*if (!correctHwInstalled)
	reinit();*/
	

} /* tpInit */


/**************************************************************************
**                                                                        
** tpSetupFunction                                                        
**                                                                        
** Function is called, after the test program is loaded and after each    
** power down/up of the test system (after tpInit).
** Please, use the tpPowerUpFunction() instead of this function. 
**                                                                        
**************************************************************************/
void tpSetupFunction(void)
{   
	/* 
     	* the next three lines are needed for calling the command interpreter 
     	* (former onlineCompiler) from the debugger. Don't remove them! 
     	*/
    if (FALSE) onlineCompiler(); 
    if (FALSE) ci(); 
    if (FALSE) CI();
    
    systemPrintf2("running setup");
    
    
  /****************************************************************/
   
   systemPrintf2("enabling AUTOROUTER ...");
   
   CTRL AUTOROUTER
      SET_MODE 	ENABLE
      END;
      
   systemPrintf2 ("enabling AUTOROUTER done ");
      
 /******************************************************************/
    
    
/***********************DOWNLOAD DSS-PATTERN FILE*************************/

   #define DSSFILE "/home/weber/sz/test1/ADC_TEST_NEW.dss"

   systemPrintf2("downloading dss file " DSSFILE);
  
   CTRL DSS1   
      DO_DOWN_LOAD 	(DSSFILE)
      SET_INIT_PLATE 	TRUE
      DO_APPLY_IDLE_STATE
   END;
      
   systemPrintf2 ("downloading dss file done ");
    
    	/*CTRL DSS1
		DO_DOWNLOAD 	"/home/weber/sz/test1/record.dss"
	END;*/
	
	
   

}  /* tpSetupFunction */


/*************************************************************************
**                                                                        
** tpStartPollFunction                                                    
**                                                                        
** Function is called while waiting for START (BUTTON)                    
**                                                                        
** The following return values are available                                
**                                                                        
**   NOTHING_HAPPENED -   do NOT run the test                            
**   RUN_TEST         -   run the test                                    
**   END_OF_WAFER     -   calls tpEndOfWaferFunction()
**   END_OF_LOT       -   calls tpEndOfLotFunction()                                     
**   EXIT_TP          -   terminates TP                                  
**   DUMMY_TEST       -   dummy test                                      
**   RETEST           -   starts retest                               
**   RUN_NO_TEST      -   ignore remote ctrl start                    
**                                                                        
**************************************************************************/
long tpStartPollFunction(void)
{
   return (NOTHING_HAPPENED);

} /* tpStartPollFunction */


/**************************************************************************
**                                                                        
** tpStartFunction                                                        
**                                                                        
** Function is called, when a new test run is started                     
**
** Only here a change of the test mode (normal test, dummy test, retest)  
** is allowed !                                                           
**                                                                        
** CAUTION:                                                               
** The setting of statistical values and any counters is NOT allowed !    
**                                                                        
**************************************************************************/
void tpStartFunction(void)
{
} /* tpStartFunction */


/*************************************************************************
**                                                                        
** tpAlarmFunction                                                        
**                                                                        
** Function is called if there was a HW ALARM after a test run             
**                                                                        
**************************************************************************/
void tpAlarmFunction(void)
{
} /* tpAlarmFunction */



/**************************************************************************
**                                                                        
** tpFailFunction                                                         
**                                                                        
** Function is called at the end of a test run, when the test result was FAIL
**                                                                        
** Put a hardware reset of the used instruments in here to ensure a       
** defined state of the hardware after "Stop At Fail"                     
**                                                                        
**************************************************************************/
void tpFailFunction(void)
{
} /* tpFailFunction */


/**************************************************************************
**                                                                        
** tpPassFunction                                                         
**                                                                        
** Function is called at the end of a test run, when the test result was PASS
**                                                                        
**************************************************************************/
void tpPassFunction(void)
{
} /* tpPassFunction */


/*************************************************************************
**                                                                        
** tpEndFunction                                                          
**                                                                        
** Function is called at the end of a test run, regardless of the test    
** result.                                                                
**                                                                        
** Set your hardware to a defined (and secure) state here (HW set down)   
**                                                                        
**************************************************************************/
void tpEndFunction(void)
{

	CTRL DSS1
      		DO_APPLY_IDLE_STATE
      	END;

   
      /* close debug waveform database
      CTRL DSS1 
	 DO_CLOSE_DEBUG_WDB 
	 SET_DEBUG_MODE    	FALSE    
	 END; */

} /* tpEndFunction */


/*************************************************************************
**                                                                        
** tpEndOfWaferFunction                                                   
**                                                                        
** Function is called if the tpStartPollFunction returns END_OF_WAFER     
**                                                                        
**************************************************************************/
void tpEndOfWaferFunction(void)
{
} /* tpEndOfWaferFunction */


/*************************************************************************
/*                                                                        
/* tpEndOfLotFunction                                                     
/*                                                                        
/* Function is called if the tpStartPollFunction returns END_OF_LOT       
/*                                                                        
/*************************************************************************/
void tpEndOfLotFunction(void)
{
} /* tpEndOfLotFunction */


/**************************************************************************
**
** tpRelease
**
** Function is called before IS will be closed at
** - the test program is unloaded
** - power up/down was detected
** - the test system is reinitialized.
**
** Put a user defined hardware reset of the used instruments in here.
**
***************************************************************************/

long  tpRelease(void)
{


   long ret = 0;
   
  /****************************************************************/
   
  systemPrintf2("releasing AUTOROUTER ...");
   
  CTRL AUTOROUTER
     SET_MODE 	DISABLE
     END;
      
  systemPrintf2 ("releasing AUTOROUTER done");
      
  /****************************************************************/
   
  systemPrintf2("releasing the test program");


   systemPrintf2("done");

   return ret;


} /* tpRelease */


/*************************************************************************
** 
** tpUnloadFunction 
**
** This function is only called when terminating the test program, after 
** calling tpRelease() and before IS will be closed.
**
**************************************************************************/
void tpUnloadFunction()
{
   
  systemPrintf2("releasing the test program");
  
  /* place your statements here */
  

  systemPrintf2("done");

}


/**************************************************************************
**                                                                        
**  Event Function Area (special event functions)
**                                                                        
***************************************************************************/


/*************************************************************************
**                                                                        
** tpUserFunction                                                         
**                                                                        
** This function is called, when the user is activating one of the User     
** Buttons. 
** The parameter is the ID of the user button, which is specified in the
** Definition Part.
**
***************************************************************************/
void tpUserFunction(long id, long state)
{
   
   switch (id)
   {
      case 0x1234:
         printf("USER BUTTON (ID=%d) was pressed.\n",id);
         break;
      
      default:
         printf("unknown USER BUTTON (ID=%d)!\n",id);
   }
   
   
} /* end of tpUserFunction */


/*************************************************************************
**
** tpParamChangedFunction
**
** Function is called after a change in the SPACE Parameter Dialog
**
**************************************************************************/
void tpParamChangedFunction(void)
{
   /* compare current parameter value with valid value (e.g. < 5) */
   if (lPar(numberParId) >= 5)
   {

      systemPrintf2("*****************************************");
      systemPrintf2("** the selected UNIT is not available! **");
      systemPrintf2("** Parameter is set to first available **");
      systemPrintf2("**              UNIT.                  **");
      systemPrintf2("*****************************************");
      /* set parameter to valid value */
      setLPar(1, numberParId);
      
      /* sends the parameter of test program to SPACE */
      writePar();
   }

}  /* end of tpParamChangedFunction */


/*************************************************************************
**
** tpPreSeqChangedFunction
**
** Function is called before changing a Test Setup or Sequencer in SPACE
** return values: 0 - the Test Setup/ Sequencer will be changed
**    	        !=0 - the Test Setup/ Sequencer will not be changed
**
**************************************************************************/
long tpPreSeqChangedFunction(long setupId, long seqId)
{
   long ret = 0;
   /* return value must be 0 at test program loading,
   otherwise test program can't be loaded ! */
 
   return (ret);
}  /* end of tpPreSeqChangedFunction */


/*************************************************************************
**
** tpPostSeqChangedFunction
**
** Function is called after changing a Test Setup or Sequencer
**
**************************************************************************/
void tpPostSeqChangedFunction()
{
}  /* end of tpPostSeqChangedFunction */


/*************************************************************************
**
** tpTsChangedFunction
**
** Function is called when any test step behaviour (e.g. set inactive or
** active) is changed in the test steps.
**
**************************************************************************/
void tpTsChangedFunction()
{
}  /* end of tpTsChangedFunction */


/*************************************************************************
**                                                                        
** tpPreTestStepFunction   
** (remove comment to activate this function)
**                                                                        
** This function is called before calling a test step when using the      
** Limit Sequencer (Test Setup)                                              
**                                                                        
**************************************************************************/
/*void tpPreTestStepFunction()
{
}  /* end of tpPreTestStepFunction */


/*************************************************************************
**                                                                        
** tpPostTestStepFunction                                                 
** (remove comment to activate this function)
**                                                                        
** This function is called after returning form a test step when using the  
** Limit Sequencer (Test Setup)                                             
**                                                                        
**************************************************************************/
/*void tpPostTestStepFunction()
{
}  /* end of tpPostTestStepFunction */


/*************************************************************************
**                                                                        
** tpPreDoStoreFunction                                                   
** (remove comment to activate this function)
**                                                                        
** This function is called when the function doStore() is executed,       
** before checking the result for PASS or FAIL                            
** Parameters:                                                            
** double *value  - a pointer to the result value                         
** long resIndex  - the index of the result                               
** long site      - the current site number (0..n-1; n site counter)    
**                                                                        
**************************************************************************/
/*void tpPreDoStoreFunction(double *value, long resIndex, long site)
{
} /* tpPreDoStoreFunction */


/*************************************************************************
** 
** tpPostDoStoreFunction
** (remove comment to activate this function)
** 
** This function is called when the function doStore() is executed,
** after checking the result for PASS or FAIL 
** Parameters: 
** double *value    - a pointer to the result value
** long resIndex    - the index of the result 
** bool_t *failFlag - a pointer to fail flag; flag is result of P/F check
** long *failBinIndex- a pointer to the fail bin index
** long site        - the current site number (0..n-1; n site counter)
** 
**************************************************************************/
/*void tpPostDoStoreFunction(double *value, long resIndex, bool_t *failFlag,
                            long *failBinIndex, long site)
 {
    if (tpGetActualTestStep() == tpTsIdxNam("example0"))
    {
       if (resIndex == i1ResId) /* check the first result of first test step
       {
 	 if ((1<<site) & *failFlag) /* if actual site fail *
          {
             *failFlag &= ~(1<<site); /* set failFlag to pass *
           }
        }
    }
 } /* tpPostDoStoreFunction */


/*************************************************************************
** 
** tpAlarmProcessFunction
** (remove comment to activate this function)
**
** This function is called immediately in case of a hardware alarm.
** Parameters: 
** long instrument   - id of the instrument, which reported the alarm
** long siteInfo     - bit mask with a bit set for every test site, which had
**    	               an alarm 
** long alarmMask    - a bit mask of all occurred alarms
**
**************************************************************************/
/*void tpAlarmProcessFunction(long instrument, long siteInfo, long alarmMask)
{
} /* tpAlarmProcessFunction */

/*************************************************************************
** 
** tpAlarmResultFunction
** (remove comment to activate this function)
**
** This function is called inside the doStore function, if there was an 
** hardware alarm since the last doStore or since the beginning of the test 
** step
**
**************************************************************************/
/*void tpAlarmResultFunction()
{
} /* tpAlarmResultFunction */

/*************************************************************************
** 
** tpClearCountersFunction 
**
** This function is called before all counters and statistical values are
** cleared. This is when 
** - loading new TP
** - changing the sequencer
** - End of Lot 
** - activating SPACE button: Clear Counters
** - calling the TP function tpClearCounters()
**
**************************************************************************/
void tpClearCountersFunction()
{
} /* tpClearCountersFunction */
     
 
/*************************************************************************
** 
** tpSoftwareAlarmFunction
**
** This function is called when a software alarm occurs.
** Parameters: 
** long alarmSource  - possible parameter values are:
**                     YIELD_ALARM
**                     CFAIL_ALARM
**                     FAIL_ALARM
**                     RETEST_ALARM
**                     The parameter is representing a bit mask for the
**                     different software alarm which has occured.
** unsigned long siteMask - bit mask of all sites where the alarm(s)occurred.
**
**************************************************************************/
void tpSoftwareAlarmFunction(long alarmSource, unsigned long siteMask)
{
} /* tpSoftwareAlarmFunction */



/**************************************************************************
**
** Main function of the test program
**
***************************************************************************/
int main(int argc, char *argv[])
{

  tpmSetEnv(argc, argv);

  /*
  ** reading sysParFile
  ** replace NULL with your own sysParFile (optional)
  */
  tpmMain(argc,argv,NULL);

} /* main */

/**************************************************************************
**                                                                        
** Private Sequencer Definition Area 
**                                                                        
***************************************************************************/

/*SEQ MySeq1()
{
   long 	actIdx;
   bool_t	doShort;
   long         cnt;

   doShort = bPar(doShortParId);

   actIdx = tpTsIdxNam("example0"); /* get the test step Name  index 
   tpSetActualTestStep(actIdx);     /* set the actual test step index 
   exampleTest0();   	      	    /* execute exampleTest0() 
   					   
   if (tpIsTsFail(actIdx))    	    /* this example: the first result will 
   {  	             	      	    /* never fail due to the 
      return;        	      	    /* tpPostDoStoreFunction() 
   }

   if (doShort)
   {
      actIdx = tpTsIdxNam("example1"); /* get the test step name  index 
      tpSetActualTestStep(actIdx);     /* set the actual test step index 
      exampleTest1(); 
   }	   

   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   ** if "exampleTest1()" DOES NOT fail execute the group of tests
   **	   exampleTest2-5() no matter if they fail
   **~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   

   if(!tpIsTsFail(tpTsIdxNam("example1")))/* if doShort,TS was not executed
   {  	             	      	          /* and so that TS was not fail 
      actIdx = tpTsIdxNam("example2"); /* get the test step name  index 
      tpSetActualTestStep(actIdx);     /* set the actual test step index 
      exampleTest2(); 
   	   
      actIdx = tpTsIdxNam("example3"); /* get the test step name index 
      tpSetActualTestStep(actIdx);     /* set the actual test step index 
      exampleTest3();
       
      actIdx = tpTsIdxNam("example4"); /* get the test step name index 
      tpSetActualTestStep(actIdx);     /* set the actual test step index 
      exampleTest4(); 
	      
      actIdx = tpTsIdxNam("example5"); /* get the test step name index 
      tpSetActualTestStep(actIdx);     /* set the actual test step index 
      cnt = 0;
      do 
      {
         exampleTest5();	  /* execute the exampleTest5() 
         cnt++; 		  /* up to 3 times if the test step FAILS 
      } 
      while ((cnt < 3) && tpIsTsFail(actIdx));
   }

} 
 /* End MySeq1 */
 

/**************************************************************************
**                                                                        
** Definition Area                                                        
**                                                                        
***************************************************************************/

DEFINITION

   /* global parameters are valid in all TEST_SETUPs */
   
   GLOBAL_PARAMETER
      BUTTON_PAR   "TestButton",	0x1234,    &testButtonParId
   GLOBAL_PARAMETER_END
   
/**************************************************************************
** definition of limit sequencers                                         
**************************************************************************/

TEST_SETUP "ADC_TEST" , PBIN(1)

	DOUBLE_PAR "voltage",		  "4.9",   "5.1",   "5.0",  "V",         &voltageParId
	
	DOUBLE_PAR "signal frequency",    "1020.0", "1000.0", "1013.5",  "Hz",   &sigFreqParId
   	DOUBLE_PAR "signal amplitude",    "1.400", "1.500", "1.490"," V",        &sigAmplParId
   	DOUBLE_PAR "signal DC baseline",  "-10.0", "10.0",  "0",    " V",        &sigDcBaseParId
   	DOUBLE_PAR "LPF frequency",       "0.5",   "3000.0","2000", "Hz",        &lpfParId
	DOUBLE_PAR "signal Range",	  "1.200", "1.260", "1.250","V"	 ,       &sigRangeParId
	DOUBLE_PAR "clock frequency",	  "41000","42000","41513","Hz",          &clkFreqParId
	LONG_PAR   "increments",  	   0,       16000,    25,                &incrementParId
	
	
	TEST_STEP "powerStart", powerStart, ID(1),COMMENT("Bring up DPS")
    	  RES "voltage out","4.5", "5.5", "V", &voltageResId, ID(101), FBIN(11)
   	TEST_STEP_END
   
   	TEST_STEP  "triangleStart", triangleStart, ID(2),COMMENT("Activates DSP to output a triangle wave")
     	  RES "signal frequency", "1000", "1020", "Hz", &sigFreqResId,ID(102), FBIN(12)
   	TEST_STEP_END  

	TEST_STEP "powerStop", powerStop, ID(3),COMMENT("Shuts down the DPS")
    	  RES "voltage out","-0.05", "0.2", "V", &voltageResId2, ID(103), FBIN(13)
   	TEST_STEP_END
   
TEST_SETUP_END

/*
  BOOL_PAR     "Do Short", TRUE, &doShortParId,COMMENT("enable Short Tests")
   LONG_PAR     "number", 1, 8, 1, &numberParId
   DOUBLE_PAR   "voltage", "0.0", "8.0", "5.5", " V", &voltageParId

   TEST_STEP    "example0",  exampleTest0,  COMMENT("just an example")
     RES "i1",   "0.80", "1.2",  "mA",   &i1ResId,        ID(101), FBIN(11)
     RES "i2",   "0.55", "1.45", "mA",   &i2ResId,        ID(102), FBIN(12)
     RES "imp.", "1.0",  "10.0", "kOhm", &impedanceResId, ID(300), FBIN(13)
   TEST_STEP_END
   
   TEST_STEP    "example1",  exampleTest1,ID(1),COMMENT("just an example 1 ")
     RES "R4 ",	"0.95", "1.91",  "mA",   &i1_1ResId, 	FBIN(21)
     RES "R5 ",	"0.85", "1.96",  "mA",   &i2_1ResId, 	FBIN(22)
     RES "R6 ",	"0.75", "10.01",  "kOhm", &imp_1ResId,  FBIN(23)
   TEST_STEP_END

   TEST_STEP    "example2",  exampleTest2,ID(2),COMMENT("just an example 2 ")
     RES 	"R7 ",	"0.95",		"1.9",  "mA",  	&i1_2ResId,  FBIN(31)
     RES 	"R8 ",	"0.15",		"1.95", "mA",  	&i2_2ResId,  FBIN(32)
     RES 	"R9 ",	"1.0",		"10.0", "kOhm",	&imp_2ResId, FBIN(33)
   TEST_STEP_END

   TEST_STEP    "example3",  exampleTest3,ID(3),COMMENT("just an example 3 ")
     RES 	"R10",	"0.95",		 "1.9",  "mA", 	&i1_3ResId,  FBIN(41)
   TEST_STEP_END

   TEST_STEP    "example4",  exampleTest4,ID(4),COMMENT("just an example 4 ")
     RES 	"R11",	"0.95",		"1.9",  "mA",  	&i1_4ResId,  FBIN(51)
   TEST_STEP_END

   TEST_STEP    "example5",  exampleTest5,ID(5),COMMENT("just an example 5 ")
     RES 	"R12",	"0.95",		"1.2",  "mA",  	&i1_5ResId,  FBIN(61)
   TEST_STEP_END
*/   



/**************************************************************************
** definition of binning                                                  
***************************************************************************/

/* 
** name of software bins (PBIN, FBIN) can be defined with the SWB_DEF block
** (only one bin number can be assigned to each name) 
*/
/*SWB_DEF
   SWB  "pass"      ,1   
   SWB  "current1"  ,11
   SWB  "current2"  ,12
   SWB  "impedance" ,13
SWB_DEF_END*/

HWB_DEF
   HWB "pass", 1,    1  
   HWB "fail", 2,    11, 12
HWB_DEF_END

/* SEQUENCER "Testlab", &Testlab
   ADD_TS powerStart
   ADD_TS triangleStart
SEQUENCER_END */

/*
SEQUENCER "MyOwnSequencer1" , &MySeq1
   ADD_TS sourcesTest
SEQUENCER_END
*/

DEFINITION_END

/**************************************************************************/
/***************** end of test program ************************************/
/**************************************************************************/
