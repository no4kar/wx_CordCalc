#ifndef CALCSET_H_
#define CALCSET_H_

#define _USE_MATH_DEFINES/*provides many mathematical constants, like M_PI*/
#include<cmath>

/*=========================================================
------------------intermediate functions set----------------
=========================================================*/


/* hypotenuse * cos(angle) */
inline static double DeltaX1(const double hypotenuse, const double angleRad) { return double(hypotenuse * std::cos(angleRad)); }
/* x1 - x0 */
inline static double DeltaX2(const double x1, const double x0) { return double(x1 - x0); }
/* hypotenuse * sin(angle) */
inline static double DeltaY1(const double hypotenuse, const double angleRad) { return double(hypotenuse * std::sin(angleRad)); }
/* y1 - y0 */
inline static double DeltaY2(const double y1, const double y0) { return double(y1 - y0); }
/*hypotenuse = ((y1-y0)^2 + (x1-x0)^2)^0,5*/
inline static double Hypotenuse1(const double y1, const double y0, const double x1, const double x0) { return double(std::sqrt(std::pow(DeltaY2(y1, y0), 2) + std::pow(DeltaX2(x1, x0), 2))); }
/*hypotenuse = (deltaY^2 + deltaX^2)^0,5*/
inline static double Hypotenuse2(const double deltaY, const double deltaX) { return double(std::sqrt(std::pow(deltaY, 2) + std::pow(deltaX, 2))); }
/*return degrees per radian. "std::trigonometric_functions" RETURN value as radian*/
inline static double Rad2Degr(const double radian) { return double(radian * (180 / M_PI)); }
/*return radian per degrees. "std::trigonometric_functions" GET value as radian*/
inline static double Degr2Rad(const double degrees) { return double(degrees * (M_PI / 180)); }
/*angle to the X-axis in radians*/
inline static double AngleRad2Xaxis(const double y1, const double y0, const double x1, const double x0) { return std::atan2(DeltaY2(y1, y0), DeltaX2(x1, x0)); }
/*angle to the X-axis in degrees*/
inline static double AngleDegr2Xaxis(const double y1, const double y0, const double x1, const double x0) { return Rad2Degr(AngleRad2Xaxis(y1,y0,x1,x0)); }
/*rotate a 'point' around 'cc' by an increased angle('incrAngleDegr')*/
static wxRealPoint RotPointByIncrAngle(const wxRealPoint& cc, const wxRealPoint& point, const double incrAngleDegr) {
	double d = Hypotenuse1(point.y, cc.y, point.x, cc.x)
		, angle = incrAngleDegr + Rad2Degr(std::atan2(DeltaY2(point.y, cc.y), DeltaX2(point.x, cc.x)));
	return wxRealPoint(cc.x + DeltaX1(d, Degr2Rad(angle)), cc.y + DeltaY1(d, Degr2Rad(angle))); }



/*=========================================================
------------------Line-Line calculation set----------------
=========================================================*/


/*!: line0, line1, x1, y1, len1, angle1;	?: x0, y0*/
static
flag_t
ClcLinLin1(
	wxArrayString& crntPrms
	, wxArrayString& nxtPrms
) {
	double x1, y1, len1, angle1;

	if (!nxtPrms[PRM(X_CRD)].ToDouble(&x1)
		|| !nxtPrms[PRM(Y_CRD)].ToDouble(&y1)
		|| !nxtPrms[PRM(LENGTH)].ToDouble(&len1)
		|| !nxtPrms[PRM(ANGLE)].ToDouble(&angle1)) {
#if defined(_DEBUG)
		assert(0), abort();
#else
#endif/*!_DEBUG*/
		return (flag_t)0;
	}

	crntPrms[PRM(X_CRD)] = wxString::FromDouble((x1 - DeltaX1(len1, Degr2Rad(angle1))), PRECISION);
	crntPrms[PRM(Y_CRD)] = wxString::FromDouble((y1 - DeltaY1(len1, Degr2Rad(angle1))), PRECISION);
	return (BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD));
}

/*!: line0, x0, y0, line1, x1, y1;	?: len1, angle1*/
static
flag_t
ClcLinLin2(
	wxArrayString& crntPrms
	, wxArrayString& nxtPrms
) {
	double x0, y0, x1, y1;

	if (!crntPrms[PRM(X_CRD)].ToDouble(&x0)
		|| !crntPrms[PRM(Y_CRD)].ToDouble(&y0)
		|| !nxtPrms[PRM(X_CRD)].ToDouble(&x1)
		|| !nxtPrms[PRM(Y_CRD)].ToDouble(&y1)
		) {
#if defined(_DEBUG)
		assert(0), abort();
#else
#endif/*!_DEBUG*/
		return (flag_t)0;
	}

	nxtPrms[PRM(LENGTH)] = wxString::FromDouble(Hypotenuse1(y1, y0, x1, x0), PRECISION);
	nxtPrms[PRM(ANGLE)] = wxString::FromDouble(AngleDegr2Xaxis(y1, y0, x1, x0), PRECISION);
	return (BIT_FLAG(LENGTH) | BIT_FLAG(ANGLE));
}

