#ifndef _LOGPANEL_H_
#define _LOGPANEL_H_

//#include <wx/bmpbuttn.h>
#include <wx/panel.h>

#include "OgreString.h"
#include "OgreLog.h"

class wxBoxSizer;
class wxTextCtrl;
class LogToTextRedirector;

using Ogre::String;

class LogPanel : public wxPanel, public Ogre::LogListener
{
public:
	LogPanel(wxWindow* parent, wxWindowID id = wxID_ANY,
             const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
             long style = wxTAB_TRAVERSAL, const wxString& name = wxT("LogPanel"));
	virtual ~LogPanel();

	void attachLog(Ogre::Log *log);
	void detachLog(Ogre::Log *log);

	virtual void messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml = Ogre::LML_NORMAL, bool maskDebug = false, const Ogre::String &logName = Ogre::String(""));

protected:
	wxBoxSizer* mBoxSizer;
	wxTextCtrl* mTextControl;
};

#endif // _LOGPANEL_H_
