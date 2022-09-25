#include"CdCc_frm.h"
#if INCLUDE_WXSQLITE3
#include "wx/wxsqlite3.h"
#include<memory>

void TestSQL(void) {

	std::shared_ptr<wxSQLite3Database> shptr_db(new wxSQLite3Database());

}

void Test2(const wxString& fileName) {
	wxCharBuffer(fileName.ToUTF8());
}



#endif