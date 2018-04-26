#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Multiline_Input.H>
#include <iostream>

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
 * This class creates Gui components for a UI to manage waypoints.
 * This software is meant to run on MacOS, and Linux using g++ compiler.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering, IAFSE
 *                       ASU at the Polytechnic campus
 * @file    WaypointGUI.cpp
 * @date    January, 2018
 * @license See above
 **/
class WaypointGUI : public Fl_Window {
protected:
    const char *defaultFrom, *defaultTo;
    Fl_Input_Choice *frWps, *toWps;
    Fl_Input *latIn, *lonIn, *eleIn, *nameIn, *distBearIn;
    Fl_Multiline_Input *addrIn;
    Fl_Button *modWPButt, *removeWPButt, *addWPButt, *saveButt, *refreshButt,
            *distBearButt;

    /** ClickedX and ClickedSayHello are both callbacks.
     * Callbacks need to be static functions. But, static functions
     * cannot directly access instance data. This program uses "userdata"
     * to get around that problem by passing the instance to the callback
     * function.
     */
    static void ClickedX(Fl_Widget *w, void *userdata) {
        std::cout << "You clicked Exit" << std::endl;
        exit(1);
    }

public:
    explicit WaypointGUI(const char *name = nullptr) : Fl_Window(520, 300, name) {
        begin();

        defaultFrom = "from waypoint";
        defaultTo = "to waypoint";

        frWps = new Fl_Input_Choice(40, 10, 200, 25, "from");
        frWps->add(defaultFrom);
        frWps->value(0);
        toWps = new Fl_Input_Choice(40, 45, 200, 25, "to");
        toWps->add(defaultTo);
        toWps->value(0);
        latIn = new Fl_Input(270, 10, 230, 25, "lat");
        lonIn = new Fl_Input(270, 45, 230, 25, "lon");
        eleIn = new Fl_Input(270, 80, 230, 25, "ele");
        nameIn = new Fl_Input(270, 115, 230, 25, "name");
        addrIn = new Fl_Multiline_Input(270, 150, 230, 70, "addr");
        distBearIn = new Fl_Input(225, 260, 255, 25);

        removeWPButt = new Fl_Button(50, 80, 140, 25, "Remove Waypoint");
        addWPButt = new Fl_Button(55, 115, 130, 25, "Add Waypoint");
        modWPButt = new Fl_Button(50, 150, 140, 25, "Modify Waypoint");
        saveButt = new Fl_Button(50, 185, 140, 25, "Save Library");
        refreshButt = new Fl_Button(50, 220, 140, 25, "Reload Library");
        distBearButt = new Fl_Button(20, 260, 180, 25, "Distance and Bearing");

        end();
        show();
    }
};
