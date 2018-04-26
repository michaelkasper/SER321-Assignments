#include "WaypointGUI.cpp"
#include "Waypoint.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Multiline_Input.H>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <map>
#include <algorithm>
#include <vector>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <sstream>


using namespace std;

/**
 * Copyright (c) 2018 Tim Lindquist,
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
 * Purpose: C++ FLTK client UI for Waypoint management.
 * This class extends the Gui component class WaypointGUI and demonstrates
 * sample control functions that respond to button clicks drop-down selects.
 * This software is meant to run on Linux and MacOS using g++.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * @file    samplemain.cpp
 * @date    January, 2018
 **/
class WaypointApp : public WaypointGUI {

    /** ClickedX is one of the callbacks for GUI controls.
     * Callbacks need to be static functions. But, static functions
     * cannot directly access instance data. This program uses "userdata"
     * to get around that by passing the instance to the callback
     * function. The callback then accesses whatever GUI control object
     * that it needs for implementing its functionality.
     */
    static void ClickedX(Fl_Widget *w, void *userdata) {
        std::cout << "You clicked Exit" << std::endl;
        exit(1);
    }

    static void ClickedRemoveWP(Fl_Widget *w, void *userdata) {
        auto *anInstance = (WaypointApp *) userdata;
        std::string selected(anInstance->frWps->value());

        if (anInstance->removeWaypoint(&selected)) {
            std::cout << "You clicked the remove waypoint button with "
                      << selected
                      << std::endl;
        }
    }

    static void ClickedAddWP(Fl_Widget *w, void *userdata) {
        auto *anInstance = (WaypointApp *) userdata;

        std::string lat(anInstance->latIn->value());
        std::string lon(anInstance->lonIn->value());
        std::string ele(anInstance->eleIn->value());
        std::string name(anInstance->nameIn->value());
        std::string addr(anInstance->addrIn->value());

        Waypoint wp(atof(lat.c_str()), atof(lon.c_str()), atof(ele.c_str()), name, addr);

        anInstance->addWaypoint(wp);

        std::cout << "You clicked the add waypoint button lat: " << wp.getLatStr()
                  << " lon: " << wp.getLonStr() << " ele: " << wp.getEleStr() << " name: "
                  << wp.name << std::endl;

        Fl_Input_Choice *fromWPChoice = anInstance->frWps;
        fromWPChoice->value(wp.name.c_str());
    }

    static void SelectedFromWP(Fl_Widget *w, void *userdata) {
        auto *anInstance = (WaypointApp *) userdata;
        std::string selected(anInstance->frWps->value());

        anInstance->showWaypoint(&selected);

        std::cout << "You changed the selection in the Fl_Input_Choice 'from' to the value "
                  << selected
                  << std::endl;
    }

    static void SelectedToWP(Fl_Widget *w, void *userdata) {
        WaypointApp *anInstance = (WaypointApp *) userdata;
        std::string selected(anInstance->toWps->value());

        anInstance->showWaypoint(&selected);

        std::cout << "You changed the selection in the Fl_Input_Choice 'to' to the value "
                  << selected
                  << std::endl;

    }

    static void ModifyWP(Fl_Widget *w, void *userdata) {
        WaypointApp *anInstance = (WaypointApp *) userdata;

        Fl_Input_Choice *fromWPChoice = anInstance->frWps;
        Fl_Input_Choice *toWPChoice = anInstance->toWps;

        std::string oldFromSelected(fromWPChoice->value());
        std::string oldToSelected(toWPChoice->value());
        std::string oldSelectedKey(anInstance->selectedKey);

        if (anInstance->removeWaypoint(&oldSelectedKey)) {
            Waypoint wp(anInstance->latIn, anInstance->lonIn, anInstance->eleIn, anInstance->nameIn,
                        anInstance->addrIn);

            anInstance->addWaypoint(wp);

            if (!oldSelectedKey.compare(oldFromSelected)) {
                fromWPChoice->value(wp.name.c_str());
            }

            if (!oldSelectedKey.compare(oldToSelected)) {
                toWPChoice->value(wp.name.c_str());
            }

            std::cout << "You clicked modify waypoint button:"
                      << oldSelectedKey << std::endl;
        }
    }

    static void CalculatDist(Fl_Widget *w, void *userdata) {
        WaypointApp *anInstance = (WaypointApp *) userdata;

        Fl_Input_Choice *fromWPChoice = anInstance->frWps;
        Fl_Input_Choice *toWPChoice = anInstance->toWps;

        std::string fromSelected(fromWPChoice->value());
        std::string toSelected(toWPChoice->value());

        std::map<std::string, Waypoint>::iterator it;

        Waypoint *frwp;
        Waypoint *towp;

        bool hasFrom = false;
        bool hasTo = false;

        it = anInstance->waypoints.find(fromSelected);
        if (it != anInstance->waypoints.end()) {
            frwp = &it->second;
            hasFrom = true;
        }

        it = anInstance->waypoints.find(toSelected);
        if (it != anInstance->waypoints.end()) {
            towp = &it->second;
            hasTo = true;
        }

        if (hasFrom || hasTo) {
            if (!(hasFrom && hasTo)) {
                frwp = towp = (hasFrom ? frwp : towp);
            }

            std::ostringstream message;

            message << frwp->distanceGCTo(towp, Waypoint::KMETER)
                    << " km at "
                    << frwp->bearingGCInitTo(towp, Waypoint::KMETER)
                    << " degrees ";

            anInstance->distBearIn->value(message.str().c_str());
        }

        std::cout << "You clicked distance and bearing"
                  << std::endl;
    }

