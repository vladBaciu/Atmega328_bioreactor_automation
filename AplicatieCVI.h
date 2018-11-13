/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_Pump3                      2       /* control type: LED, callback function: (none) */
#define  PANEL_Pump2                      3       /* control type: LED, callback function: (none) */
#define  PANEL_Pump1                      4       /* control type: LED, callback function: (none) */
#define  PANEL_SavePath                   5       /* control type: string, callback function: (none) */
#define  PANEL_SaveButton                 6       /* control type: command, callback function: BrowseButton */
#define  PANEL_PICTURE                    7       /* control type: picture, callback function: (none) */
#define  PANEL_SPLITTER_2                 8       /* control type: splitter, callback function: (none) */
#define  PANEL_TimeBox                    9       /* control type: textBox, callback function: (none) */
#define  PANEL_Stamp                      10      /* control type: textMsg, callback function: (none) */
#define  PANEL_ComList                    11      /* control type: listBox, callback function: (none) */
#define  PANEL_Cagliari                   12      /* control type: textMsg, callback function: (none) */
#define  PANEL_OpenConnection             13      /* control type: command, callback function: OpenSerialConnection */
#define  PANEL_CloseCommand               14      /* control type: command, callback function: CloseSerialConnection */
#define  PANEL_DECORATION_3               15      /* control type: deco, callback function: (none) */
#define  PANEL_ConnectionStatus           16      /* control type: textBox, callback function: (none) */
#define  PANEL_TEXTMSG_5                  17      /* control type: textMsg, callback function: (none) */
#define  PANEL_DECORATION                 18      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_2               19      /* control type: deco, callback function: (none) */
#define  PANEL_TEXTMSG_4                  20      /* control type: textMsg, callback function: (none) */
#define  PANEL_DECORATION_4               21      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_5               22      /* control type: deco, callback function: (none) */
#define  PANEL_QUITBUTTON                 23      /* control type: command, callback function: QuitCallback */
#define  PANEL_DECORATION_6               24      /* control type: deco, callback function: (none) */
#define  PANEL_TEXTMSG                    25      /* control type: textMsg, callback function: (none) */
#define  PANEL_ConnectedOrNot             26      /* control type: LED, callback function: (none) */
#define  PANEL_FedPmpText_2               27      /* control type: textMsg, callback function: (none) */
#define  PANEL_FedPmpText                 28      /* control type: textMsg, callback function: (none) */
#define  PANEL_STOP_DIS2                  29      /* control type: numeric, callback function: (none) */
#define  PANEL_STOP_DIS1                  30      /* control type: numeric, callback function: (none) */
#define  PANEL_STOP_FED                   31      /* control type: numeric, callback function: (none) */
#define  PANEL_START_DIS2                 32      /* control type: numeric, callback function: (none) */
#define  PANEL_START_DIS1                 33      /* control type: numeric, callback function: (none) */
#define  PANEL_DECORATION_8               34      /* control type: deco, callback function: (none) */
#define  PANEL_START_FED                  35      /* control type: numeric, callback function: (none) */
#define  PANEL_DECORATION_7               36      /* control type: deco, callback function: (none) */
#define  PANEL_CheckAndControl            37      /* control type: timer, callback function: CheckAndControl */
#define  PANEL_TEXTMSG_2                  38      /* control type: textMsg, callback function: (none) */
#define  PANEL_CYCLE_TIME                 39      /* control type: numeric, callback function: (none) */
#define  PANEL_Day                        40      /* control type: LED, callback function: (none) */
#define  PANEL_Night                      41      /* control type: LED, callback function: (none) */
#define  PANEL_TIMER                      42      /* control type: timer, callback function: GetPcTime */
#define  PANEL_INFO_BUTTON                43      /* control type: command, callback function: INFOBTN */
#define  PANEL_DECORATION_9               44      /* control type: deco, callback function: (none) */
#define  PANEL_SERIALCHECK                45      /* control type: string, callback function: (none) */
#define  PANEL_ERROR_SAVEPATH             46      /* control type: LED, callback function: (none) */
#define  PANEL_TEMP_INDICATOR             47      /* control type: string, callback function: (none) */
#define  PANEL_PRESSURE_INDICATOR         48      /* control type: string, callback function: (none) */
#define  PANEL_GASPROD_INDICATOR          49      /* control type: string, callback function: (none) */

#define  PANEL_2                          2
#define  PANEL_2_DISSTOP2                 2       /* control type: numeric, callback function: (none) */
#define  PANEL_2_DISSTART2                3       /* control type: numeric, callback function: (none) */
#define  PANEL_2_DISSTOP1                 4       /* control type: numeric, callback function: (none) */
#define  PANEL_2_DISSTART1                5       /* control type: numeric, callback function: (none) */
#define  PANEL_2_FEDINFO12                6       /* control type: numeric, callback function: (none) */
#define  PANEL_2_FEDINFO1                 7       /* control type: numeric, callback function: (none) */
#define  PANEL_2_DISSTOP2_2               8       /* control type: numeric, callback function: (none) */
#define  PANEL_2_DISSTART2_2              9       /* control type: numeric, callback function: (none) */
#define  PANEL_2_DISSTOP1_2               10      /* control type: numeric, callback function: (none) */
#define  PANEL_2_DISSTART1_2              11      /* control type: numeric, callback function: (none) */
#define  PANEL_2_FEDINFO12_2              12      /* control type: numeric, callback function: (none) */
#define  PANEL_2_FEDINFO1_2               13      /* control type: numeric, callback function: (none) */
#define  PANEL_2_TEXTMSG_2                14      /* control type: textMsg, callback function: (none) */
#define  PANEL_2_TEXTMSG                  15      /* control type: textMsg, callback function: (none) */
#define  PANEL_2_TEXTMSG_3                16      /* control type: textMsg, callback function: (none) */
#define  PANEL_2_EXITBUTTON               17      /* control type: command, callback function: ExitINFO */
#define  PANEL_2_DECORATION               18      /* control type: deco, callback function: (none) */
#define  PANEL_2_DECORATION_2             19      /* control type: deco, callback function: (none) */
#define  PANEL_2_TEXTMSG_4                20      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK BrowseButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CheckAndControl(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CloseSerialConnection(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ExitINFO(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GetPcTime(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK INFOBTN(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OpenSerialConnection(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
