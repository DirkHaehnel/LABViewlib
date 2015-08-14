/*
 * CIN source file 
 */

#include "extcode.h"

/*
 * typedefs
 */

typedef struct {
	int32 dimSizes;
	uInt32 element[1];
	} TD1;
typedef TD1 **TD1Hdl;

typedef struct {
	float64 BinningMicroS;			// constant: the bin width in micro seconds
	float64 OverflowConstant;		// constant: the factor determining the macro time overflow
	float64 MacrotimeCycle;			// local: the current macro time cycle, i.e. macrotime in µs
	float64 BinningCycle;			// local: the current binning cycle
	uInt32 CurrentDecay;			// local: the decay currently used for counting
	uInt32 BinsUsed;				// output: the numbers of bins finished within this call
	int32 DecayUsed;				// output: the decay finished within this call (-1 == None)
	uInt32 TotalCount;				// the total number of photons processed
	uInt32 GapCount;				// the number of gaps that occured
	uInt32 InvalidCount;			// the number of photons with invalid routing information
	} PARAMETERS;

typedef struct {
	int32 dimSizes[2];
	uInt32 element[1];
	} MATRIX;
typedef MATRIX **HMATRIX;

typedef struct {
	int32 dimSizes[3];
	uInt32 element[1];
	} CUBE;
typedef CUBE **HCUBE;

//#pragma pack(1) 

typedef struct{
			unsigned nsync		:10; 	// number of sync period
			unsigned dtime		:15;    // delay from last sync in units of chosen resolution 
			unsigned channel	:6;
			unsigned special	:1;
				} HHraw;
typedef HHraw *LPHHraw;


//#pragma pack(4)

#define SIZE(h)		((*h)->dimSizes)
#define DATA(h)		((*h)->element)

//#ifdef _DEBUG

//CIN MgErr CINLoad(RsrcFile reserved);

//CIN MgErr CINLoad(RsrcFile reserved)
//{
//	_asm int 3;
//	return noErr;
//}
//#endif

CIN MgErr CINRun(TD1Hdl hRawdata, uInt32* pOffset, uInt32 *pCountRead, PARAMETERS *pParameters, uInt16 *pModule, HMATRIX hMCSTraces, HCUBE hDecays);

CIN MgErr CINRun(TD1Hdl hRawdata, uInt32* pOffset, uInt32 *pCountRead, PARAMETERS *pParameters, uInt16 *pModule, HMATRIX hMCSTraces, HCUBE hDecays)
{
	LPHHraw lpRawdata = (LPHHraw)(DATA(hRawdata));
	uInt32 mcsLength = SIZE(hMCSTraces)[1]; //number of bins in MCS array (=1000 for integration time >=1ms
	uInt32 decayLength = SIZE(hDecays)[2]; //number of bins
	uInt32 channelCount = SIZE(hMCSTraces)[0]; //number of channels
	uInt32 blockLength = decayLength * channelCount; //number of (decay)bins on all channels = full length of array
	uInt32 nRawCount = *pCountRead;
	uInt32 nRouterIdx;
	uInt32* pMCSTrace = DATA(hMCSTraces);
	uInt32* pDecay = DATA(hDecays);
	float64 macrotime;
	uInt32 microtime;
	uInt32 hhchannel;
	float64 nextBin = pParameters->BinningCycle + pParameters->BinningMicroS; //e.g. 1ms intTime: "BinningCycle" + 1000 
	float64 maxBin = pParameters->BinningCycle + mcsLength * pParameters->BinningMicroS;
	uInt32 newBins, diff, offset, size, sizeDecay = sizeof(uInt32) * blockLength; //size of input array in bytes
	//*pOffset /= (*pModule == 0 ? (decayLength == 4096 ? 3 : 2) : 2);

	// move the current bin to the first position:
	size = sizeof(uInt32) * (mcsLength - 1); //size of input array in bytes
	for (nRouterIdx = 0; pParameters->BinsUsed > 0 && nRouterIdx < channelCount; nRouterIdx++) //for every channel
	{
		pMCSTrace[nRouterIdx * mcsLength] = pMCSTrace[nRouterIdx * mcsLength + pParameters->BinsUsed];
		ClearMem(pMCSTrace + nRouterIdx * mcsLength + 1, size);
	}
	//re-initialize
	pParameters->BinsUsed = 0;
	pParameters->DecayUsed = -1;
	
		for (; *pOffset < nRawCount; (*pOffset)++, lpRawdata++)
		{
			if ((lpRawdata->channel & 0x3F) == 0x3F) //condition for macro time (nsync) overflow
			pParameters->MacrotimeCycle += pParameters->OverflowConstant;
		//else if (special == 1 && channel between 1-15) external marker support could be inserted here

			else // valid photon
			{
				macrotime = pParameters->MacrotimeCycle + (pParameters->OverflowConstant / 1024) * lpRawdata->nsync;
				microtime = decayLength - (lpRawdata->dtime) - 1;
				hhchannel = lpRawdata->channel; // no trigger information
				if (macrotime >= maxBin)
				{
					diff = mcsLength - pParameters->BinsUsed;
					if (diff > 0)
					{
						// clear all bins that are empty
						offset = pParameters->BinsUsed;
						size = sizeof(uInt32) * diff;
						for (nRouterIdx = 0; nRouterIdx < channelCount; nRouterIdx++)
							ClearMem(pMCSTrace + nRouterIdx * mcsLength + offset, size);
						pParameters->BinsUsed = mcsLength;
						pParameters->BinningCycle = maxBin;
						// clear the decays
						if (diff != 1)
						{
							if (diff % 2) // due to an odd number of bins the current decay is switched
							{
								pParameters->DecayUsed = pParameters->CurrentDecay;
								pParameters->CurrentDecay = (pParameters->CurrentDecay ? 0 : 1);
							}
							// the used decay is empty anyway
							ClearMem(pDecay + pParameters->DecayUsed * blockLength, sizeDecay);
						}
						else
						{
							pParameters->DecayUsed = pParameters->CurrentDecay;
							pParameters->CurrentDecay = (pParameters->CurrentDecay ? 0 : 1);
						}
					}
					// the current photon will be the first in the next cycle
					// therefor we have to undo eventual MTOV cycles
					if ((lpRawdata->channel & 0x3F) == 0x3F) // MTOV : macro timer overflow
						pParameters->MacrotimeCycle -= pParameters->OverflowConstant;
					break; // break the main loop -> terminate the routine
				}
				if (hhchannel < channelCount)
				{
					if (macrotime >= nextBin)
					{
						newBins = (macrotime - nextBin) / pParameters->BinningMicroS + 1;
						pParameters->BinsUsed += newBins;
						pParameters->BinningCycle += newBins * pParameters->BinningMicroS;
						nextBin = pParameters->BinningCycle + pParameters->BinningMicroS;
						if (newBins % 2)
						{
							pParameters->DecayUsed = pParameters->CurrentDecay;
							pParameters->CurrentDecay = (pParameters->CurrentDecay ? 0 : 1);
						}
						ClearMem(pDecay + pParameters->CurrentDecay * blockLength, sizeDecay);
					}
					++pMCSTrace[hhchannel * mcsLength + pParameters->BinsUsed];
					++pDecay[pParameters->CurrentDecay * blockLength + hhchannel * decayLength + microtime];
				}
				++pParameters->TotalCount;
			}
			//if (lpRawdata->bitFlags & 0x2) // GAP : a gap occurred prior to this photon
			//	++pParameters->GapCount;
		}
	
	
	//*pOffset *= (*pModule == 0 ? (decayLength == 4096 ? 3 : 2) : 2);
	return noErr;
}
