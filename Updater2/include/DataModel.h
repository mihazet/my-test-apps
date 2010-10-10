#ifndef _DATA_MODEL_H_
#define _DATA_MODEL_H_

#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/wxsqlite3.h>

enum
{
    MAC_USAGE_ID,
    MAC_USAGE_DATE,
    MAC_USAGE_TIME,
    MAC_USAGE_BOARDNAME,
    MAC_USAGE_OLDMAC,
    MAC_USAGE_NEWMAC,
    MAC_USAGE_OPERATOR,

    MAC_USAGE_MAX
};

class ReportDataModel : public wxDataViewVirtualListModel
{
public:
    ReportDataModel();
    ~ReportDataModel();

    virtual unsigned int GetColumnCount() const;
    virtual wxString GetColumnType(unsigned int col) const;
    virtual unsigned int GetRowCount();
    virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const;
    virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col);
    wxSQLite3Database *GetDB();

private:
    wxSQLite3Database *_db;
};

#endif /* _DATA_MODEL_H_ */

