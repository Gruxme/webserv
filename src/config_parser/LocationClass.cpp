# include "LocationClass.hpp"

/* ----- Constructors & Destructor respectively ----- */
LocationClass::LocationClass() : __path(""), __root(""), __cgi_ext("") {}
LocationClass::~LocationClass() {}

/* ----- Location Parser ----- */
void    LocationClass::parseLocation( std::string buffer ) {
    if (buffer.find("path = ") != std::string::npos)
        this->__path = buffer.substr(buffer.find("path = ") + strlen("path = "));
    else if (buffer.find("root = ") != std::string::npos)
        this->__root = buffer.substr(buffer.find("root = ") + strlen("root = "));
    else if (buffer.find("cgi_ext") != std::string::npos) {
        this->__cgi_ext = buffer.substr(buffer.find("cgi_ext = ") + strlen("cgi_ext = "));
    }
}

/* -- PUBLIC METHODS */
std::vector<std::string> LocationClass::split( std::string str, char separator ) {
    std::vector<std::string>  myvec;
    __SIZE_TYPE__ currentIndex = 0, i = 0, startIndex = 0, endIndex = 0;
    while (i <= str.length()) {
        if (str[i] == separator || i == str.length()) {
            endIndex = i;
            std::string subString = "";
            subString.append(str, startIndex, endIndex - startIndex);
            myvec.push_back(subString);
            currentIndex++;
            startIndex = endIndex + 1;
        }
        i++;
    }
    return myvec;
}

/* ----- Getters----- */
std::string LocationClass::getPath( void ) const { return this->__path; }
std::string LocationClass::getRoot( void ) const { return this->__root; }
std::string LocationClass::getCgiExt( void ) const { return this->__cgi_ext; }