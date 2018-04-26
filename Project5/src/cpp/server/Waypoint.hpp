#include <string>
#include <cmath>
#include <jsoncpp/json/value.h>

using namespace std;

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
class Waypoint {
protected:
    static constexpr double pi = 3.14159265;

    double toRadians(double deg) {
        return (deg * pi) / 180.0;
    }

    double toDegrees(double rad) {
        return (rad * 180.0) / pi;
    }

private:
    string toStr(double theDouble);

public:
    static const int STATUTE = 0;
    static const int NAUTICAL = 1;
    static const int KMETER = 2;
    static const int radiusE = 6371;

    double lat;
    double lon;
    double ele;
    string name;
    string addr;

    Waypoint();

    Waypoint(double aLat, double aLon, double anElevation, string aName, string addr);

    Waypoint(const Json::Value &value);

    ~Waypoint();

    void setValues(double aLat, double aLon, double anElevation, string aName, string aAddr);

    string getLatStr();

    string getLonStr();

    string getEleStr();

    void print();

    Json::Value toJson();

    double bearingGCInitTo(Waypoint *wp, int scale);

    double distanceGCTo(Waypoint *wp, int scale);
};
