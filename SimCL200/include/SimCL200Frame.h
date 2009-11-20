#ifndef _SIMCL200FRAME_H_
#define _SIMCL200FRAME_H_

#include <wx/wx.h>
#include <wx/timer.h>
#include <wx/ribbon/bar.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/ribbon/toolbar.h>
#include <wx/ribbon/gallery.h>

class wxSerialPort;

class SimCL200Frame: public wxFrame
{    

public:
    /// Constructors
    SimCL200Frame();
    SimCL200Frame(wxWindow *parent, wxWindowID id = wxID_ANY,
                  const wxString &caption = wxT("Chroma meter CL-200 RS232 Simulator"),
                  const wxPoint &pos = wxDefaultPosition,
                  const wxSize &size = wxDefaultSize,
                  long style = wxDEFAULT_FRAME_STYLE);

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
                const wxString &caption = wxT("Chroma meter CL-200 RS232 Simulator"),
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE);

    /// Destructor
    ~SimCL200Frame();

private:
    void Init();
    void CreateControls();

    void OnReadSerialPortTimer(wxTimerEvent &event);
    void OnBaudRateClicked(wxRibbonButtonBarEvent &event);
    void OnBaudRateDropDown(wxRibbonButtonBarEvent &event);

    void AppendDataToRingBuffer(unsigned char *buf, size_t len);
    void ParseDataLengthFinish(unsigned long len);
    unsigned char GetDataAtRingBufferOffset(unsigned long offset);
    unsigned char CalculateBCC(unsigned char *msg, size_t len);
    void ProcessCL200ShortMessage(unsigned char *msg);
    void ProcessCL200LongMessage(unsigned char *msg);

    wxRibbonBar *m_ribbon;
    wxTimer *m_readTimer;
    wxSerialPort *m_port;
    unsigned char *m_ringBuffer;
    unsigned long m_parseIndex;
    unsigned long m_appendIndex;
    wxDateTime m_previousAppendTime;

    DECLARE_CLASS(SimCL200Frame)
    DECLARE_EVENT_TABLE()
};

#endif
    // _SIMCL200FRAME_H_