static
flag_t
ClcLinLin3(
	wxArrayString& prvPrms
	, wxArrayString& crntPrms
) {
	double x0, y0, x1, y1, angle1;

	if (!prvPrms[PRM(X_CRD)].ToDouble(&x0)
		|| !prvPrms[PRM(Y_CRD)].ToDouble(&y0)
		|| !crntPrms[PRM(X_CRD)].ToDouble(&x1)
		|| !crntPrms[PRM(ANGLE)].ToDouble(&angle1)
		) {
#if defined(_DEBUG)
		assert(0), abort();
#else
#endif/*!_DEBUG*/
		return (flag_t)0;
	}

	y1 = y0 + (DeltaX2(x1, x0) * std::tan(Degr2Rad(angle1)));

	crntPrms[PRM(Y_CRD)] = wxString::FromDouble(y1, PRECISION);
	crntPrms[PRM(LENGTH)] = wxString::FromDouble(Hypotenuse1(y1, y0, x1, x0), PRECISION);
	return (BIT_FLAG(Y_CRD) | BIT_FLAG(LENGTH));
}

static
flag_t
ClcLinLin4(
	wxArrayString& prvPrms
	, wxArrayString& crntPrms
) {
	double x0, y0, x1, y1, angle1;

	if (!prvPrms[PRM(X_CRD)].ToDouble(&x0)
		|| !prvPrms[PRM(Y_CRD)].ToDouble(&y0)
		|| !crntPrms[PRM(Y_CRD)].ToDouble(&y1)
		|| !crntPrms[PRM(ANGLE)].ToDouble(&angle1)
		) {
#if defined(_DEBUG)
		assert(0), abort();
#else
#endif/*!_DEBUG*/
		return (flag_t)0;
	}

	x1 = x0 + (DeltaY2(y1, y0) / std::tan(Degr2Rad(angle1)));

	crntPrms[PRM(X_CRD)] = wxString::FromDouble(x1, PRECISION);
	crntPrms[PRM(LENGTH)] = wxString::FromDouble(Hypotenuse1(y1, y0, x1, x0), PRECISION);
	return (BIT_FLAG(X_CRD) | BIT_FLAG(LENGTH));
}

static
flag_t
ClcLinLin5(
	wxArrayString& prvPrms
	, wxArrayString& crntPrms
) {
	double x0, y0, len1, angle1;

	if (!prvPrms[PRM(X_CRD)].ToDouble(&x0)
		|| !prvPrms[PRM(Y_CRD)].ToDouble(&y0)
		|| !crntPrms[PRM(LENGTH)].ToDouble(&len1)
		|| !crntPrms[PRM(ANGLE)].ToDouble(&angle1)) {
#if defined(_DEBUG)
		assert(0), abort();
#else
#endif/*!_DEBUG*/
		return (flag_t)0;
	}

	crntPrms[PRM(X_CRD)] = wxString::FromDouble((x0 + DeltaX1(len1, Degr2Rad(angle1))), PRECISION);
	crntPrms[PRM(Y_CRD)] = wxString::FromDouble((y0 + DeltaY1(len1, Degr2Rad(angle1))), PRECISION);
	return (BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD));
}

static
flag_t
ClcLinLin6(
	wxArrayString& prvPrms
	, wxArrayString& crntPrms
	, wxArrayString& nxtPrms
) {
	double x0, y0
		, x1, y1, angle1, b1
		, x2, y2, angle2, b2;

	if (!prvPrms[PRM(X_CRD)].ToDouble(&x0)
		|| !prvPrms[PRM(Y_CRD)].ToDouble(&y0)
		|| !crntPrms[PRM(ANGLE)].ToDouble(&angle1)
		|| !nxtPrms[PRM(X_CRD)].ToDouble(&x2)
		|| !nxtPrms[PRM(Y_CRD)].ToDouble(&y2)
		|| !nxtPrms[PRM(ANGLE)].ToDouble(&angle2)
		) {
#if defined(_DEBUG)
		assert(0), abort();
#else
#endif/*!_DEBUG*/
		return (flag_t)0;
	}
	/*linear function is y(x) = tan(angle)*x + b => b = y(x) - (tan(angle)*x) */
	b1 = y0 - (std::tan(Degr2Rad(angle1)) * x0);
	b2 = y2 - (std::tan(Degr2Rad(angle2)) * x2);

	x1 = (b2 - b1) / (std::tan(Degr2Rad(angle1)) - std::tan(Degr2Rad(angle2)));
	y1 = (std::tan(Degr2Rad(angle1)) * x1) + b1;

	crntPrms[PRM(X_CRD)] = wxString::FromDouble(x1, PRECISION);
	crntPrms[PRM(Y_CRD)] = wxString::FromDouble(y1, PRECISION);
	crntPrms[PRM(LENGTH)] = wxString::FromDouble(Hypotenuse1(y1, y0, x1, x0), PRECISION);
	return (BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD) | BIT_FLAG(LENGTH));
}

