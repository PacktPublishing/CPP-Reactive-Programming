//
//  main.cpp
//  spreadsheet
//
//  Created by Irit Katriel on 15/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#include "spreadsheet.hpp"

#include <iostream>

int main (int argc, const char * argv[])
{ 
    using namespace spreadsheet;

    Spreadsheet sheet;

    Cell<double> a = sheet.NewCell<double>();
    Cell<double> b = sheet.NewCell<double>();
    Cell<double> c = sheet.NewCell<double>();
    Cell<double> d = sheet.NewCell<double>();
    Cell<double> e = sheet.NewCell<double>();
    Cell<double> f = sheet.NewCell<double>();
    
    c.Set(SQRT(a()*a() + b()*b()));
    a.Set(3.0);
    b.Set(4.0);
    d.Set(c()+b());
    e.Set(d()+c());
    
    std::cout << " a=" << a.Value() 
              << " b=" << b.Value() 
              << " c=" << c.Value() 
              << " d=" << d.Value() 
              << " e=" << e.Value() 
              << std::endl;
    
    c.Set(2*(a()+b()));
    c.Set(4*(a()+b()));
    c.Set(5*(a()+b()));
    c.Set(6*(a()+b()));
    c.Set(7*(a()+b()));
    c.Set(8*(a()+b()));
    c.Set(a());

    std::cout << " a=" << a.Value() 
              << " b=" << b.Value() 
              << " c=" << c.Value() 
              << " d=" << d.Value() 
              << " e=" << e.Value() 
              << std::endl;

    c.Set(2*(a()+b()));
    c.Set(4*(a()+b()));
    c.Set(5*(a()+b()));
    c.Set(6*(a()+b()));
    c.Set(7*(a()+b()));
    c.Set(8*(a()+b()));
    c.Set(a()+1);
    
    std::cout << " a=" << a.Value() 
    << " b=" << b.Value() 
    << " c=" << c.Value() 
    << " d=" << d.Value() 
    << " e=" << e.Value() 
    << std::endl;
    
    std::cout << "Goodbye!\n";
    
    return 0;
}

