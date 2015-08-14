/* CIN source file */

#include "extcode.h"

/* Typedefs */

typedef struct {
	int32 dimSize;
	float64 element[1];
	} TD0;
typedef TD0 **TD0Hdl;

typedef struct{
	int32 dimSize;
	uInt32 element[1];
	} TD1;
typedef TD1 **TD1Hdl;

typedef struct {
	int32 dimSize[4];
	uInt32 element[1];
	} TD4;
typedef TD4 **TD4Hdl;

typedef struct {
	float64 BinningMicroS;
	float64 OverflowConstant;
	float64 MacrotimeCycle;
	TD4Hdl hTrace;
	TD4Hdl hRetrace;
	int32 currentLine;
	int32 currentPlane;
	} TD3;


typedef struct{
			unsigned nsync		:10; 	// numer of sync period
			unsigned dtime		:15;    // delay from last sync in units of chosen resolution 
			unsigned channel	:6;
			unsigned special	:1;
				} HHraw;
typedef HHraw* LPHHraw;

#define SIZE(h)		((*h)->dimSize)
#define DATA(h)		((*h)->element)
#define TIME(h)		((*h).MacrotimeCycle)

MgErr CINRun(TD1Hdl hRawData, uInt32 *BinningHH, uInt32 *nactual, LVBoolean *pShowRetrace, TD3* pInfo, uInt32 pEndOfLine);

MgErr CINRun(TD1Hdl hRawData, uInt32 *BinningHH, uInt32 *nactual, LVBoolean *pShowRetrace, TD3* pInfo, uInt32 pEndOfLine)
{
	uInt32 idx;
	uInt32 apEndOfLine = 0;
	//float64 pMacrotimeCycle = pInfo->MacrotimeCycle;
	float64 macrotime;
	uInt32 hhchannel, currentBin;
	LPHHraw lpRawdata = (LPHHraw)(DATA(hRawData));
	uInt32* pRetrace = DATA(pInfo->hRetrace);
	uInt32* pTrace = DATA(pInfo->hTrace);
	uInt32 width = SIZE(pInfo->hTrace)[3];
	int32 height = SIZE(pInfo->hTrace)[2];
	int32 planes = SIZE(pInfo->hTrace)[1];
	uInt32 channels = SIZE(pInfo->hTrace)[0];
	int32 planesize = width * height;
	int32 channelsize = planesize * planes;
	int32 offset = pInfo->currentPlane * planesize + pInfo->currentLine * width;
	
	for (idx = 0; idx < *nactual && apEndOfLine == 0; idx++, lpRawdata++) //for each event record
	{
		if (lpRawdata->special == 1) 
		{	
			pInfo->MacrotimeCycle += pInfo->OverflowConstant;
		}
		//else if (special == 1 && channel between 1-15) external marker support could be inserted here
		else //valid photon
		{
			macrotime = pInfo->MacrotimeCycle; // macrotimeCycle has a resolution of 13µs which is sufficient for scanning. If not, add LaserRepetitionTime*nsync
			hhchannel = lpRawdata->channel;
			currentBin = (uInt32)((float64)(macrotime / pInfo->BinningMicroS));
			if (currentBin < width)
				{
					if (hhchannel < channels)
						++pTrace[hhchannel * channelsize + offset + currentBin];
				}
				else if (*pShowRetrace && currentBin < 2 * width)
				{
					if (hhchannel < channels)
						++pRetrace[hhchannel * channelsize + offset + currentBin - width];
				}
			
			if ((!*pShowRetrace && currentBin >= width) || (*pShowRetrace && currentBin >= 2 * width))
			{
				apEndOfLine=1;
				pInfo->MacrotimeCycle = 0;
			}
		}
	}
	*nactual=apEndOfLine;
	return noErr;
	}