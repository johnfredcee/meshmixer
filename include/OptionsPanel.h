
#ifndef OPTIONS_PANEL_H_INCLUDED
#define OPTIONS_PANEL_H_INCLUDED

#include <wx/panel.h>

class wxCheckBox;
class wxFlexGridSizer;
class wxPanel;

class OptionsPanel : public wxPanel
{
    DECLARE_CLASS(OptionsPanel);
public:
    OptionsPanel(wxWindow* parent, wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                 long style = wxTAB_TRAVERSAL, const wxString& name = "OptionsPanel");
    virtual ~OptionsPanel();
    virtual bool TransferDataFromWindow();
    virtual bool TransferDataToWindow();
    unsigned int getOptions(void);
    void setOptions(unsigned int);
private:
    unsigned int mOptions;

    wxCheckBox* mCheckBox0; 
    wxCheckBox* mCheckBox1; 
    wxCheckBox* mCheckBox2;
    wxCheckBox* mCheckBox3;
    wxCheckBox* mCheckBox4;
    wxCheckBox* mCheckBox5;
    wxCheckBox* mCheckBox6;
    wxCheckBox* mCheckBox7;
    wxCheckBox* mCheckBox8;
    wxCheckBox* mCheckBox9;
    wxCheckBox* mCheckBox10;
    wxCheckBox* mCheckBox11;
    wxCheckBox* mCheckBox12;
    wxCheckBox* mCheckBox13;
    wxCheckBox* mCheckBox14;
    wxCheckBox* mCheckBox15;
    wxCheckBox* mCheckBox16;
    wxCheckBox* mCheckBox17;
    wxCheckBox* mCheckBox18;
    wxCheckBox* mCheckBox19;

    wxFlexGridSizer* mSizer;
};

#endif
