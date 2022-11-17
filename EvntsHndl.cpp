#include "CdCc_frm.h"
/*#include <wx\regex.h>/*regular expretion*/
#include <regex>/*std::regex*/
#include <string>

/*================ evnt_HANDLERS ================*/



void
FRAME_CLASS_NAME::OnQuit(
	wxCommandEvent& evnt
) {

	this->SaveTrajectory();
	this->Close();
	evnt.Skip();/**/
}

void
FRAME_CLASS_NAME::OnAbout(
	wxCommandEvent& evnt
) {
	wxMessageBox(_("Developer:\n\nBakhtiiarAsadov n4k\nNikopol UKRAINE."), _("wx_Coordinate_Calculater"));
	evnt.Skip();
}

void
FRAME_CLASS_NAME::OnDuplicateStr(
	wxCommandEvent& evnt
) {
	const wxSizerItemList& szrItmLst = this->m_p_strCrdSzr->GetChildren();/*list of BxSzr*/
	size_t chldCnt = szrItmLst.GetCount();
	wxArrayString paramsBuff;
	for (int i = chldCnt - 1/*count2indx*/; i >= 0; i--)/*search from below to above*/
	{
		wxBoxSizer* p_strCrd = this->GetStrCrd(i);
		wxSizerItemList::const_iterator iter = p_strCrd->GetChildren().begin();

		if (((wxCheckBox*)((*iter)->GetWindow()))->GetValue())
		{
			this->m_p_strCrdSzr->Insert(i + 1/*add below*/, this->CreateStrCrd(this->GetParamsFromStrCrd(p_strCrd, paramsBuff)), 0, wxEXPAND | wxALL, 5);
			paramsBuff.Clear();
		}
	}

	this->m_p_mainBxSzr->Layout();/*Call this to force layout of the children anew*/
	evnt.Skip();/**/
}

void
FRAME_CLASS_NAME::OnCopyStr(
	wxCommandEvent& evnt
) {

	const wxSizerItemList& szrItmLst = this->m_p_strCrdSzr->GetChildren();/*list of BxSzr*/
	for (int i = szrItmLst.GetCount() - 1/*count2indx*/; i >= 0; i--)/*search from below to above*/
	{
		wxBoxSizer* p_strCrd = this->GetStrCrd(i);
		wxSizerItemList::const_iterator iter = p_strCrd->GetChildren().begin();
		
		if (((wxCheckBox*)((*iter)->GetWindow()))->GetValue())
		{
			this->m_p_strCrdSzr->Insert(i + 1/*add below*/, this->CreateStrCrd(this->m_dflt_vld), 0, wxEXPAND | wxALL, 5);
		}
	}

	/*this->SetSizer(this->m_p_mainBxSzr);/**/
	this->m_p_mainBxSzr->Layout();/*Call this to force layout of the children anew*/
	evnt.Skip();
}

void
FRAME_CLASS_NAME::OnDeleteStr(
	wxCommandEvent& evnt
) {

	const wxSizerItemList& szrItmLst = this->m_p_strCrdSzr->GetChildren();/*list of BxSzr*/
	size_t chldCnt = szrItmLst.GetCount();
	for (int i = chldCnt - 1/*count2indx*/; i >= 0 && chldCnt > 1/*leave one strCrd*/; i--)/*search from below to above*/
	{
		wxBoxSizer* p_strCrd = this->GetStrCrd(i);
		wxSizerItemList::const_iterator iter = p_strCrd->GetChildren().begin();

		if (((wxCheckBox*)((*iter)->GetWindow()))->GetValue())
		{
			this->DeleteStrCrd(p_strCrd);
			--chldCnt;
		}
	}

	this->m_p_mainBxSzr->Layout();/*Call this to force layout of the children anew*/
	evnt.Skip();/**/
}

void
FRAME_CLASS_NAME::OnCalcStrCrd(
	wxCommandEvent& evnt
) {
	bool result = false;
	
	for (size_t i = 0, length = this->m_p_strCrdSzr->GetItemCount()
		; i < length
		; i++)
		result = CalcStrCrd(i);

	this->m_p_mainBxSzr->Layout();
	evnt.Skip();/**/
}


void
FRAME_CLASS_NAME::OnTxtCtrlTxtEntr(
	wxCommandEvent& evnt
) {
	wxTextCtrl* p_TC = (wxTextCtrl*)(evnt.GetEventObject());
	p_TC->SetForegroundColour(*wxBLACK);
#if defined(_REGEX_)

	std::smatch strsMtch;
	std::string txtCtrVal(p_TC->GetValue().c_str());
	if (std::regex_match(txtCtrVal, strsMtch, std::regex("^[+-]?\\d*[\\.\\,]?\\d*"))) {
		p_TC->ChangeValue/*ChangeValue() doesn't generate wxEVT_TEXT event, SetValue() does*/
		(wxString::FromDouble(std::strtod(strsMtch[0].str().c_str(), NULL), PRECISION/*precision*/));
	}
	else {
		p_TC->ChangeValue/*ChangeValue() doesn't generate wxEVT_TEXT event, SetValue() does*/("?");
	}
	
#elif wxUSE_REGEX
	
#endif/*wxRegEx || std::regex*/

 evnt.Skip();/**/
}

void
FRAME_CLASS_NAME::OnTxtCtrlTxt(
wxCommandEvent& evnt
) {
	this->OnTxtCtrlTxtEntr(evnt);
	evnt.Skip();/**/
}

void
FRAME_CLASS_NAME::OnTxtCtrlTxtUpDate(
	wxCommandEvent& evnt
) {
	this->OnAbout(evnt);
	evnt.Skip();/**/
}