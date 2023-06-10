/*#include <wx/gdicmn.h>/*wxPoint*/

#include"CdCc_frm.h"
#include"Calc.h"
#include"CalcSet.h"



/*flags of known values*/
static
flag_t
GetKnownFlag(
	const wxArrayString& params
) {
	flag_t flag = (flag_t)NULL;

	if (params[PRM(LINE)] == "Line") flag |= BIT_FLAG(LINE);/*Line/Arc*/
	if (params[PRM(ARC_CW)] == "Arc(CW)") flag |= BIT_FLAG(ARC_CW);/*Line/Arc*/
	if (params[PRM(ARC_CCW)] == "Arc(CCW)") flag |= BIT_FLAG(ARC_CCW);/*Line/Arc*/

	if (params[PRM(X_CRD)] != "?") flag |= BIT_FLAG(X_CRD);/*X:*/
	if (params[PRM(Y_CRD)] != "?") flag |= BIT_FLAG(Y_CRD);/*Y:*/

	if (params[PRM(ANGLE)] != "?") flag |= BIT_FLAG(ANGLE);/*Angle*/

	if (params[PRM(I_CRD)] != "?") flag |= BIT_FLAG(I_CRD);/*I:*/
	if (params[PRM(J_CRD)] != "?")  flag |= BIT_FLAG(J_CRD);/*J:*/

	if (params[PRM(LENGTH)] != "?" && (flag & BIT_FLAG(LINE))) flag |= BIT_FLAG(LENGTH);/*Len/Rad*/
	if (params[PRM(RADIUS)] != "?" && ((flag & BIT_FLAG(ARC_CW)) || (flag & BIT_FLAG(ARC_CCW)))) flag |= BIT_FLAG(RADIUS);/*Len/Rad*/

	return flag;
}

/*#############################################################################*/

