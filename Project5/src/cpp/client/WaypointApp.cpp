#include "WaypointGUI.cpp"
#include "../server/Waypoint.hpp"

#include <algorithm>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <sstream>
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <json/json.h>
#include "waypointcollectionstub.h"

using namespace jsonrpc;
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

    static void ClickedRemoveAction(Fl_Widget *w, void *userdata) {
        auto *anInstance = (WaypointApp *) userdata;

        if (anInstance->hasSelectedWaypoint()) {
            const string selected(anInstance->selectedWaypoint->name);

            anInstance->removeWaypoint(selected);
            std::cout << "You clicked the remove waypoint button with "
                      << selected
                      << std::endl;

        }
    }

    static void ClickedAddAction(Fl_Widget *w, void *userdata) {
        auto *anInstance = (WaypointApp *) userdata;

        std::string lat(anInstance->latIn->value());
        std::string lon(anInstance->lonIn->value());
        std::string ele(anInstance->eleIn->value());
        std::string name(anInstance->nameIn->value());
        std::string addr(anInstance->addrIn->value());

        auto *wp = new Waypoint(atof(lat.c_str()), atof(lon.c_str()), atof(ele.c_str()), name, addr);

        anInstance->addWaypoint(wp);

        std::cout << "You clicked the add waypoint button lat: " << wp->getLatStr()
                  << " lon: " << wp->getLonStr() << " ele: " << wp->getEleStr() << " name: "
                  << wp->name << std::endl;
    }

    static void SelectedFromAction(Fl_Widget *w, void *userdata) {
        auto *anInstance = (WaypointApp *) userdata;

        if (strcmp(anInstance->frWps->value(), anInstance->defaultFrom) == 0) {
            anInstance->fromWaypoint = nullptr;
            anInstance->selectedWaypoint = nullptr;
            anInstance->clearInputs();
        } else {
            Waypoint *wp = anInstance->getWaypoint(anInstance->frWps->value());
            anInstance->setFromWaypoint(wp);

            std::cout << "You changed the selection in the Fl_Input_Choice 'from' to the value "
                      << anInstance->frWps->value()
                      << std::endl;
        }
    }

    static void SelectedToAction(Fl_Widget *w, void *userdata) {
        auto *anInstance = (WaypointApp *) userdata;

        if (strcmp(anInstance->toWps->value(), anInstance->defaultTo) == 0) {
            anInstance->toWaypoint = nullptr;
            anInstance->selectedWaypoint = nullptr;
            anInstance->clearInputs();
        } else {
            Waypoint *wp = anInstance->getWaypoint(anInstance->toWps->value());
            anInstance->setToWaypoint(wp);

            std::cout << "You changed the selection in the Fl_Input_Choice 'to' to the value "
                      << anInstance->toWps->value()
                      << std::endl;
        }


    }

    static void ClickedModifyAction(Fl_Widget *w, void *userdata) {
        auto *anInstance = (WaypointApp *) userdata;

        std::string lat(anInstance->latIn->value());
        std::string lon(anInstance->lonIn->value());
        std::string ele(anInstance->eleIn->value());
        std::string name(anInstance->nameIn->value());
        std::string addr(anInstance->addrIn->value());

        auto *wp = new Waypoint(atof(lat.c_str()), atof(lon.c_str()), atof(ele.c_str()), name, addr);

        if (anInstance->hasSelectedWaypoint()) {
            anInstance->modifyWaypoint(anInstance->selectedWaypoint, wp);
            std::cout << "You clicked modify waypoint button"
                      << std::endl;
        }


    }

    static void ClickedCalculatDistAction(Fl_Widget *w, void *userdata) {
        auto *anInstance = (WaypointApp *) userdata;

        anInstance->calculatDist();

        std::cout << "You clicked distance and bearing"
                  << std::endl;
    }

    static void ClickedSaveLibraryAction(Fl_Widget *w, void *userdata) {
        auto *anInstance = (WaypointApp *) userdata;

        anInstance->saveCollection();
        std::cout << "You clicked save library"
                  << std::endl;
    }


    static void ClickedRefreshLibraryAction(Fl_Widget *w, void *userdata) {
        auto *anInstance = (WaypointApp *) userdata;

        anInstance->reloadCollection();
        std::cout << "You clicked refresh library"
                  << std::endl;
    }

