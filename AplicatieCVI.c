//==============================================================================
//
// Title:		AplicatieCVI
// Purpose:		A short description of the application.
//
// Created on:	7/30/2018 at 11:04:06 AM by Vlad-Eusebiu Baciu.
// Copyright:	Universitatea Transilvania Brasov. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <formatio.h>
#include <time.h>
#include <rs232.h>
#include <ansi_c.h>
#include <utility.h>
#include <cvirte.h>		
#include <userint.h>
#include <cvitdms.h>  
#include "AplicatieCVI.h"
#include "toolbox.h"
#include "excel_tdms.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle = 0;
static int iOpenComPort;
static int iOpenComPortStatus=0;

time_t timestamp;
static struct tm *localTimeStr;
static char * chTime;
static int hours[25];
static int pumps[25];
static int flags[25];
static int hour=-1;
static int minutes=-1;
static double start_fed,stop_fed,start_dis1,stop_dis1,start_dis2,stop_dis2,cycle_time;
static int fed_pump_state=0;
static int dis_pump_state1=0;
static int dis_pump_state2=0;
static int air_pump_state=0;
static int FSM_states=0;
static int cycles=0;
static int child_panel;
static int bytes_read;
static char read_data[1000],outMessage[1000],*tbox_read_data;
static char *command="";
char noPumpPopUp[255];
void showHours(int x[]);
void showPump(int fed_pump_state,int air_pump_state, int dis_pump_state1, int dis_pump_state2);
void checkPumps(int h,int m,double startF,double stopF,double startD1,double stopD1,double startD2,double stopD2);
void checkFed(int h,int m,double startF,double stopF);
void checkDis1(int h,int m,double startD1,double stopD1);  
void checkDis2(int h,int m,double startD2,double stopD2);  
void Event_Char_Detect_Func(int p,int e,void *a);
void checkCycles(); 
char * app(char* s, char c);


  //////////////////////***********TDMS excel BEGIN *********////////////////////

   static FileInfo		gFileInfo;
   static char 			* FILENAME;
   static const char 	*FILE_DESC				= "Data file for sensor values ";
   static const char    *GROUP_NAME				= "Aquired data";
   static const char    *GROUP_DESC				= "Data aquired from Atmega328";
   static ChannelInfo   chInfo;
   static const char  	*chNamesAndDesc[8]={"Time","PC time","Temperature", "Temperature in Celsius degrees", "Pressure ", "Pressure in Bar","Gas production","Gas production - x" }; 
   TDMSinfo TDMS_FILE;
   static int fileSelected=0;
   
   DataToWrite sensorData;
   
  
   
   
   static int timeBufferIndex=0;
   static int tempBufferIndex=0;
   static int pressureBufferIndex=0;
   static int gasProdIndex=0;
  //////////////////////***********TDMS excel END *********////////////////////  


/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int error = 0;


	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (panelHandle = LoadPanel (0, "AplicatieCVI.uir", PANEL));
	 SetCtrlAttribute(PANEL,PANEL_Pump1,ATTR_ON_COLOR,VAL_GREEN);
	 SetCtrlAttribute(PANEL,PANEL_Pump2,ATTR_ON_COLOR,VAL_GREEN); 
	 SetCtrlAttribute(PANEL,PANEL_Pump3,ATTR_ON_COLOR,VAL_GREEN); 
	 SetCtrlAttribute(PANEL,PANEL_ConnectedOrNot,ATTR_ON_COLOR,VAL_GREEN); 
	 child_panel=  LoadPanel(PANEL,"AplicatieCVI.uir",PANEL_2);
	
	 
	 
	 gFileInfo.numChannels=4;

	 
	 
	 TDMS_FILE.mainChannels=malloc (sizeof(TDMSChannelHandle) * gFileInfo.numChannels);
	 TDMS_FILE.tdmsFileHandle=0;
	/* display the panel and run the user interface */
	errChk (DisplayPanel (panelHandle));
	errChk (RunUserInterface ());

	
	// ISR
	     

Error:
	/* clean up */
	if (panelHandle > 0)
		DiscardPanel (panelHandle);
	return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.

int CVICALLBACK panelCB (int panel, int event, void *callbackData,
						 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:

			break;
	}
	return 0;
}




