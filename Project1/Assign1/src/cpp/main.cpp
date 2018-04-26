#include "Fraction.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

/**
 * Copyright (c) 2015 Tim Lindquist,
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Purpose: Deomonstrate simple C++ class
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * @file    main.cpp
 * @date    July, 2015
 **/
int main() {
   // when using heap (new operator), accessing member methods is via ->
   Fraction * aFract = new Fraction();
   cout<<"aFract: "<<aFract->toString()<<endl;
   aFract->setNumerator(4);
   aFract->setDenominator(3);
   cout<<"aFract: "<<aFract->toString()<<endl;

   // when using stack (automatic), accessing member methods is via dot notation
   Fraction bFract;
   bFract.setNumerator(3);
   bFract.setDenominator(2);
   cout<<"bFract: "<<bFract.toString()<<endl;

   Fraction cFract;
   cFract = *aFract + bFract;
   cout << aFract->toString() << " + " << bFract.toString() << " = " <<
           cFract.toString() << endl;
   cFract.reduce();
   cout << aFract->toString() << " + " << bFract.toString() << " = " <<
           cFract.toString() << " reduced" << endl;

   cFract = *aFract * bFract;
   cout << aFract->toString() << " * " << bFract.toString() << " = " <<
           cFract.toString() << endl;
   cFract.reduce();
   cout << aFract->toString() << " * " << bFract.toString() << " = " <<
           cFract.toString() << " reduced" << endl;

   return (0);
}
