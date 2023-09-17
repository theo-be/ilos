#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <iostream>
#include <string>

class TextBox {
    public:
    TextBox();
    TextBox(std::string message, std::string speaker, int index, int fontSize);

    void show () const;

    bool isUsed () const;
    void setUsed (bool state);

    private:
    std::string message;
    std::string speaker;
    bool used;
    int messageIndex;
    int fontSize;
};




#endif