#include <wx/wx.h>
#include "SimCubeApp.h"
#include "HistoryPane.h"

HistoryPane::HistoryPane()
{
    Init();
}

HistoryPane::HistoryPane(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

HistoryPane::~HistoryPane()
{

}

bool HistoryPane::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void HistoryPane::Init()
{
}

void HistoryPane::CreateControls()
{
}

////////////////////////////////////////////////////////////////////////////
HistoryDataModel::HistoryDataModel() : wxDataViewVirtualListModel(2)
{
    _db = wxGetApp().GetMemDatabase();
}

void HistoryDataModel::GetValueByRow(wxVariant &variant, unsigned int row,
                                     unsigned int col) const
{
    variant = "xxx";
}

bool HistoryDataModel::GetAttrByRow(unsigned int row, unsigned int col,
                                    wxDataViewItemAttr &attr)
{
    return false;
}

bool HistoryDataModel::SetValueByRow(wxVariant &variant,
                                     unsigned int row, unsigned int col)
{
    return false;
}

