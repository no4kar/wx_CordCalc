#include "CdCc_frm.h"



wxBoxSizer*
FRAME_CLASS_NAME::CreateStrCrd(
	wxArrayString& params
) {
	const int flags_for_BxSzr = wxHORIZONTAL | wxALL;/*flags for wxGridSizer* grs1_ptr*/
	wxBoxSizer* p_strCrd = new wxBoxSizer(wxHORIZONTAL);/*++++++++ Create BxSzr ++++++++*/	TO_HISTORY(p_strCrd);

	wxCheckBox* p_ChckBx = new wxCheckBox(this
		, wxID_ANY
		, ""
		, wxDefaultPosition
		, wxSize(20, this->m_dflt_row_hight)
		, wxALIGN_CENTER);/*++++++++ Create ChckBx ++++++++*/
	p_strCrd->Add(p_ChckBx, 0, flags_for_BxSzr, 2);/*++++++++ Add ChckBx in the BxSzr ++++++++*/	TO_HISTORY(p_ChckBx);

	wxComboBox* p_CmbBx = new wxComboBox(this
		, wxID_ANY
		, params[0]
		, wxDefaultPosition
		, wxDefaultSize
		, this->m_cmbx_chc
		, wxTE_CENTRE | wxCB_READONLY | wxCB_SORT
	);/*++++++++ Create ComboBox(Line/Arc) for the BxSzr ++++++++*/	TO_HISTORY(p_CmbBx);
	p_strCrd->Add(p_CmbBx, 1, flags_for_BxSzr, 2);/*++++++++ Add Create ComboBox(Line/Arc) in the BxSzr ++++++++*/



	wxTextValidator vld(wxFILTER_INCLUDE_CHAR_LIST);/*text validater with charInclLst*/
	vld.SetCharIncludes("0123456789+-.?");

	for (size_t indx = 1/*miss ComboBox(Line/Arc)*/, paramSz = params.size() - 1/*leave the last for ComboBox*/
		; indx < paramSz; indx++) {
		wxTextCtrl* p_TC_temp = new wxTextCtrl(this
			, wxID_ANY
			, params[indx]
			, wxDefaultPosition
			, wxDefaultSize
			, wxTE_CENTRE/*| wxTE_PROCESS_ENTER/**/
			, vld);/*++++++++ Create TxtCtrl(TC) for the BxSzr ++++++++*/	TO_HISTORY(p_TC_temp);

		p_TC_temp->Bind(wxEVT_TEXT/*wxTE_PROCESS_ENTER need wxTextCtrl(,long stile = wxTE_PROCESS_ENTER)*/
			, &FRAME_CLASS_NAME::OnTxtCtrlTxt
			, this);
		p_TC_temp->SetMaxLength(10);
		p_TC_temp->SetMinSize(p_CmbBx->GetSize());

		p_strCrd->Add(p_TC_temp, 1, flags_for_BxSzr, 2);/*++++++++ Add TxtCtrl(TC) in the BxSzr ++++++++*/
	}

	p_CmbBx = new wxComboBox(this
		, wxID_ANY
		, std::move("")
		, wxDefaultPosition
		, wxDefaultSize
		, this->m_cmbx_optn
		, wxTE_CENTRE /* | wxCB_READONLY */ | wxCB_SORT
	);/*++++++++ Create ComboBox(Option) for the BxSzr ++++++++*/	TO_HISTORY(p_CmbBx);
	p_strCrd->Add(p_CmbBx, 1, flags_for_BxSzr, 2);/*++++++++ Add Create ComboBox(Option) in the BxSzr ++++++++*/

	m_rows++;/*++++++++ Count(add) BxSzr ++++++++*/

	return p_strCrd;
}

bool
FRAME_CLASS_NAME::DeleteStrCrd(
	wxBoxSizer* p_strCrd
) {
	p_strCrd->DeleteWindows();/*solution firstone delete all windows from BoxSizer*/
	return this->m_p_strCrdSzr->Remove(p_strCrd);
}

wxArrayString&
FRAME_CLASS_NAME::GetParamsFromStrCrd(
	size_t indx
	, wxArrayString& params
)const {
	return this->GetParamsFromStrCrd(this->GetStrCrd(indx), params);
}

