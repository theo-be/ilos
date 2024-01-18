#ifndef DEBUG_H
#define DEBUG_H

#include "App.hpp"

class Debug : public App {

    public:

    Debug();
    ~Debug();

    static void addLabel (const char *label);
    static void addInfo(const char *label);

    static void update ();


    protected:

    // file des infos de debug a afficher

    // faire une map de liste chainee des infos

    // comment gerer les types de var

};



#endif