int CVICALLBACK OpenSerialConnection (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	int iSelectedComPort=0;
	int iComPortOpenError=0;
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(PANEL,PANEL_ComList,&iSelectedComPort);
			iComPortOpenError = OpenComConfig (iSelectedComPort, "", 9600, 0, 8, 2, 512, 512);
			InstallComCallback (iSelectedComPort, LWRS_RXCHAR, 0, 0 , Event_Char_Detect_Func, 0);
			    
			ResetTextBox(PANEL,PANEL_ConnectionStatus,"");
	if(fileSelected==1)		
	{		if(iComPortOpenError >=0)
			{
				   //succes
				   
				   SetCtrlVal(PANEL,PANEL_ConnectionStatus,"COM PORT is Open");
				   iOpenComPortStatus=1;
				   SetCtrlAttribute(PANEL,PANEL_OpenConnection,ATTR_DIMMED,	1);
				   SetCtrlAttribute(PANEL,PANEL_CloseCommand,ATTR_DIMMED,	0);
				   SetCtrlAttribute(PANEL,PANEL_ConnectionStatus,ATTR_TEXT_COLOR,VAL_GREEN);
				   iOpenComPort=iSelectedComPort;
				   SetCtrlVal(PANEL,PANEL_ConnectedOrNot,1);
				   	char msg[30];
					sprintf(msg,"0");
				 	ComWrt(iOpenComPort,msg,10);
			}
			else
			{
				  //error
				     
				     SetCtrlVal(PANEL,PANEL_ConnectionStatus,"Failed to open the port");
					 SetCtrlAttribute(PANEL,PANEL_ConnectionStatus,ATTR_TEXT_COLOR,VAL_RED);    
			}
	}
	else 
	{
		
				 MessagePopup("Warning!","First select a file for data storage!");
		
	}
			break;
	}
	return 0;
}





void CVICALLBACK Event_Char_Detect_Func (int portNo,int eventMask,void *callbackData)

{	
		 
		 Delay(0.5);
		 Fmt (outMessage, ""); 
	
		 int	strLen = GetInQLen (iOpenComPort); 
		 if(strLen!=0)
		 { 		ComRd (iOpenComPort, read_data, strLen);
   		 		Fmt (outMessage, "%s%s",outMessage,read_data);
				Fmt (read_data, "");  
		 }
         SetCtrlVal(PANEL,PANEL_SERIALCHECK,outMessage);
		 
		 char *serialMsg= malloc (sizeof(char) * sizeof(outMessage));
		 serialMsg=outMessage;
		 int len = strlen (serialMsg); 
		
	if(len==18)	
	{	 char *t=malloc (sizeof(char) * 10);
		 char *p=malloc (sizeof(char) * 10);
		 char *l=malloc (sizeof(char) * 10);
		 int j=0;
		 for(int i=0;i<18;i++)
		 {
			 
			 if(j==5) j=0;
			 
			 
			 if(i>=1 && i<6)  
				 { 	
					 	t[j]=serialMsg[i];
			  			j++;
				 }
			 if(i>=7 && i<12)			
			 	{
				 		p[j]=serialMsg[i];
						j++;
				}
			 if(i==17)
				 	    l[0]=serialMsg[i];
				 
			
		 }
	
	
		 sensorData.temperature[tempBufferIndex++]=t;   
		 sensorData.pressure[pressureBufferIndex++]=p;
		 sensorData.gasProd[gasProdIndex++]=l;
	   	 sensorData.time[timeBufferIndex++]=chTime;
		 //split uart message   ex: t25.23p52.22level1
		  
		 
		
	//	if(strchr(serialMsg,'t')) 
	//			 {
	//	 
	//				serialMsg++;
	//				for (int i=0;i<3;i++)
	//				{
	//					char *t=serialMsg[i++];
	//				}
	//				sensorData.temperature[tempBufferIndex++]=serialMsg;
	//				serialMsg--;
	//				
	//				
	//			 }
	//			 else if(strchr(serialMsg,'p'))
	//			{
	//
	//			 	serialMsg++;
	//				sensorData.pressure[pressureBufferIndex++]=serialMsg;
	//				serialMsg--; 
	//			}
	//			else if(strchr(serialMsg,'l'))
	//			{
	//		
	//				serialMsg=serialMsg+5;
	//				sensorData.gasProd[gasProdIndex++]=serialMsg;
	//				sensorData.time[timeBufferIndex++]=chTime;
	//				serialMsg=serialMsg-5;
	//			
	//			}
	//			else 
	//			{
	//					///nothing	
	//			}*/
					if(timeBufferIndex==BUFFER_SIZE && gasProdIndex==BUFFER_SIZE &&  pressureBufferIndex==BUFFER_SIZE &&   tempBufferIndex==BUFFER_SIZE)
		{
			
			AddDataToChannels(&TDMS_FILE,sensorData);
			timeBufferIndex=0;
			gasProdIndex=0;
			pressureBufferIndex=0;
			tempBufferIndex=0;
			
		}
		
	}	
	

	return;
}