private:
    waypointcollectionstub *remoteCollection = nullptr;
    Waypoint *selectedWaypoint = nullptr;
    Waypoint *fromWaypoint = nullptr;
    Waypoint *toWaypoint = nullptr;

    void reset() {
        this->frWps->clear();
        this->toWps->clear();

        this->frWps->add(defaultFrom);
        this->toWps->add(defaultTo);

        this->frWps->value(0);
        this->toWps->value(0);

        this->toWaypoint = nullptr;
        this->fromWaypoint = nullptr;
        this->selectedWaypoint = nullptr;

        this->clearInputs();
    }

    void clearInputs() {
        this->latIn->value("");
        this->lonIn->value("");
        this->eleIn->value("");
        this->nameIn->value("");
        this->addrIn->value("");
    }

    void refresh() {
        Waypoint *fromWaypoint = this->fromWaypoint;
        Waypoint *toWaypoint = this->toWaypoint;
        Waypoint *selectedWaypoint = this->selectedWaypoint;

        this->reset();

        this->fromWaypoint = fromWaypoint;
        this->toWaypoint = toWaypoint;
        this->selectedWaypoint = selectedWaypoint;

        Json::Value result = this->remoteCollection->getNames();

        vector<string> menuItems;
        for (int i = 0; i < result.size(); i++) {
            menuItems.emplace_back(result[i].asString().c_str());
        }

        sort(menuItems.begin(), menuItems.end());

        for (string menuItem : menuItems) {
            this->frWps->add(menuItem.c_str());
            this->toWps->add(menuItem.c_str());
        }

        if (this->hasToWaypoint()) {
            this->toWps->value(this->toWaypoint->name.c_str());
        }

        if (this->hasFromWaypoint()) {
            this->frWps->value(this->fromWaypoint->name.c_str());
        }

        if (this->hasSelectedWaypoint()) {
            this->showWaypoint(this->selectedWaypoint);
        }
    }

    void removeWaypoint(const string &key) {
        if (this->remoteCollection->remove(key)) {
            if (this->fromWaypoint == this->selectedWaypoint) {
                this->fromWaypoint = nullptr;
            } else if (this->toWaypoint == this->selectedWaypoint) {
                this->toWaypoint = nullptr;
            }

            this->selectedWaypoint = nullptr;
            this->refresh();
        }
    }


    void modifyWaypoint(Waypoint *oldWp, Waypoint *newWp) {
        if (this->fromWaypoint == oldWp) {
            this->fromWaypoint = newWp;
        } else if (this->toWaypoint == oldWp) {
            this->toWaypoint = newWp;
        }

        this->remoteCollection->remove(oldWp->name);
        this->remoteCollection->add(newWp->toJson());
        this->selectedWaypoint = newWp;
        this->refresh();
    }


    void addWaypoint(Waypoint *wp) {
        if (this->fromWaypoint == this->selectedWaypoint) {
            this->fromWaypoint = wp;
        } else if (this->toWaypoint == this->selectedWaypoint) {
            this->toWaypoint = wp;
        }

        this->remoteCollection->remove(wp->name);
        this->remoteCollection->add(wp->toJson());
        this->selectedWaypoint = wp;
        this->refresh();
    }

    void setToWaypoint(Waypoint *wp) {
        this->selectedWaypoint = wp;
        this->toWaypoint = wp;
        this->showWaypoint(this->toWaypoint);
    }

    void setFromWaypoint(Waypoint *wp) {
        this->selectedWaypoint = wp;
        this->fromWaypoint = wp;
        this->showWaypoint(this->fromWaypoint);
    }

    Waypoint *getWaypoint(const string &wpName) {
        Json::Value json = this->remoteCollection->get(wpName);
        auto *wp = new Waypoint(json);
        return wp;
    }


    void showWaypoint(Waypoint *wp) {
        this->latIn->value(wp->getLatStr().c_str());
        this->lonIn->value(wp->getLonStr().c_str());
        this->eleIn->value(wp->getEleStr().c_str());
        this->nameIn->value(wp->name.c_str());
        this->addrIn->value(wp->addr.c_str());
    }

    void reloadCollection() {
        this->remoteCollection->resetFromJsonFile();
        this->refresh();
    }

    void saveCollection() {
        this->remoteCollection->saveToJsonFile();
    }

    void calculatDist() {
        Waypoint *frwp = nullptr;
        Waypoint *towp = nullptr;

        if (this->hasFromWaypoint()) {
            frwp = this->fromWaypoint;
        }

        if (this->hasToWaypoint()) {
            towp = this->toWaypoint;
        }

        if (frwp != nullptr || towp != nullptr) {
            if (!(frwp != nullptr && towp != nullptr)) {
                frwp = towp = (frwp != nullptr ? frwp : towp);
            }

            std::ostringstream message;

            message << frwp->distanceGCTo(towp, Waypoint::KMETER)
                    << " km at "
                    << frwp->bearingGCInitTo(towp, Waypoint::KMETER)
                    << " degrees ";

            this->distBearIn->value(message.str().c_str());
        }
    }

    bool hasSelectedWaypoint() {
        return this->selectedWaypoint != nullptr;
    }

    bool hasFromWaypoint() {
        return this->fromWaypoint != nullptr;
    }

    bool hasToWaypoint() {
        return this->toWaypoint != nullptr;
    }


public:
    WaypointApp(const char *name, waypointcollectionstub *collection) : WaypointGUI(name) {
        remoteCollection = collection;
        removeWPButt->callback(ClickedRemoveAction, (void *) this);
        addWPButt->callback(ClickedAddAction, (void *) this);
        frWps->callback(SelectedFromAction, (void *) this);
        toWps->callback(SelectedToAction, (void *) this);
        distBearButt->callback(ClickedCalculatDistAction, (void *) this);
        modWPButt->callback(ClickedModifyAction, (void *) this);
        saveButt->callback(ClickedSaveLibraryAction, (void *) this);
        refreshButt->callback(ClickedRefreshLibraryAction, (void *) this);
        callback(ClickedX);
        this->refresh();
    }
};

int main(int argc, char *argv[]) {

    string host = "http://127.0.0.1:8080";
    if (argc > 1) {
        host = string(argv[1]);
    }
    HttpClient httpclient(host);
    waypointcollectionstub collection(httpclient);
    WaypointApp cm("Michael Kasper's Waypoint Browser", &collection);
    return (Fl::run());
}
