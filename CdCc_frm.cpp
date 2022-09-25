#include "CdCc_frm.h"

#if 1
wxBEGIN_EVENT_TABLE(FRAME_CLASS_NAME, wxFrame)
	/*EVT_BUTTON*/
	/*EVT_BUTTON(BTN1, FRAME_CLASS_NAME::OnBtn1Clicked)/*for one-time binding*/
	/*EVT_MENU*/	
wxEND_EVENT_TABLE()
#endif

/*size_t FRAME_CLASS_NAME::m_rows = 0;/*static member initializing*/
/*
	class FRAME_CLASS_NAME is going to be a graphical interface component.
	Derived (or inherited) by ::wxFrame class.
	(is going to be a FORM in a "windows" language)
*/

FRAME_CLASS_NAME::FRAME_CLASS_NAME(const wxString& title)
	:
	wxFrame(NULL, wxID_ANY, title, wxPoint(50, 70), wxSize(580, 200))
	, m_p_mainBxSzr(new wxBoxSizer(wxVERTICAL))
	, m_p_strCrdSzr(new wxBoxSizer(wxVERTICAL))
	, m_rows(0)
	, m_db_path(wxGetCwd() + wxT("\\wx_CdCc.db"))
#if ADD_DFLT_LN == 1
	, m_dflt_vld()
	, m_cmbx_chc()
