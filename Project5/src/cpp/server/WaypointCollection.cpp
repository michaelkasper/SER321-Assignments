/**
 * Copyright 2016 Tim Lindquist,
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
 * Purpose: WaypointCollection is a class defining the interface between clients
 * and the server. The server implementation of WaypointCollection
 * provides storage for description of multiple waypoints
 *
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version July 2016
 */

#include "WaypointCollection.hpp"
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>

using namespace std;

WaypointCollection::WaypointCollection() {
    resetFromJsonFile("waypoints.json");
}

WaypointCollection::~WaypointCollection() {
    //cout << "waypoint collection destructor.\n";
    library.clear();
}

bool WaypointCollection::resetFromJsonFile(string jsonFileName) {
    bool ret = false;
    Json::Reader reader;
    Json::Value root;
    std::ifstream json(jsonFileName.c_str(), std::ifstream::binary);
    bool parseSuccess = reader.parse(json, root, false);
    if (parseSuccess) {
        library.clear();
        for (Json::Value &wpJson: root) {
            Waypoint wp(wpJson);
            library[wp.name] = wp;
        }
        ret = true;
    }
    return ret;
}

bool WaypointCollection::saveToJsonFile(string jsonFileName) {
    bool ret = false;
    Json::Value jsonLib;
    for (std::map<string, Waypoint>::iterator i = library.begin(); i != library.end(); i++) {
        string key = i->first;
        //cout << key << " " << endl;
        Waypoint aWaypoint = library[key];
        Json::Value jsonWaypoint = aWaypoint.toJson();
        jsonLib[key] = jsonWaypoint;
    }
    ret = true;
    Json::StyledStreamWriter ssw("  ");
    std::ofstream jsonOutFile(jsonFileName.c_str(), std::ofstream::binary);
    ssw.write(jsonOutFile, jsonLib);
    return ret;
}

bool WaypointCollection::add(const Json::Value &aWaypointJson) {
    bool ret = false;
    Waypoint aWaypoint(aWaypointJson);
    // The map container does not allow duplicates so adding will replace existing
    // value with new Waypoint instance.
    //aWaypoint.print();
    library[aWaypoint.name] = aWaypoint;
    return true;
}

bool WaypointCollection::remove(string aName) {
    bool ret = false;
    // does the library contain this waypoint? if so remove, if not return false
    if (library.count(aName) > 0) {
        library.erase(aName);
        ret = true;
    }
    return true;
}

Json::Value WaypointCollection::get(string aName) {
    if (library.count(aName) > 0) {
        return library[aName].toJson();
    }
    return Waypoint().toJson();
}

Json::Value WaypointCollection::getNames() {
    Json::Value ret(Json::arrayValue);
    vector<string> myVec;
    for (map<string, Waypoint>::iterator it = library.begin(); it != library.end(); ++it) {
        myVec.push_back(it->first);
    }
    for (std::vector<string>::iterator it = myVec.begin(); it != myVec.end(); ++it) {
        ret.append(Json::Value(*it));
    }
    return ret;
}