wxArrayString&
FRAME_CLASS_NAME::GetParamsFromStrCrd(
	wxBoxSizer* p_strCrd
	, wxArrayString& params
)const {
	if (p_strCrd == nullptr)
		return params;/*BoxSizer with this 'indx' don't exist*/

	wxComboBox* p_CmbBx;
	wxTextCtrl* p_TxtCtrl;
	const wxSizerItemList& StrCrdPrm = p_strCrd->GetChildren();/*list of BxSzr*/
	/*wxSizerItemList::const_iterator mainIter = SzrItmLst.begin()
		, endIter = SzrItmLst.end();/**/
	size_t indx = 1/*miss wxCheckBox*/
		, endIndx = StrCrdPrm.GetCount() - 1;/**/

#if defined(_DEBUG) 
	p_CmbBx = (wxComboBox*)((StrCrdPrm[indx])->GetWindow());/*now need '(*mainIter)->GetWindow()' becouse 'wxComboBox' is not a 'Sizer'*/
	params.Add(p_CmbBx->GetValue());
#else
	params.Add(((wxComboBox*)((StrCrdPrm[endIndx])->GetSizer()))->GetValue());
#endif

	++indx;/*miss wxComboBox*/

	for (
		; indx < endIndx/*leave last wxComboBox(OPTION)*/
		; ++indx
		) {
#if defined(_DEBUG)
		p_TxtCtrl = (wxTextCtrl*)((StrCrdPrm[indx])->GetWindow());
		params.Add(p_TxtCtrl->GetValue());
#else
		params.Add(((wxTextCtrl*)((StrCrdPrm[endIndx])->GetSizer()))->GetValue());
#endif		
	}

#if defined(_DEBUG)
	p_CmbBx = (wxComboBox*)((StrCrdPrm[endIndx])->GetWindow());/*now need '(*mainIter)->GetWindow()' becouse 'wxComboBox' is not a 'Sizer'*/
	params.Add(p_CmbBx->GetValue());
#else
	params.Add(((wxComboBox*)((StrCrdPrm[endIndx])->GetSizer()))->GetValue());
#endif

	return params;
	}

wxBoxSizer*
FRAME_CLASS_NAME::SetParamsToStrCrd(
	size_t indx
	, wxArrayString& params
	, const flag_t flags = ~((flag_t)0)
	, const wxColour& txtClr = *wxBLACK
) {
	return 	this->SetParamsToStrCrd(this->GetStrCrd(indx), params, flags, txtClr);
}

wxBoxSizer*
FRAME_CLASS_NAME::SetParamsToStrCrd(
	wxBoxSizer* p_strCrd
	, wxArrayString& params
	, const flag_t flags = ~((flag_t)0)
	, const wxColour& txtClr = *wxBLACK
) {
	if (p_strCrd == nullptr)
		return nullptr;/*BoxSizer with this 'indx' don't exist*/

	wxComboBox* p_CmbBx;
	wxTextCtrl* p_TxtCtrl;
	const wxSizerItemList& StrCrdPrm = p_strCrd->GetChildren();/*list of BxSzr*/
	size_t indx = 2/*skip wxCheckBox and skip wxComboBox, modefyed by user only*/
		, endIndx = StrCrdPrm.GetCount() - 1;/**/
	flag_t flag_id = 1;

	if (endIndx/*without wxCheckBox*/ != params.GetCount())
		return nullptr;

	/*wxSizerItemList::const_iterator mainIter = SzrItmLst.begin()
		, endIter = SzrItmLst.end();/**/
	wxArrayString::const_iterator prmsIter = params.begin()
		, endPrmsIter = params.end();

	++prmsIter;/*skip 'Line/Arc', modefyed by user only*/

	for (
		; (indx < endIndx)/*leave last wxComboBox(OPTION)*/ && (prmsIter != endPrmsIter)
		; ++indx, ++prmsIter, flag_id <<= 1
		) {
		if ((flags & flag_id) == NULL)continue;
		p_TxtCtrl = (wxTextCtrl*)((StrCrdPrm[indx])->GetWindow());/*now need '(*mainIter)->GetWindow()' becouse 'wxTextCtrl' is not a 'Sizer'*/
		p_TxtCtrl->SetForegroundColour(txtClr);
		p_TxtCtrl->ChangeValue(*prmsIter);/*ChangeValue() doesn't generate wxEVT_TEXT event, SetValue() does*/
	}

	if ((flags & flag_id) != NULL) {/*wxComboBox(OPTION)*/
		p_CmbBx = (wxComboBox*)((StrCrdPrm[endIndx])->GetWindow());
		p_CmbBx->SetForegroundColour(txtClr);
		p_CmbBx->ChangeValue(*prmsIter);/*ChangeValue() doesn't generate wxEVT_TEXT event, SetValue() does*/
	}

	return p_strCrd;
}

/*Get BoxSizer-child from mainBoxSizer(m_p_strCrdSzr). If not exist, return nullptr*/
wxBoxSizer*
FRAME_CLASS_NAME::GetStrCrd(
	size_t indx
)const {
	return (wxBoxSizer*)this->m_p_strCrdSzr->GetItem((size_t)indx)/*Returns pointer to item or NULL*/->GetSizer();
}