#pragma optimize("", off)
bool
FRAME_CLASS_NAME::CalcStrCrd(
	size_t crntIndx
) {
	flag_t result = (flag_t)NULL;
	size_t itmCnt = this->m_p_strCrdSzr->GetItemCount()/*for efficienty*/
		, nxtIndx = (crntIndx + 1) % itmCnt/*this construct is necessary for a dynamic list where the number of elements changes each time*/
		, prvIndx = (crntIndx + (itmCnt - 1)) % itmCnt;

	wxArrayString crntPrms
		, nxtPrms
		, prvPrms;

	flag_t crntKwnFlg = GetKnownFlag(this->GetParamsFromStrCrd(crntIndx, crntPrms))
		, nxtKwnFlg
		, prvKwnFlg;

	if (this->GetParamsFromStrCrd(nxtIndx, nxtPrms).empty()
		|| this->GetParamsFromStrCrd(prvIndx, prvPrms).empty()) {/*if params will be empty, a string coordinate isn't exist*/
#if defined(_DEBUG)
		assert(0), abort();
#else
#endif/*!_DEBUG*/
		return false;
	}

	nxtKwnFlg = GetKnownFlag(nxtPrms);
	prvKwnFlg = GetKnownFlag(prvPrms);

	/*(kwnFlg ^ (BIT_FLAG(0) | BIT_FLAG(1) | BIT_FLAG(2))) == NULL -- set of this bits need only, other bits must be zero*/
	/*(kwnFlg & (BIT_FLAG(0) | BIT_FLAG(1) | BIT_FLAG(2))) != NULL -- one of this bit set, other bits don't matter*/
	/*(kwnFlg & BIT_FLAG(0)) && (kwnFlg & BIT_FLAG(1)) && (kwnFlg & BIT_FLAG(2)) -- set of this bits need only, other bits don't matter*/

	switch (crntKwnFlg)
	{
		/*##############################################################################*/
		/*###################		L		I		N		E	s	####################*/
		/*##############################################################################*/

	case BIT_FLAG(LINE):/*crntStrCrd = CmbBx:Line X:? Y:? Angle:? Len:?*/

		if ((nxtKwnFlg & BIT_FLAG(LINE))
			&& (nxtKwnFlg & BIT_FLAG(X_CRD))
			&& (nxtKwnFlg & BIT_FLAG(Y_CRD))
			&& (nxtKwnFlg & BIT_FLAG(ANGLE))
			&& (nxtKwnFlg & BIT_FLAG(LENGTH))
			) {
			result = CANCAT2(ClcLin, 1)(crntPrms, nxtPrms);
			result = (result) ? (bool)this->SetParamsToStrCrd(crntIndx, crntPrms, result >> X_CRD/*skip line-arc flags*/, *wxGREEN) : false;
			break;
		}

		/*#############################################################################*/

	case BIT_FLAG(LINE) | BIT_FLAG(ANGLE):/*crntStrCrd = CmbBx:Line Angle:! */

		if ((nxtKwnFlg & BIT_FLAG(LINE))
			&& (nxtKwnFlg & BIT_FLAG(X_CRD))
			&& (nxtKwnFlg & BIT_FLAG(Y_CRD))
			&& (nxtKwnFlg & BIT_FLAG(ANGLE))

			&& (prvKwnFlg & BIT_FLAG(LINE))
			&& (prvKwnFlg & BIT_FLAG(X_CRD))
			&& (prvKwnFlg & BIT_FLAG(Y_CRD))
			) {
			result = CANCAT2(ClcLin, 6)(prvPrms, crntPrms, nxtPrms);
			result = (result) ? (bool)this->SetParamsToStrCrd(crntIndx, crntPrms, result >> X_CRD/*skip line-arc flags*/, *wxGREEN) : false;
			break;
		}

		/*#############################################################################*/

	case BIT_FLAG(LINE) | BIT_FLAG(ANGLE) | BIT_FLAG(LENGTH):/*crntStrCrd = CmbBx:Line X:? Y:? Angle:! Len:!*/

		if ((prvKwnFlg & (BIT_FLAG(LINE) | BIT_FLAG(ARC_CW) | BIT_FLAG(ARC_CCW)))
			&& (prvKwnFlg & BIT_FLAG(X_CRD))
			&& (prvKwnFlg & BIT_FLAG(Y_CRD))
			) {
			result = CANCAT2(ClcLin, 5)(prvPrms, crntPrms);
			result = (result) ? (bool)this->SetParamsToStrCrd(crntIndx, crntPrms, result >> X_CRD/*skip line-arc flags*/, *wxGREEN) : false;
			break;
		}

		
		/*#############################################################################*/

	case BIT_FLAG(LINE) | BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD):/*crntStrCrd = CmbBx:Line X:! Y:! Angle:? Len:?*/
	case BIT_FLAG(LINE) | BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD) | BIT_FLAG(ANGLE):/*crntStrCrd = CmbBx:Line X:! Y:! Angle:! Len:?*/
	case BIT_FLAG(LINE) | BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD) | BIT_FLAG(ANGLE) | BIT_FLAG(LENGTH):/*crntStrCrd = CmbBx:Line X:! Y:! Angle:! Len:!*/

		if ((nxtKwnFlg & BIT_FLAG(LINE))
			&& (nxtKwnFlg & BIT_FLAG(X_CRD))
			&& (nxtKwnFlg & BIT_FLAG(Y_CRD))) {/*nxtStrCrd = CmbBx:Line X:! Y:! Angle:!? Len:!?*/
			result = CANCAT2(ClcLin, 2)(crntPrms, nxtPrms);
			result = (result) ? (bool)this->SetParamsToStrCrd(nxtIndx, nxtPrms, result >> X_CRD/*skip line-arc flags*/, *wxGREEN) : false;
			break;
		}

		if ((nxtKwnFlg & BIT_FLAG(LINE))
			&& (nxtKwnFlg & BIT_FLAG(ANGLE))
			&& (nxtKwnFlg & BIT_FLAG(LENGTH))
			) {
			result = CANCAT2(ClcLin, 5)(crntPrms, nxtPrms);
			result = (result) ? (bool)this->SetParamsToStrCrd(nxtIndx, nxtPrms, result >> X_CRD/*skip line-arc flags*/, *wxGREEN) : false;
			break;
		}

		if ((nxtKwnFlg & (BIT_FLAG(ARC_CW) | BIT_FLAG(ARC_CCW)))
			&& (nxtKwnFlg & BIT_FLAG(X_CRD))
			&& (nxtKwnFlg & BIT_FLAG(Y_CRD))
			&& (nxtKwnFlg & BIT_FLAG(RADIUS))
			) {/*prvStrCrd = X:! Y:! other dont matter*/
			result = CANCAT2(ClcArc, 1)(crntPrms, nxtPrms, (nxtKwnFlg & (BIT_FLAG(ARC_CW)) ? flag_set::ARC_CW : flag_set::ARC_CCW));
			result = (result) ? (bool)this->SetParamsToStrCrd(nxtIndx, nxtPrms, result >> X_CRD/*skip line-arc flags*/, *wxGREEN) : false;
			break;
		}
		/**/

		/*#############################################################################*/

	case BIT_FLAG(LINE) | BIT_FLAG(X_CRD) | BIT_FLAG(ANGLE):/*crntStrCrd = CmbBx:Line X:! Y:? Angle:!*/

		if ((prvKwnFlg & BIT_FLAG(X_CRD))
			&& (prvKwnFlg & BIT_FLAG(Y_CRD))) {/*prvStrCrd = X:! Y:! other dont matter*/
			result = CANCAT2(ClcLin, 3)(prvPrms, crntPrms);
			result = (result) ? (bool)this->SetParamsToStrCrd(crntIndx, crntPrms, result >> X_CRD/*skip line-arc flags*/, *wxGREEN) : false;
			break;
		}

		/*#############################################################################*/

	case BIT_FLAG(LINE) | BIT_FLAG(Y_CRD) | BIT_FLAG(ANGLE):/*crntStrCrd = CmbBx:Line X:? Y:! Angle:!*/

		if ((prvKwnFlg & BIT_FLAG(X_CRD))
			&& (prvKwnFlg & BIT_FLAG(Y_CRD))) {/*prvStrCrd = X:! Y:! other dont matter*/
			result = CANCAT2(ClcLin, 4)(prvPrms, crntPrms);
			result = (result) ? (bool)this->SetParamsToStrCrd(crntIndx, crntPrms, result >> X_CRD/*skip line-arc flags*/, *wxGREEN) : false;
			break;
		}

		/*#############################################################################*/

	case BIT_FLAG(LINE) | BIT_FLAG(LENGTH):/*crntStrCrd = CmbBx:Line Length:!*/
		
		if ((nxtKwnFlg & BIT_FLAG(LINE))
			&& (nxtKwnFlg & BIT_FLAG(X_CRD))
			&& (nxtKwnFlg & BIT_FLAG(Y_CRD))
			&& (nxtKwnFlg & BIT_FLAG(LENGTH))

			&& (prvKwnFlg & (BIT_FLAG(LINE) | BIT_FLAG(ARC_CW) | BIT_FLAG(ARC_CCW)))
			&& (prvKwnFlg & BIT_FLAG(X_CRD))
			&& (prvKwnFlg & BIT_FLAG(Y_CRD))
			) {/*prvStrCrd = X:! Y:! other dont matter*/
				result = CANCAT2(ClcLin, 7)(prvPrms, crntPrms, nxtPrms);
			result = (result) ? (bool)this->SetParamsToStrCrd(crntIndx, crntPrms, result >> X_CRD/*skip line-arc flags*/, *wxGREEN) : false;
			break;
		}
		/**/


		/*##############################################################################*/
		/*###################		A		R		K		s	########################*/
		/*##############################################################################*/

	case BIT_FLAG(ARC_CW) | BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD):/*crntStrCrd = CmbBx:Line,ArcCw,ArcCCw X:! Y:!*/
	case BIT_FLAG(ARC_CCW) | BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD):/*crntStrCrd = CmbBx:Line,ArcCw,ArcCCw X:! Y:!*/
	case BIT_FLAG(ARC_CW) | BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD) | BIT_FLAG(ANGLE):/*crntStrCrd = CmbBx:Line X:! Y:! Angle:! Len:?*/
	case BIT_FLAG(ARC_CCW) | BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD) | BIT_FLAG(ANGLE):/*crntStrCrd = CmbBx:Line X:! Y:! Angle:! Len:?*/
	case BIT_FLAG(ARC_CW) | BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD) | BIT_FLAG(ANGLE) | BIT_FLAG(RADIUS):/*crntStrCrd = CmbBx:Line X:! Y:! Angle:! Len:!*/
	case BIT_FLAG(ARC_CCW) | BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD) | BIT_FLAG(ANGLE) | BIT_FLAG(RADIUS):/*crntStrCrd = CmbBx:Line X:! Y:! Angle:! Len:!*/
	case BIT_FLAG(ARC_CW) | BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD)| BIT_FLAG(I_CRD) | BIT_FLAG(J_CRD) | BIT_FLAG(ANGLE) | BIT_FLAG(RADIUS):/*crntStrCrd = CmbBx:Line X:! Y:! Angle:! Len:!*/
	case BIT_FLAG(ARC_CCW) | BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD)| BIT_FLAG(I_CRD) | BIT_FLAG(J_CRD) | BIT_FLAG(ANGLE) | BIT_FLAG(RADIUS):/*crntStrCrd = CmbBx:Line X:! Y:! Angle:! Len:!*/


		if ((nxtKwnFlg & BIT_FLAG(LINE))
			&& (nxtKwnFlg & BIT_FLAG(X_CRD))
			&& (nxtKwnFlg & BIT_FLAG(Y_CRD))
			) {/*nxtStrCrd = CmbBx:Line X:! Y:! Angle:!? Len:!?*/
			result = CANCAT2(ClcLin, 2)(crntPrms, nxtPrms);
			result = (result) ? (bool)this->SetParamsToStrCrd(nxtIndx, nxtPrms, result >> X_CRD/*skip line-arc flags*/, *wxGREEN) : false;
			break;
		}

		if ((nxtKwnFlg & BIT_FLAG(LINE))
			&& (nxtKwnFlg & BIT_FLAG(ANGLE))
			&& (nxtKwnFlg & BIT_FLAG(LENGTH))
			) {
			result = CANCAT2(ClcLin, 5)(crntPrms, nxtPrms);
			result = (result) ? (bool)this->SetParamsToStrCrd(nxtIndx, nxtPrms, result >> X_CRD/*skip line-arc flags*/, *wxGREEN) : false;
			break;
		}


		if ((nxtKwnFlg & (BIT_FLAG(ARC_CW) | BIT_FLAG(ARC_CCW)))
			&& (nxtKwnFlg & BIT_FLAG(X_CRD))
			&& (nxtKwnFlg & BIT_FLAG(Y_CRD))
			&& (nxtKwnFlg & BIT_FLAG(RADIUS))
			) {/*prvStrCrd = X:! Y:! other dont matter*/
			result = CANCAT2(ClcArc, 1)(crntPrms, nxtPrms, (nxtKwnFlg & (BIT_FLAG(ARC_CW)) ? flag_set::ARC_CW : flag_set::ARC_CCW));
			result = (result) ? (bool)this->SetParamsToStrCrd(crntIndx, crntPrms, result >> X_CRD/*skip line-arc flags*/, *wxGREEN) : false;
			break;
		}
		/**/

		/*#############################################################################*/


	case BIT_FLAG(LINE) | BIT_FLAG(ARC_CW) | BIT_FLAG(ARC_CCW) | BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD) | BIT_FLAG(ANGLE) | BIT_FLAG(I_CRD) | BIT_FLAG(J_CRD) | BIT_FLAG(RADIUS):/*everything is known*/
		break;

#if defined(_DEBUG) && 0
	default:
		assert(0), abort();
#else

#endif/*!_DEBUG*/
	}

	return result;
}
#pragma optimize("", on)
