#ifndef _LOGTOTEXTREDIRECTOR_H_
#define _LOGTOTEXTREDIRECTOR_H_

#include "OgreLog.h"
#include "OgreString.h"

class wxTextCtrl;

class LogToTextRedirector : public Ogre::LogListener
{
    public:
        LogToTextRedirector(wxTextCtrl* textCtrl, const Ogre::String& logName);
        ~LogToTextRedirector();

        void messageLogged(const Ogre::String& message, Ogre::LogMessageLevel level, bool maskDebug, const Ogre::String &logName);

    protected:
        wxTextCtrl* mTextControl;
        Ogre::String mLogName;
};

#endif // _LOGTOTEXTREDIRECTOR_H_

