#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows it is in resources and even
// though we could still include the XPM here it would be unused)
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "../sample.xpm"
#endif

/*====================STD library====================*/
/*#include <memory>/*shar_ptr*/
/*#include <exception>/**/
/*====================wxWidget library====================*/
/*#include <wx/grid.h>/**/
/*#include <wx/valnum.h>/*wxFloatingPointValidator,*/
#define INCLUDE_WXSQL 1
#if INCLUDE_WXSQL
#include "wx/wxsqlite3.h"
#endif


#define FRAME_CLASS_NAME CordCalcFrame
#define APP_CLASS_NAME CordCalcApp

	/*DEBUG MENEGMENT*/
#define BOXSIZER_1 0
#define BOXSIZER_2 1
#define ADD_DFLT_LN 1
#define	GET_VAL_FROM_LINE 0
#define PRECISION 3
#define CANCAT2(t1, t2) t1##t2
#define CANCAT3(t1, t2, t3) CANCAT2(t1, t2)##t3
#define MY_DEBUG 1
#define ASK_ACT(ask,act) if(##ask){##act##;}
#define m(x) m_##x      /*member*/
#define m_p(x) m(p_##x)     /*member pointer*/
#define CLASS_CONSTRUCT FRAME_CLASS_NAME
#define CLASS_DESTRUCT virtual ~CLASS_CONSTRUCT

#if MY_DEBUG && _DEBUG
#define CRTE(x) new x
#define DSTR(x) delete x
#define MY_TRY(x) try{
#define MY_CATCH(x) } catch (const std::exception& ex) { wxMessageBox(ex.what()); }
#else
#define CRTE(x)
#define DSTR(x)
#define MY_TRY(x)
#define MY_CATCH(x)
#endif/*TO_HISTORY*/


typedef uint32_t flag_t;
typedef const char* cstr_t;

/* IDs for the controls and the menu commands/**/
enum CANCAT2(FRAME_CLASS_NAME, _IDs)
{
	/*BUTTON*/
	ID_BTN1 = 1
		, ID_BTN2
		, ID_TEXTCTRL1
		, ID_TEXTCTRL2

		/*IDes for menubar*/
		, idMenuQuit
		, idMenuAbout
		, idMenuDuplicateStr
		, idMenuCopyStr
		, idMenuDeleteStr
		, idMenuCalcStrCrd

		/*, CANCAT3(FRAME_CLASS_NAME, _, ENM)/**/
};



// Define a new frame type: this is going to be our main frame
class FRAME_CLASS_NAME
	: public wxFrame
{
private:

#if MY_DEBUG && _DEBUG
	std::vector<wxString> m(history);
	#define TO_HISTORY(ptr) 	m_history.push_back(FRAME_CLASS_NAME::Ptr2Str(ptr))
#else
	#define TO_HISTORY(ptr)
#endif/*TO_HISTORY*/

	wxBoxSizer* m_p(mainBxSzr);
	wxBoxSizer* m_p(strCrdSzr);
#if INCLUDE_WXSQL
	wxSQLite3Database* m_p(db);
#endif

	size_t m(rows);
	const size_t m(dflt_row_hight) = 20;
	wxString m(db_path);


#if ADD_DFLT_LN == 1
	wxArrayString m(cmbx_chc),m(cmbx_optn), m(dflt_vld);
#endif/*ADD_DFLT_LN == 1*/

public:
	CLASS_CONSTRUCT(const wxString& title);
	CLASS_DESTRUCT();

	/*logic*/
	/* evnt handlers*/
	/* (these functions should _not_ be virtual)*/


private:
	void OnTxtCtrlTxt(wxCommandEvent& evnt);
	void OnTxtCtrlTxtEntr(wxCommandEvent& evnt);
	void OnTxtCtrlTxtUpDate(wxCommandEvent& evnt);
	void OnQuit(wxCommandEvent& evnt);
	void OnAbout(wxCommandEvent& evnt);
	void OnDuplicateStr(wxCommandEvent& evnt);
	void OnCopyStr(wxCommandEvent& evnt);
	void OnDeleteStr(wxCommandEvent& evnt);
	void OnCalcStrCrd(wxCommandEvent& evnt);

	wxBoxSizer* CreateStrCrd(wxArrayString& param);
	bool DeleteStrCrd(wxBoxSizer* p_StrCrd);
	wxBoxSizer* GetStrCrd(size_t indx)const;
	/*to SQL-table*/
	void SaveTrajectory(void)const;
	/*from SQL-table*/
	void SelectTrajectory(void);

	/*get 'index' of BxSzr and empty 'params'*/
	wxArrayString& GetParamsFromStrCrd(size_t indx, wxArrayString& params)const;
	/*get 'p_StrCrd' of BxSzr and empty 'params'*/
	wxArrayString& GetParamsFromStrCrd(wxBoxSizer* p_StrCrd, wxArrayString& params)const;
	/*returns nullptr as error*/
	wxBoxSizer* SetParamsToStrCrd(size_t indx, wxArrayString& params, flag_t flags, const wxColour& txtClr);
	/*returns nullptr as error*/
	wxBoxSizer* SetParamsToStrCrd(wxBoxSizer* p_StrCrd, wxArrayString& params, flag_t flags, const wxColour& txtClr);

	bool CalcStrCrd(size_t indx);

	cstr_t Double2Fmt(char* ans, cstr_t fmt, cstr_t source)const;

#if defined(MY_DEBUG)
	cstr_t Ptr2Str(void* ptr);/*convert ptr to string*/
#else
#endif/*TO_HISTORY*/

	wxDECLARE_EVENT_TABLE();    /* any class wishing to process wxWidgets evnts must use this macro*/
};