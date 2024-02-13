#ifndef DEBUG_H
#define DEBUG_H

#include <vector>

#include "App.hpp"

#include "DebugInfo.hpp"

class Debug : public App {

    public:


    static void addLabel (const char *name);
    static void addInfo(const char *label, const std::string &info);

    static void update ();


    protected:



    static std::vector<DebugInfo> m_debugSections;


    private:
    Debug();
    ~Debug();

};


#endif