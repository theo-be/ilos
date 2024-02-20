
#ifndef DEBUGINFO_H
#define DEBUGINFO_H


#include <string>
#include <vector>


class DebugInfo {
    public:

    const std::string &getLabel () const;
    void addInfo(const std::string &info);


    private:


    std::string m_label;

    std::vector<std::string> m_infos;
};

#endif