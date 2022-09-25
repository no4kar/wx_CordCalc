#include"CdCc_frm.h"


#if defined(_DEBUG)
cstr_t
FRAME_CLASS_NAME::Ptr2Str(
	void* ptr
) {
	static char buff[3 +/*for '0x' and '\0'*/
#if defined(WIN32)
		(1 << 5)/*2^5*/
#else
		(1 << 6)/*2^6*/
#endif
	] = { 0 };

	return	((sprintf(buff, "0x%p", ptr)) != 0)
		? (cstr_t)buff
		: (cstr_t)(NULL);
}
#endif



/*Convert precision in double-var from 'source' to 'ans' according 'fmt'. Return 'ans'*/
cstr_t
FRAME_CLASS_NAME::Double2Fmt(
	char* ans
	, cstr_t fmt
	, cstr_t source
)const {

	double temp_flt;

	/*assert(expression) is a macro with double negation"!!" and logic-OR"||". If first expression return TRUE next "||" expressions will not be executed.*/
	if ((sscanf(source, "%lf", &temp_flt) == EOF)
		|| (sprintf(ans, fmt, temp_flt) == EOF)) {
#if defined(_DEBUG)
		abort();/**/
#else
		return strcpy(ans, "?");
#endif/*DEBUG*/
	}
	return ans;
}