//char * app(char* s, char c) {
//    char *copy;
//    int l = strlen(s);
//    copy = malloc(l+1);    
//    copy = s;              
//    copy[l] = c;
//    copy[l+1] = '\0';
//    return copy;
//}


int CVICALLBACK CloseSerialConnection (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	int iCloseComPortError=0;

	switch (event)
	{
		case EVENT_COMMIT:
			ResetTextBox(PANEL,PANEL_ConnectionStatus,""); 
			if(iOpenComPortStatus==1)
			{
				iCloseComPortError=CloseCom(iOpenComPort);
				if(iCloseComPortError>=0)
				{
					  iOpenComPortStatus=0;
					  SetCtrlVal(PANEL,PANEL_ConnectedOrNot,0);
					  SetCtrlVal(PANEL,PANEL_ConnectionStatus,"COM PORT successfully closed");
				      SetCtrlAttribute(PANEL,PANEL_OpenConnection,ATTR_DIMMED,	0);
				      SetCtrlAttribute(PANEL,PANEL_CloseCommand,ATTR_DIMMED,	1);
				      SetCtrlAttribute(PANEL,PANEL_ConnectionStatus,ATTR_TEXT_COLOR,VAL_GREEN);
				}
				else
				{
					  iOpenComPortStatus=1;
					 
					  SetCtrlVal(PANEL,PANEL_ConnectionStatus,"COM PORT not closed");
				      SetCtrlAttribute(PANEL,PANEL_OpenConnection,ATTR_DIMMED,	1);
				      SetCtrlAttribute(PANEL,PANEL_CloseCommand,ATTR_DIMMED,	0);
				      SetCtrlAttribute(PANEL,PANEL_ConnectionStatus,ATTR_TEXT_COLOR,VAL_RED);
				}
			}
		

			break;
		case EVENT_TIMER_TICK:

			break;
	}
	return 0;
}
  

//int CVICALLBACK TableHours (int panel, int control, int event,
//							void *callbackData, int eventData1, int eventData2)
//{
//	int        idx;
//    char    a[256];
//    Point cell;
//	char *content= malloc(sizeof(char) * 60); 
//	char msg[512]; 
//	switch (event)
//	{
//		case EVENT_VAL_CHANGED:
//		 
//		 
//		            cell.y = eventData1;
//		            cell.x = eventData2;
//		            GetTableCellVal (PANEL, PANEL_TABLE, MakePoint (cell.x, cell.y), a);
//		            GetTableCellRingIndexFromValue (panelHandle, PANEL_TABLE, 0, MakePoint (cell.x, cell.y), &idx, a);
//			
//		if(cell.x == 1)		
//			{
//           
//					 if(idx==1)
//					 {
//						hours[cell.y]=1; 
//			  
//					 }
//					 else
//					 {
//						hours[cell.y]=0;   
//				 
//					 
//					 }
//			
//					 showHours(hours);
//		
//			}
//		else
//			{
//				if(idx==1)
//			
//						pumps[cell.y]=1;
//					
//				else if(idx==2)
//				 	
//					    pumps[cell.y]=2;
//					
//				else if(idx==3)
//				 	
//					    pumps[cell.y]=3;
//					
//				else 
//						pumps[cell.y]=0;
//			
//			
//			
//			}
//			break;
//	}
//	return 0;
//}

