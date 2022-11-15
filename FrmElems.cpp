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
	params.Add(((wxComboBox*)((StrCrdPrm[indx])->GetWindow()))->GetValue());
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
		params.Add(((wxTextCtrl*)((StrCrdPrm[indx])->GetWindow()))->GetValue());
#endif		
	}

#if defined(_DEBUG)
	p_CmbBx = (wxComboBox*)((StrCrdPrm[endIndx])->GetWindow());/*now need '(*mainIter)->GetWindow()' becouse 'wxComboBox' is not a 'Sizer'*/
	params.Add(p_CmbBx->GetValue());
#else
	params.Add(((wxComboBox*)((StrCrdPrm[endIndx])->GetWindow()))->GetValue());
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

/**/
void
FRAME_CLASS_NAME::SaveStrCrds(
	void
)const {
	/*save to D:\3\myProjects\cdcc.db*/
#if INCLUDE_WXSQL && 1
	wxSQLite3ResultSet Res;
	wxArrayString params;
	size_t strCrdCount = (this->m_p_strCrdSzr->GetChildren()).GetCount();
	size_t indx = 0;
	char buff[0xff] = { 0 };

	MY_TRY(1)

		this->m_p_db->Open(this->m_db_path);

	Res = this->m_p_db->ExecuteQuery("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='strcrd';");/*if exists one return "1"*/
	if (std::strcmp(Res.GetAsString(0), "1") == NULL) {
		Res = this->m_p_db->ExecuteQuery("DELETE FROM strcrd;");
	}
	else {
		Res = this->m_p_db->ExecuteQuery("CREATE TABLE IF NOT EXISTS "
			"strcrd"
			"( id INT PRIMARY KEY NOT NULL"
			", line_arc TEXT NOT NULL"
			", x TEXT NOT NULL"
			", y TEXT NOT NULL"
			", angle TEXT NOT NULL"
			", i TEXT NOT NULL"
			", j TEXT NOT NULL"
			", len_rad TEXT NOT NULL"
			", opt TEXT);");
	}

	ASK_ACT(Res.IsOk() == NULL, assert(0));

	for (size_t indx = 0; indx < strCrdCount; indx++)
	{
		this->GetParamsFromStrCrd(indx, params);

		ASK_ACT(params.GetCount() != 8, assert(0));/*id+8 columls in sqlite-table*/

		std::sprintf(buff, "INSERT INTO strcrd VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');"
			, indx, params[0].c_str(), params[1].c_str(), params[2].c_str(), params[3].c_str()
			, params[4].c_str(), params[5].c_str(), params[6].c_str(), params[7].c_str());

		Res = this->m_p_db->ExecuteQuery((const char*)buff);
		params.clear();
	}

	this->m_p_db->Close();

	MY_CATCH(1)


#endif
}

/**/
void
FRAME_CLASS_NAME::SelectStrCrds(
	void
) {
	/*save to D:\3\myProjects\cdcc.db*/
#if INCLUDE_WXSQL && 1
	wxSQLite3ResultSet Res;
	wxArrayString params;
	/*size_t strCrdCount;/**/
	size_t indx = 0;
	cstr_t strCrdTblSchm =
		"strcrd"
		"(id INT PRIMARY KEY NOT NULL"
		", line_arc TEXT NOT NULL"
		", x TEXT NOT NULL"
		", y TEXT NOT NULL"
		", angle TEXT NOT NULL"
		", i TEXT NOT NULL"
		", j TEXT NOT NULL"
		", len_rad TEXT NOT NULL"
		", opt TEXT)";

	MY_TRY(1)

	this->m_p_db->Open(this->m_db_path);

	Res = this->m_p_db->ExecuteQuery("SELECT sql FROM sqlite_master WHERE type='table' AND name='strcrd';");

	ASK_ACT(Res.NextRow() == NULL, assert(0));

	/*std::strstr(haystack, needle);*//* "IF NOT EXISTS" and ";" will be cut off*/


	ASK_ACT(std::strstr(Res.GetAsString(0).c_str(), strCrdTblSchm) != NULL, assert(0));

	Res = this->m_p_db->ExecuteQuery("SELECT * FROM strcrd;");
	
	for (size_t indx = 0; Res.NextRow(); indx++)
	{
		for (size_t i = 1, length = Res.GetColumnCount(); i < length; i++) {
			params.Add(Res.GetAsString(i));	}

		this->m_p_strCrdSzr->Add(this->CreateStrCrd(params), 0, wxEXPAND | wxALL, 5);
		params.clear();
	}
	Res.Finalize();
	this->m_p_db->Close();

	MY_CATCH(1)


#endif
}