/*!: line0, x0, y0, line1, len1, line2, x2, y2, len2;	?: x1, y1*/
static
flag_t
ClcLinLin7(
	wxArrayString& prvPrms
	, wxArrayString& crntPrms
	, wxArrayString& nxtPrms
) {
	/*known variables*/
	double x0, y0, len1, x2, y2, len2;
	/*intermediate variables*/
	double a, h, /*distance between circles centers(cc)*/d, /*angle to the X-axis*/angle;
	char optn;

#if 0
	wxString str = crntPrms[PRM(OPTION)];
	optn = *(str.c_str());
#else
	optn = *(crntPrms[PRM(OPTION)].c_str());
#endif /**/

	wxRealPoint cc, point;

	if (!prvPrms[PRM(X_CRD)].ToDouble(&x0)
		|| !prvPrms[PRM(Y_CRD)].ToDouble(&y0)
		|| !crntPrms[PRM(LENGTH)].ToDouble(&len1)
		|| !nxtPrms[PRM(X_CRD)].ToDouble(&x2)
		|| !nxtPrms[PRM(Y_CRD)].ToDouble(&y2)
		|| !nxtPrms[PRM(LENGTH)].ToDouble(&len2)
		) {
#if defined(_DEBUG)
		assert(0), abort();
#else
#endif/*!_DEBUG*/
		return (flag_t)0;
	}

	
	/*		 (x1,y1)
			   /|\
			  / | \
	 len1 == r1 |  r2 == len2
			/   h   \
	(x0,y0)/-a--|--b-\(x2,y2)

	(1)r1^2 = h^2 + a^2; (2)r2^2 = h^2 + b^2. We can substructe (2)-equesion from (1)-equesion.
	(1)-(2) => r1^2 - r2^2 = a^2 - b^2; |d = a + b => b = d - a|;
	r1^2 - r2^2 = a^2 - (d - a)^2;
	r1^2 - r2^2 = a^2 - d^2 + 2da - a^2;
	a = (r1^2 - r2^2 + d^2 ) / (2d);
	a = ((r1 - r2)*(r1 + r2) + d*d) / (2*d)
	(1) => h = (r1^2 - a^2)^0.5
	*/

	d = Hypotenuse1(y2, y0, x2, x0);
	a = ((len1 - len2) * (len1 + len2) + (d * d)) / (2 * d);
	h = std::sqrt((len1 - a) * (len1 + a));
	angle = AngleDegr2Xaxis(y2, y0, x2, x0);

	switch (optn)
	{
	default:
		crntPrms[PRM(OPTION)] = "1";
	case'1':
		point = wxRealPoint(x0 + a, y0 + h);
		break;
	
	case'2':
		point = wxRealPoint(x0 + a, y0 - h);
		break;
	}
	cc = wxRealPoint(x0, y0);

	point = RotPointByIncrAngle(cc, point, angle);

	crntPrms[PRM(X_CRD)] = wxString::FromDouble(point.x, PRECISION);
	crntPrms[PRM(Y_CRD)] = wxString::FromDouble(point.y, PRECISION);/**/
	return (BIT_FLAG(X_CRD) | BIT_FLAG(Y_CRD) | BIT_FLAG(OPTION));
}


/*=======================================================
------------------Arc-Arc calculation set----------------
=======================================================*/

static
flag_t
ClcArcArc1(
	wxArrayString& prvPrms
	, wxArrayString& crntPrms
) {
	double x0, y0, len1, angle1;

	if (!prvPrms[PRM(X_CRD)].ToDouble(&x0)
		|| !prvPrms[PRM(Y_CRD)].ToDouble(&y0)
		|| !crntPrms[PRM(LENGTH)].ToDouble(&len1)
		|| !crntPrms[PRM(ANGLE)].ToDouble(&angle1)) {
#if defined(_DEBUG)
		assert(0), abort();
#else
#endif/*!_DEBUG*/
		return (flag_t)0;
	}
}

#endif // !CALCSET_H_