//void showHours(int x[])
//{
//		ResetTextBox(PANEL,PANEL_Hours,"");
//		char showHours[512]="";
//		
//	   for(int i=1;i<=24;i++)
//	   {
//		  if(x[i]==1)
//		  {
//			 sprintf(showHours,"%d\n",i);
//			 SetCtrlVal(PANEL,PANEL_Hours,showHours);
//			  
//		  }
//	   }
//	
//}
int CVICALLBACK QuitCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			    SetCtrlVal(PANEL,PANEL_Pump1,0);
				SetCtrlVal(PANEL,PANEL_Pump2,0); 
				SetCtrlVal(PANEL,PANEL_Pump3,0);
				char msg[30];
				sprintf(msg,"0");
				 ComWrt(iOpenComPort,msg,10); 
				 CloseCom (iOpenComPort); 
				 DiscardPanel(child_panel);
				 
				 if(TDMS_FILE.tdmsFileHandle!=NULL)
				 				TDMS_CloseFile(TDMS_FILE.tdmsFileHandle); 
				 
				 QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK BrowseButton (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
 ;

       switch (event)
       {
              case EVENT_COMMIT:  
                 	if (FileSelectPopupEx ("", "*.tdms", "*.tdms", "Enter TDM Streaming File", 
						VAL_OK_BUTTON, 0, 0, gFileInfo.filePath) > 0)
						{
							SetCtrlVal (PANEL, PANEL_SavePath, gFileInfo.filePath);
							gFileInfo.numChannels=4;
							chInfo.FILE_NAME="TDM Streaming Log Data";
							chInfo.FILE_DESC="Data file created by CVI";
							chInfo.GROUP_NAME="Sensors values";
							chInfo.GROUP_DESC="Data measured by Atmega328";
							for(int i=0;i<8;i++)
								chInfo.strings[i]=chNamesAndDesc[i];
							
							
							if (FileExists (gFileInfo.filePath, 0) == 1)
							{
								RemoveTDMSFiles (gFileInfo.filePath);
								CreateFile(&TDMS_FILE,gFileInfo,chInfo);     
							}			 
							else
							{
								
								CreateFile(&TDMS_FILE,gFileInfo,chInfo);
							}
							fileSelected=1;
  
						}			
						 
                    break;
       }
	return 0;
}

int CVICALLBACK GetPcTime (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			  			
			break;
		case EVENT_TIMER_TICK:
			   char a[512]="";
			   char *b="";
			 
			   
						ResetTextBox(PANEL,PANEL_TimeBox,""); 
			            time(&timestamp);
						localTimeStr=localtime(&timestamp);
						chTime=asctime(localTimeStr);
					 	SetCtrlVal(PANEL,PANEL_TimeBox,chTime);   //afiseaza timpul
						hour= localTimeStr->tm_hour;
						minutes=localTimeStr->tm_min;
					
					
					
						 
			break;
	}
	return 0;
}

//int CVICALLBACK CheckHourTimer (int panel, int control, int event,
//								void *callbackData, int eventData1, int eventData2)
//{
//	char msj[30];
//	switch (event)
//	{
//		case EVENT_COMMIT:

//			break;
//		case EVENT_TIMER_TICK:
//					if (iOpenComPortStatus)
//					{
//						showPump(hour,pumps);  
//						 if(hours[hour] && !flags[hour] && (pumps[hour]!=0))
//						 {
//							 for(int i=1;i<=24;i++)
//							 {
//								if (i==hour)  flags[hour]=1;
//								else 						   flags[i]=0;	
//							 }
//							sprintf(msj,"%d",pumps[hour]); 
//							ComWrt(iOpenComPort,msj,6);			 //actioneaza pompe     
//							
//						
//							 
//						 }
//					}
//			break;
//	}
//	return 0;
//}
void showPump(int fed_pump_state,int air_pump_state, int dis_pump_state1, int dis_pump_state2)
{	 
	   
		if(fed_pump_state)
		{
			   SetCtrlVal(PANEL,PANEL_Pump1,1);
		}
		else   SetCtrlVal(PANEL,PANEL_Pump1,0);
			    
		
		if(air_pump_state)
		{
			    SetCtrlVal(PANEL,PANEL_Pump3,1);
		}
		else    SetCtrlVal(PANEL,PANEL_Pump3,0);
		
		
		
		if(dis_pump_state1 || dis_pump_state2)
		{
			    SetCtrlVal(PANEL,PANEL_Pump2,1);    
			
		}				  
		else 	SetCtrlVal(PANEL,PANEL_Pump2,0); 			
		
		if(cycles==0) 
		{
				SetCtrlVal(PANEL,PANEL_Day,1); 
				SetCtrlVal(PANEL,PANEL_Night,0);
		}
		else {
				 SetCtrlVal(PANEL,PANEL_Day,0); 
				SetCtrlVal(PANEL,PANEL_Night,1);
		}
		
	  
}

//int CVICALLBACK PumpNotSelectedIn10Sec (int panel, int control, int event,
//										void *callbackData, int eventData1, int eventData2)
//{
//	switch (event)
//	{
//		case EVENT_COMMIT:

