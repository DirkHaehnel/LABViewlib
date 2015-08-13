/*
 * CIN source file 
 */

#include "extcode.h"

/*
 * typedefs
 */

typedef struct {
	int32 dimSize;
	uInt32 Numeric[1];
	} TD3;
typedef TD3 **TD3Hdl;

typedef struct {
	TD3Hdl Array;
	} TD2;

typedef struct {
	int32 dimSize;
	TD2 Cluster[1];
	} TD1;
typedef TD1 **TD1Hdl;

CIN MgErr CINRun(TD1Hdl Array, int32 *Index1, int32 *Index2, uInt32 *Element);

CIN MgErr CINRun(TD1Hdl Array, int32 *Index1, int32 *Index2, uInt32 *Element) {

	TD3Hdl temp;
	if ((*Index1>=0)&&(*Index1<(*Array)->dimSize))
	{
		temp=((*Array)->Cluster)[*Index1].Array;
		if ((*Index2>=0)&&(*Index2<(*temp)->dimSize))
			*Element = ((*temp)->Numeric)[*Index2];
		else
			*Element=0;
	}
	else
		*Element=0;
	return noErr;
	}
