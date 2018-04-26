#include "Fraction.hpp"
#include <iostream>
#include <stdlib.h>
#include <cmath>

/**
 * Copyright 2015 Tim Lindquist,
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Purpose: demonstrate simple C++ class
 * Ser321 Foundations of Distributed Applications
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version July 2015
 */
Fraction::Fraction(){
   numerator = 0;
   denominator = 1;
}

Fraction::~Fraction() {
   //cout << "Fraction destructor called." << endl;
   numerator=0;
   denominator=0;
}

void Fraction::setNumerator(int n) {
   numerator = n;
}

int Fraction::getNumerator() {
   return numerator;
}

void Fraction::setDenominator(int n) {
   denominator = n;
}

int Fraction::getDenominator() {
   return denominator;
}

Fraction Fraction::operator+(const Fraction& b){
   Fraction a;
   a.numerator = numerator*b.denominator+b.numerator*denominator;
   a.denominator = denominator * b.denominator;
   return a;
}

Fraction Fraction::operator*(const Fraction& b){
   Fraction a;
   a.numerator = numerator * b.numerator;
   a.denominator = denominator * b.denominator;
   return a;
}

void Fraction::reduce(){
   for (int i = denominator * numerator; i > 1; i--) {  
      if ((denominator % i == 0) && (numerator % i == 0)) {  
         denominator /= i;  
         numerator /= i;  
      }  
                  
   }
}

string Fraction::toString(){
   string ret = "fraction "+std::to_string(numerator)+"/"+std::to_string(denominator);
   return ret;
}