//			break;
//		case EVENT_TIMER_TICK:
//		  
//		 int flag=0;	
//		  if(iOpenComPortStatus) 		   
//			for(int i=1;i<=24;i++)
//			{
//				  if(pumps[i]==0 && hours[i]) flag=1;
//				 
//				
//			}
//		    if(flag!=0) 
//				  	{
//					  				
//					   				MessagePopup("Warning", "Check to see if all pumps are selected.");
//					}
//			break;
//	}
//	return 0;
//}

int CVICALLBACK CheckAndControl (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
		case EVENT_TIMER_TICK:
					GetCtrlVal(PANEL,PANEL_START_FED,&start_fed);
					GetCtrlVal(PANEL,PANEL_STOP_FED,&stop_fed);
					GetCtrlVal(PANEL,PANEL_START_DIS1,&start_dis1);
					GetCtrlVal(PANEL,PANEL_START_DIS2,&start_dis2); 
					GetCtrlVal(PANEL,PANEL_STOP_DIS1,&stop_dis1);
					GetCtrlVal(PANEL,PANEL_STOP_DIS2,&stop_dis2);
					
					checkCycles();
					checkPumps(hour,minutes,start_fed,stop_fed,start_dis1,stop_dis1,start_dis2,stop_dis2);
			break;
	}
	return 0;
}


void checkPumps(int h,int m,double startF,double stopF,double startD1,double stopD1,double startD2,double stopD2)  
{
	if( iOpenComPortStatus !=0)
	{
		
		if(FSM_states==0) checkFed(h,m,startF,stopF);
		else if(FSM_states==1) checkDis1(h,m,startD1,stopD1);
		else if(FSM_states==2)  checkDis2(h,m,startD2,stopD2);
		else {}
		  showPump(fed_pump_state,air_pump_state,dis_pump_state1,dis_pump_state2); 
	}
	else 
	{ 
				//nothing 
	}
}
void checkFed(int h,int m,double startF,double stopF)
{
	

	char msj[30];
	FSM_states=0;
	  if(fed_pump_state ==0 )
	  {
		  int startHour= (int) startF;
		  int startMin= (int) (startF*100) % 100;
		  if(startHour==h && startMin==m)
		  	{
			 	   fed_pump_state=1;
				   air_pump_state=1;
				   sprintf(msj,"1");	   // start fed_pomp and air_pomp			  
				   ComWrt(iOpenComPort,msj,10);
				
		 	 }
		  else
		  	{ 
			  		fed_pump_state=0;
				
			}
		  		
		 
	  }
	  else 
	  {
		 int stopHour = (int) stopF;
		 int stopMin =  (int) (stopF*100) % 100; 
		 if(stopHour == h && stopMin == m)
		 {
				  fed_pump_state=0;
				  air_pump_state=1;
				  sprintf(msj,"2");		 // stop fed_pomp 		  
				  ComWrt(iOpenComPort,msj,10);
			 	  FSM_states=1;
		 }
		 else  
		 { 
			 fed_pump_state=1; 
			 air_pump_state=1; 
		 }
		  
	  }

}
void checkDis1(int h,int m,double startD1,double stopD1)
{
	   	char msj[30];
	   FSM_states=1;
	  if(dis_pump_state1 ==0 )
	  {
		  int startHour= (int) startD1;
		  int startMin= (int) (startD1*100) % 100;
		  if(startHour==h && startMin==m)
		  	{
			 	   dis_pump_state1=1;
				   air_pump_state=1;
				   sprintf(msj,"3");    //start dis_pump			  
				   ComWrt(iOpenComPort,msj,10);
				
		 	 }
		  else
		  	{ 
			  		dis_pump_state1=0;
				
			}
		  		
		 
	  }
	  else 
	  {
		 int stopHour = (int) stopD1;
		 int stopMin =  (int) (stopD1*100) % 100; 
		 if(stopHour == h && stopMin==m )  
		 {
				  dis_pump_state1=0;
				  air_pump_state=0;
				  sprintf(msj,"4");		//stop  dis pomp	and air pomp				  
				  ComWrt(iOpenComPort,msj,10);
			 	  FSM_states=2;
		 }
		 else  
		 { 
			 dis_pump_state1=1; 
			 air_pump_state=1; 
		 }
		  
	  }
	
}
void checkDis2(int h,int m,double startD2,double stopD2)
{
	   	char msj[30];
	   FSM_states=2;
	  if(dis_pump_state2 ==0 )
	  {
		  int startHour= (int) startD2;
		  int startMin= (int) (startD2*100) % 100;
		  if(startHour==h && startMin==m)
		  	{
			 	   dis_pump_state2=1;
				   air_pump_state=0;
				   sprintf(msj,"5");    //start dis_pump			  
				   ComWrt(iOpenComPort,msj,10);
				
		 	 }
		  else
		  	{ 
			  		dis_pump_state2=0;
				
			}
		  		
		 
	  }
	  else 
	  {
		 int stopHour = (int) stopD2;
		 int stopMin =  (int) (stopD2*100) % 100; 
		 if(stopHour == h && stopMin == m)
		 {
				  dis_pump_state2=0;
				  air_pump_state=0;
				  sprintf(msj,"6");		//stop  dis pomp				  
				  ComWrt(iOpenComPort,msj,10);
			 	  FSM_states=0;
				  if(cycles==1) { cycles=0;  }
				  else { cycles=1; }
		 }
		 else  
		 { 
			 dis_pump_state2=1; 
			 air_pump_state=0; 
		 }
		  
	  }
	
}

