#include "LogToTextRedirector.h"

#include <wx/textctrl.h>
#include "OgreLogManager.h"

using Ogre::Log;
using Ogre::LogListener;
using Ogre::LogManager;
using Ogre::LogMessageLevel;

LogToTextRedirector::LogToTextRedirector(wxTextCtrl* textCtrl, const Ogre::String& logName)
  : mTextControl(textCtrl)
{
    mLogName = logName;
    Log* log = LogManager::getSingletonPtr()->getLog(mLogName);
    if(log) log->addListener(this);
}

LogToTextRedirector::~LogToTextRedirector()
{
    if(LogManager::getSingletonPtr())
    {
        Log* log = LogManager::getSingletonPtr()->getLog(mLogName);
        if(log) log->removeListener(this);
    }
}

void LogToTextRedirector::messageLogged(const Ogre::String& message,
                                        Ogre::LogMessageLevel level,
                                        bool maskDebug,
                                        const Ogre::String &logName)
{
    if(mTextControl && !mTextControl->IsBeingDeleted())
        (*mTextControl) << wxT("[") << wxString(logName.c_str()) << wxT("] ") << message << wxT("\n");
}
