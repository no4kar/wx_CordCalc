#include "CdCc_frm.h"



#define SQL_TBL_NAME "trajectory"
#define SQL_TBL_SCHM SQL_TBL_NAME \
		" ( id INT PRIMARY KEY NOT NULL"\
		", line_arc TEXT NOT NULL"\
		", x TEXT NOT NULL"\
		", y TEXT NOT NULL"\
		", angle TEXT NOT NULL"\
		", i TEXT NOT NULL"\
		", j TEXT NOT NULL"\
		", len_rad TEXT NOT NULL"\
		", opt TEXT)"


/**/
void
FRAME_CLASS_NAME::SaveTrajectory(
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

	Res = this->m_p_db->ExecuteQuery("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='" SQL_TBL_NAME "';");/*if exists one return "1"*/
	if (std::strcmp(Res.GetAsString(0), "1") == NULL) {
		Res = this->m_p_db->ExecuteQuery("DELETE FROM " SQL_TBL_NAME ";");
	}
	else {
		Res = this->m_p_db->ExecuteQuery("CREATE TABLE IF NOT EXISTS " SQL_TBL_SCHM ";");
	}

	ASK_ACT(Res.IsOk() == NULL, assert(0));

	for (size_t indx = 0; indx < strCrdCount; indx++)
	{
		this->GetParamsFromStrCrd(indx, params);

		ASK_ACT(params.GetCount() != 8, assert(0));/*id+8 columls in sqlite-table*/

		std::sprintf(buff, "INSERT INTO " SQL_TBL_NAME " VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');"
			, indx, params[0].ToStdString().c_str(), params[1].ToStdString().c_str(), params[2].ToStdString().c_str(), params[3].ToStdString().c_str()
			, params[4].ToStdString().c_str(), params[5].ToStdString().c_str(), params[6].ToStdString().c_str(), params[7].ToStdString().c_str());

		Res = this->m_p_db->ExecuteQuery((const char*)buff);
		params.clear();
	}

	this->m_p_db->Close();

	MY_CATCH(1)


#endif
}

/**/
void
FRAME_CLASS_NAME::SelectTrajectory(
	void
) {
	/*save to D:\3\myProjects\cdcc.db*/
#if INCLUDE_WXSQL && 1
	wxSQLite3ResultSet Res;
	wxArrayString params;
	size_t indx = 0;


	MY_TRY(1)

		this->m_p_db->Open(this->m_db_path);

	Res = this->m_p_db->ExecuteQuery("SELECT sql FROM sqlite_master WHERE type='table' AND name='" SQL_TBL_NAME "';");

	ASK_ACT(Res.NextRow() == NULL, assert(0));

	/*std::strstr(haystack, needle);*//* "IF NOT EXISTS" and ";" will be cut off*/


	ASK_ACT(std::strstr(Res.GetAsString(0).c_str(), SQL_TBL_SCHM) != NULL, assert(0));

	Res = this->m_p_db->ExecuteQuery("SELECT * FROM " SQL_TBL_NAME ";");

	for (size_t indx = 0; Res.NextRow(); indx++)
	{
		for (size_t i = 1, length = Res.GetColumnCount(); i < length; i++) {
			params.Add(Res.GetAsString(i));
		}

		this->m_p_strCrdSzr->Add(this->CreateStrCrd(params), 0, wxEXPAND | wxALL, 5);
		params.clear();
	}
	Res.Finalize();
	this->m_p_db->Close();

	MY_CATCH(1)


#endif
}