void checkCycles()
{
		if (cycles==1) 
		{
			
			 
			  	GetCtrlVal(PANEL,PANEL_CYCLE_TIME,&cycle_time); 
				start_fed+=cycle_time;
				stop_fed+=cycle_time;
				start_dis1+=cycle_time;
				stop_dis1+=cycle_time;
				start_dis2+=cycle_time;
				stop_dis2+=cycle_time;
				
				
				if(start_fed >=24) start_fed-=24;
				if(stop_fed >=24) stop_fed-=24;     
				if(start_dis1 >=24) start_dis1-=24;     
				if(start_dis2 >=24) start_dis2-=24;     
				if(stop_dis2 >=24) stop_dis2-=24;   
				if(stop_dis1 >=24) stop_dis1-=24;   
			
		}
		else {
		
				
				 GetCtrlVal(PANEL,PANEL_START_FED,&start_fed);
				 GetCtrlVal(PANEL,PANEL_STOP_FED,&stop_fed);
				 GetCtrlVal(PANEL,PANEL_START_DIS1,&start_dis1);
				 GetCtrlVal(PANEL,PANEL_START_DIS2,&start_dis2); 
				 GetCtrlVal(PANEL,PANEL_STOP_DIS1,&stop_dis1);
				 GetCtrlVal(PANEL,PANEL_STOP_DIS2,&stop_dis2);
				 
		
		}
}

int CVICALLBACK INFOBTN (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			
		
				DisplayPanel(child_panel);
				SetCtrlVal(child_panel,PANEL_2_FEDINFO1,start_fed);
				SetCtrlVal(child_panel,PANEL_2_FEDINFO12,stop_fed);  
				SetCtrlVal(child_panel,PANEL_2_DISSTART1,start_dis1);  
				SetCtrlVal(child_panel,PANEL_2_DISSTOP1,stop_dis1);  
				SetCtrlVal(child_panel,PANEL_2_DISSTART2,start_dis2);   	
				SetCtrlVal(child_panel,PANEL_2_DISSTOP2,stop_dis2);   
				GetCtrlVal(PANEL,PANEL_CYCLE_TIME,&cycle_time); 
				double sf =0;
				double sf1=0;
				double sd1=0;
				double sd2=0;
				double sd3=0;
				double sd4=0;
				
				sf=cycle_time+start_fed;
				sf1=cycle_time+stop_fed;
				sd1=cycle_time+start_dis1;
				sd2=cycle_time+stop_dis1;
				sd3=cycle_time+start_dis2;
				sd4=cycle_time+stop_dis2;
				
				
				if(sf >=24) sf-=24;
				if(sf1 >=24) 	sf1-=24;     
				if(sd1 >=24) sd1-=24;     
				if(sd2 >=24) sd2-=24;     
				if(sd3 >=24)  sd3-=24;   
				if(sd4 >=24)  sd4-=24;   
				
				
				SetCtrlVal(child_panel,PANEL_2_FEDINFO1_2, sf);
				SetCtrlVal(child_panel,PANEL_2_FEDINFO12_2,sf1);  
				SetCtrlVal(child_panel,PANEL_2_DISSTART1_2,sd1);  
				SetCtrlVal(child_panel,PANEL_2_DISSTOP1_2, sd2);  
				SetCtrlVal(child_panel,PANEL_2_DISSTART2_2,sd3);   	
				SetCtrlVal(child_panel,PANEL_2_DISSTOP2_2, sd4);

			break;
		case EVENT_TIMER_TICK:

			break;
	}
	return 0;
}

int CVICALLBACK ExitINFO (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				  HidePanel(child_panel);
				  
			break;
	}
	return 0;
}