    static void SaveLibrary(Fl_Widget *w, void *userdata) {
        WaypointApp *anInstance = (WaypointApp *) userdata;

        Json::Value root;

        std::map<std::string, Waypoint>::iterator it = anInstance->waypoints.begin();
        while (it != anInstance->waypoints.end()) {
            std::string key = it->first;
            Waypoint wp = it->second;

            root[key] = wp.toJson();
            it++;
        }

        std::ofstream fout("waypoint.json");
        Json::StreamWriterBuilder styledWriter;
        styledWriter.newStreamWriter()->write(root, &fout);

        std::cout << "You clicked save library"
                  << std::endl;
    }


    static void RefreshLibrary(Fl_Widget *w, void *userdata) {
        WaypointApp *anInstance = (WaypointApp *) userdata;

        anInstance->reset();

        Json::Value root;
        std::ifstream file("waypoint.json", std::ifstream::binary);
        file >> root;

        for (Json::Value &wpJson: root) {
            Waypoint wp(wpJson);
            anInstance->addWaypoint(wp);
        }

        std::cout << "You clicked refresh library"
                  << std::endl;
    }

private:
    string selectedKey;
    std::map<std::string, Waypoint> waypoints;

    void reset() {
        string defaultFrom = this->frWps->menubutton()->menu()[0].label();
        string defaultTo = this->toWps->menubutton()->menu()[0].label();
        this->frWps->clear();
        this->toWps->clear();

        this->frWps->add(defaultFrom.c_str());
        this->toWps->add(defaultTo.c_str());

        this->frWps->value(0);
        this->toWps->value(0);

        this->latIn->value("");
        this->lonIn->value("");
        this->eleIn->value("");
        this->nameIn->value("");
        this->addrIn->value("");
        this->selectedKey.clear();

        this->waypoints.clear();
    }

    void showWaypoint(string *key) {
        auto it = this->waypoints.find(*key);

        if (it != this->waypoints.end()) {
            Waypoint *selectedWp = &it->second;
            this->latIn->value(selectedWp->getLatStr().c_str());
            this->lonIn->value(selectedWp->getLonStr().c_str());
            this->eleIn->value(selectedWp->getEleStr().c_str());
            this->nameIn->value(selectedWp->name.c_str());
            this->addrIn->value(selectedWp->addr.c_str());
            this->selectedKey = *key;
        } else {
            this->latIn->value("");
            this->lonIn->value("");
            this->eleIn->value("");
            this->nameIn->value("");
            this->addrIn->value("");
            this->selectedKey.clear();
        }
    }

    bool removeWaypoint(string *key) {
        std::map<std::string, Waypoint>::iterator it = this->waypoints.find(*key);
        if (it != this->waypoints.end()) {
            Fl_Input_Choice *menus[2] = {this->frWps, this->toWps};
            for (Fl_Input_Choice *menu : menus) {
                std::string selected(menu->value());

                for (int i = 1; i < menu->menubutton()->size() - 1; i++) {
                    const Fl_Menu_Item &item = menu->menubutton()->menu()[i];
                    if (!key->compare(item.label())) {  // if they are equal
                        menu->menubutton()->remove(i);

                        if (!selected.compare(*key)) {
                            menu->value(0);
                        }
                        break;
                    }
                }
            }

            this->waypoints.erase(*key);
            this->selectedKey.clear();
            return true;
        }

        return false;
    }


    Waypoint addWaypoint(Waypoint wp) {
        Fl_Input_Choice *fromWPChoice = this->frWps;
        Fl_Input_Choice *toWPChoice = this->toWps;

        std::map<std::string, Waypoint>::iterator it = this->waypoints.find(wp.name);
        if (it != this->waypoints.end()) {
            this->removeWaypoint(&wp.name);
        }

        this->waypoints.insert(std::map<std::string, Waypoint>::value_type(wp.name, wp));

        Fl_Input_Choice *menus[2] = {this->frWps, this->toWps};
        for (Fl_Input_Choice *menu : menus) {

            string currentValue = menu->value();
            menu->add(wp.name.c_str());

            const Fl_Menu_Item *menuList = menu->menubutton()->menu();
            int itemCount = menuList->size() - 1;

            vector<string> menuItems;

            for (int i = 1; i < itemCount; i++) {
                menuItems.emplace_back(menuList[i].label());
            }

            sort(menuItems.begin(), menuItems.end());

            for (int i = 0; i < menuItems.size(); i++) {
                menu->menubutton()->replace(i + 1, menuItems[i].c_str());
            }
            menu->value(currentValue.c_str());
        }

        this->selectedKey = wp.name;
        return wp;
    }


public:
    WaypointApp(const char *name = 0) : WaypointGUI(name) {
        removeWPButt->callback(ClickedRemoveWP, (void *) this);
        addWPButt->callback(ClickedAddWP, (void *) this);
        frWps->callback(SelectedFromWP, (void *) this);
        toWps->callback(SelectedToWP, (void *) this);
        distBearButt->callback(CalculatDist, (void *) this);
        modWPButt->callback(ModifyWP, (void *) this);
        saveButt->callback(SaveLibrary, (void *) this);
        refreshButt->callback(RefreshLibrary, (void *) this);
        callback(ClickedX);
    }
};

int main() {
    WaypointApp cm("Tim Lindquist's Waypoint Browser");
    return (Fl::run());
}
