#ifndef DEBUG_H
#define DEBUG_H

#include <vector>
#include <unordered_map>
#include <string>

#include "App.hpp"

#include "DebugInfo.hpp"

class Debug : public App {

    public:


    static void addLabel (const char *name);
    static void addInfo(const std::string &label, const std::string &info);

    static void update ();
    static void displayInfos ();

    protected:



    // static std::unordered_map<std::string, std::vector<std::string>> m_debugSections;


    private:
    Debug();
    ~Debug();

};


#endif