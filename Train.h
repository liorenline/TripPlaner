#pragma once
#include <string>

class Train {
protected:
    std::string from;
    std::string to;
    std::string dep;
    double dur;
    double price;

public:
    std::string getFrom()  const { return from;}
    std::string getTo()    const { return to;}
    std::string getDep()   const { return dep;}
    double  getDur()   const { return dur;}
    double  getPrice() const { return price;}

    void setFrom(std::string f)  { from  = f; }
    void setTo(std::string t) { to = t; }
    void setDep(std::string d){ dep= d; }
    void setDur(double d) { dur= d; }
    void setPrice(double p) { price = p; }
};