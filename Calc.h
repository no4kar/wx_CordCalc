#ifndef CALC_H
#define CALC_H



/*typedef uint32_t flag_t;/**/
#define BIT_FLAG(x) ((flag_t)1 << (x))
#define PRM(enum_flag_set) ((enum_flag_set >= X_CRD) ? (enum_flag_set - X_CRD + 1) : (0))
#define STR2DBL(str_obj) strtod((str_obj).c_str(), NULL)/**/


enum flag_set{
	LINE
	, ARC_CW
	, ARC_CCW
	/*!!! #define PRM(enum_name)*/
	, X_CRD
	, Y_CRD
	, ANGLE/*to X axis*/
	, I_CRD
	, J_CRD
	, LENGTH 
	, RADIUS = LENGTH
	, OPTION
};



/*
struct Flags{
	unsigned line : 1;
	unsigned arc : 1;
	unsigned x : 1;
	unsigned y : 1;
	unsigned angle : 1;
	unsigned lenght : 1;
	unsigned radius : 1;
	unsigned i : 1;
	unsigned j : 1;
} flag;/**/

#endif/*!CALC_H*/