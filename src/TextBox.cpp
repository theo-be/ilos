
#include <iostream>
#include <string>

#include "TextBox.hpp"
#include "Font.hpp"

using namespace std;

TextBox::TextBox() : message(""), speaker(""), used(false), messageIndex(-1), fontSize(DEFAULT_FONT_SIZE)
{}

TextBox::TextBox(string message, string speaker, int index, int fontSize) : message(message), speaker(speaker), used(false), messageIndex(index), fontSize(fontSize)
{}

void TextBox::show () const {
    
}

bool TextBox::isUsed () const {
    return used;
}

void TextBox::setUsed (bool state) {
    used = state;
}

