#include "Waypoint.hpp"
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <sstream>
#include <jsoncpp/json/json.h>
#include <FL/Fl_Input.H>

/**
 * Copyright 2018 Tim Lindquist,
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
 * Purpose: demonstrate classes, alloc, init, in the context of
 * distance and direction calculations from one earth position (waypoint)
 * to another.
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version January 2018
 */
Waypoint::Waypoint() {
    lat = lon = ele = 0;
    addr = "";
    name = "";
}

Waypoint::Waypoint(double aLat, double aLon, double anElevation, string aName, string aAddr) {
    lat = aLat;
    lon = aLon;
    ele = anElevation;
    name = aName;
    addr = aAddr;
}


Waypoint::Waypoint(Json::Value &json) {
    this->lat = json["lat"].asDouble();
    this->lon = json["lon"].asDouble();
    this->ele = json["ele"].asDouble();
    this->name = json["name"].asString();
    this->addr = json["addr"].asString();
};


Waypoint::~Waypoint() {
    //cout << "Waypoint destructor.\n";
    lat = lon = ele = 0;
    name.clear();
    addr.clear();
}


void Waypoint::print() {
    cout << "Waypoint " << name << " lat "
         << lat << " lon " << lon << "\n";
}

string Waypoint::toStr(double theDouble) {
    char charFormat[10];
    sprintf(charFormat, "%4.4f", theDouble);  //format the double into a C string
    std::string returnStr(charFormat);   //convert formatted C str to C++ str
    return returnStr;
}

string Waypoint::getEleStr() {
    return this->toStr(this->ele);
}

string Waypoint::getLonStr() {
    return this->toStr(this->lon);
}

string Waypoint::getLatStr() {
    return this->toStr(this->lat);
}

Json::Value Waypoint::toJson() {
    Json::Value json;
    json["lat"] = this->lat;
    json["lon"] = this->lon;
    json["ele"] = this->ele;
    json["name"] = this->name;
    json["addr"] = this->addr;

    return json;
}


double Waypoint::distanceGCTo(Waypoint *wp, int scale) {
    double lat1 = this->lat;
    double lon1 = this->lon;

    double lat2 = wp->lat;
    double lon2 = wp->lon;

    double radianLat1 = this->toRadians(lat1);
    double radianLat2 = this->toRadians(lat2);
    double radianDeltaLat = this->toRadians(lat2 - lat1);
    double radianDeltaLon = this->toRadians(lon2 - lon1);

    double a = sin(radianDeltaLat / 2) * sin(radianDeltaLat / 2) +
               cos(radianLat1) * cos(radianLat2) * sin(radianDeltaLon / 2) * sin(radianDeltaLon / 2);
    double c = 2 * std::atan2(sqrt(a), sqrt(1 - a));

    double ret = (this->radiusE * c);

    switch (scale) {
        case STATUTE:
            ret = ret * 0.62137119;
            break;
        case NAUTICAL:
            ret = ret * 0.5399568;
            break;
    }
    return (std::floor(ret * 100 + 0.5) / 100);
}


double Waypoint::bearingGCInitTo(Waypoint *wp, int scale) {
    double lat1 = this->lat;
    double lon1 = this->lon;

    double lat2 = wp->lat;
    double lon2 = wp->lon;

    double radianLat1 = this->toRadians(lat1);
    double radianLat2 = this->toRadians(lat2);
    double radianDeltaLon = this->toRadians(lon2 - lon1);

    double y = sin(radianDeltaLon) * cos(radianLat2);
    double x = cos(radianLat1) * sin(radianLat2) -
               sin(radianLat1) * cos(radianLat2) * cos(radianDeltaLon);

    return (floor(this->toDegrees(std::atan2(y, x)) * 100 + 0.5) / 100);
}