#endif/*ADD_DFLT_LN == 1*/
{
	this->SetMinSize(wxSize(580, 200));
	/*wxBoxSizer* p_mainBS = new wxBoxSizer(wxVERTICAL);/*======== MAIN BoxSizer ========*/

	wxMenuBar* p_menuBar1 = new wxMenuBar();

	wxMenu* p_menuFile = new wxMenu();
	p_menuFile->Append(new wxMenuItem(p_menuFile, idMenuQuit, _("Quit\tAlt+F4"), _("Quit the application"), wxITEM_NORMAL));
	Connect(idMenuQuit, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&FRAME_CLASS_NAME::OnQuit);
	p_menuBar1->Append(p_menuFile, _("&File"));

	wxMenu* p_menuEdit = new wxMenu();
	p_menuEdit->Append(new wxMenuItem(p_menuEdit, idMenuDuplicateStr, _("Duplicate\tCtrl+D"), _("Copy the specified string with values"), wxITEM_NORMAL));
	Connect(idMenuDuplicateStr, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&FRAME_CLASS_NAME::OnDuplicateStr); 
	p_menuEdit->Append(new wxMenuItem(p_menuEdit, idMenuCopyStr, _("Copy\tInsert"), _("Copy the specified string without values"), wxITEM_NORMAL));
	Connect(idMenuCopyStr, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&FRAME_CLASS_NAME::OnCopyStr); 
	p_menuEdit->Append(new wxMenuItem(p_menuEdit, idMenuDeleteStr, _("Delete\tDelete"), _("Delete the specified string"), wxITEM_NORMAL));
	Connect(idMenuDeleteStr, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&FRAME_CLASS_NAME::OnDeleteStr);
	p_menuEdit->Append(new wxMenuItem(p_menuEdit, idMenuCalcStrCrd, _("Calculate"), _("Determine unknown values"), wxITEM_NORMAL));
	Connect(idMenuCalcStrCrd, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&FRAME_CLASS_NAME::OnCalcStrCrd);
	p_menuBar1->Append(p_menuEdit, _("&Edit"));

	wxMenu* p_menuHelp = new wxMenu();
	p_menuHelp->Append(new wxMenuItem(p_menuHelp, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL));
	Connect(idMenuAbout, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&FRAME_CLASS_NAME::OnAbout);
	p_menuBar1->Append(p_menuHelp, _("&Help"));

	SetMenuBar(p_menuBar1);

	/*======== Add StcTxt in the BxSzrStcTxt ========*/
#if BOXSIZER_2 == 1/*wxStaticText*/

	cstr_t stcTxtArr[] =
	{ _("")
		,_("Line/Arc")
		, _("X:")
		, _("Y:")
		, _("Angle:")
		, _("I:")
		, _("J:")
		, _("Len/Rad:")
		, _("Option:") };



	wxBoxSizer* p_BxSzrStcTxt = new wxBoxSizer(wxHORIZONTAL);/*======== Static text on top ========*/
	
	p_BxSzrStcTxt->Add(new wxStaticText(this, wxID_ANY, stcTxtArr[0], wxDefaultPosition, wxDefaultSize, wxTE_CENTRE)
		, 0, wxALL | wxCENTER, 5);/*botom of CheckBox need unstrachable*/
	for (size_t indx = 1; indx < _ARRAYSIZE(stcTxtArr); indx++) {
		p_BxSzrStcTxt->Add(new wxStaticText(this, wxID_ANY, stcTxtArr[indx], wxDefaultPosition, wxDefaultSize, wxTE_CENTRE)/*->SetForegroundColour(*wxBLUE)*/
			, 1, wxALL | wxCENTER, 5);
	}

	this->m_p_mainBxSzr->Add(p_BxSzrStcTxt, 0, wxEXPAND | wxALL, 5);/**/
#endif
	/*TO_HISTORY(p_BxSzrStcTxt);
	TO_HISTORY(m_p_mainBxSzr->GetItem((size_t)0)->GetWindow());
	TO_HISTORY(m_p_mainBxSzr->GetItem((size_t)0)->GetSizer());/**/
	/*======== Add defult line/arc ========*/
	cstr_t arrDfltVld[] =
	{ _("Line")
		, _("?")
		, _("?")
		, _("?")
		, _("?")
		, _("?")
		, _("?")
		, _("?") };

	cstr_t arrCmbxChc[] =
	{ _("Line")
		, _("Arc(CW)")
		, _("Arc(CCW)") };/*change here must be reflected in "GetKnownFlag()"*/

	cstr_t arrCmbxOptn[] = 
	{ _("")
		, _("1")
		, _("2")
		, _("3")
		, _("4") };

	this->m_cmbx_chc = wxArrayString(_ARRAYSIZE(arrCmbxChc), arrCmbxChc);
	this->m_cmbx_optn = wxArrayString(_ARRAYSIZE(arrCmbxOptn), arrCmbxOptn);
	this->m_dflt_vld = wxArrayString(_ARRAYSIZE(arrDfltVld), arrDfltVld);

#if defined(_DEBUG) && 1
	this->m_dflt_vld[1] = wxString::FromDouble(0.0, 3);
	this->m_dflt_vld[2] = wxString::FromDouble(0.0, 3);
	this->m_dflt_vld[3] = wxString::FromDouble(-90.0, 3);
	this->m_dflt_vld[6] = wxString::FromDouble(20.0, 3);
	this->m_p_strCrdSzr->Add(this->CreateStrCrd(this->m_dflt_vld), 0, wxEXPAND | wxALL, 5);

	this->m_dflt_vld[1] = "?";
	this->m_dflt_vld[2] = "?";
	this->m_dflt_vld[3] = wxString::FromDouble(45.0, 3);
	this->m_dflt_vld[6] = wxString::FromDouble(14.142, 3);
	this->m_p_strCrdSzr->Add(this->CreateStrCrd(this->m_dflt_vld), 0, wxEXPAND | wxALL, 5);

	this->m_dflt_vld[1] = wxString::FromDouble(0.0, 3);
	this->m_dflt_vld[2] = wxString::FromDouble(20.0, 3);
	this->m_dflt_vld[3] = "?";
	this->m_dflt_vld[6] = "?";
	this->m_p_strCrdSzr->Add(this->CreateStrCrd(this->m_dflt_vld), 0, wxEXPAND | wxALL, 5);

	this->m_dflt_vld[1] = "?";
	this->m_dflt_vld[2] = "?";
	this->m_dflt_vld[3] = "?";
	this->m_dflt_vld[6] = "?";

#else
	this->m_p_strCrdSzr->Add(this->CreateStrCrd(this->m_dflt_vld), 0, wxEXPAND | wxALL, 5);
#endif/*!_DEBUG && wish*/


	/*======== create a status bar just for fun (by default with 1 pane only) ========/**/
#if wxUSE_STATUSBAR
	CreateStatusBar(2);/*below of the window*/
    SetStatusText("\"FRAME_CLASS_NAME::FRAME_CLASS_NAME(const wxString& title)\"",0);
	SetStatusText("One is glad to be of service",1);
#endif

	this->m_p_mainBxSzr->Add(m_p_strCrdSzr, 0, wxEXPAND | wxALL, 5);/**/

	this->SetSizer(this->m_p_mainBxSzr); // use the sizer for layout
	this->m_p_mainBxSzr->Fit(this);/*fit empty space in window*/
	this->m_p_mainBxSzr->SetSizeHints(this); // set hints to honor min size
}

FRAME_CLASS_NAME::~FRAME_CLASS_NAME() {


}