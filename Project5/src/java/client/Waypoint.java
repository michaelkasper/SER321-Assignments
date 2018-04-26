import org.json.JSONObject;
import org.json.JSONString;

/**
 * Copyright 2018 Tim Lindquist,
 * <p>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * <p>
 * Class to perform various earth surface calculations. Given lat/lon points
 * this class can perform distance and bearing calculations - Great Circle,
 * and Rhumb-line.
 * <p>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 *
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 * Software Engineering, CIDSE, IAFSE, Arizona State University Polytechnic
 * @version January, 2018
 **/

public class Waypoint extends Object implements JSONString, Comparable {

    public final static int STATUTE = 0;
    public final static int NAUTICAL = 1;
    public final static int KMETER = 2;
    public final static double radiusE = 6371;

    public double lat; // degrees lat in DD.D format (+ north, - south)
    public double lon; // degrees lon in DD.D format (+ east, - west)
    public double ele; // elevation in feet MSL
    public String name; // a name for the waypoint
    public String address; // a name for the waypoint

    public Waypoint() {
        this.lat = 0;
        this.lon = 0;
        this.ele = 0;
        this.name = "Unknown";
    }

    public Waypoint(double lat, double lon, double ele, String name, String address) {
        this.lat = lat;
        this.lon = lon;
        this.ele = ele;
        this.name = name;
        this.address = address;
    }

    public Waypoint(String latIn, String lonIn, String eleIn, String nameIn, String address) {
        this(Double.parseDouble(latIn), Double.parseDouble(lonIn), Double.parseDouble(eleIn), nameIn.trim(), address.trim());
    }

    public Waypoint(JSONObject obj) {
        this.lat = obj.getDouble("lat");
        this.lon = obj.getDouble("lon");
        this.ele = obj.getDouble("ele");
        this.name = obj.getString("name");
        this.address = obj.getString("addr");
    }

    /**
     * calculate the distance great circle route between this and the
     * argument waypoints. Return the result in the requested scale
     */
    public double distanceGCTo(Waypoint wp, int scale) {
        double lat1 = this.lat;
        double lon1 = this.lon;

        double lat2 = wp.lat;
        double lon2 = wp.lon;

        double radianLat1 = Math.toRadians(lat1);
        double radianLat2 = Math.toRadians(lat2);
        double radianDeltaLat = Math.toRadians(lat2 - lat1);
        double radianDeltaLon = Math.toRadians(lon2 - lon1);

        double a = Math.sin(radianDeltaLat / 2) * Math.sin(radianDeltaLat / 2) + Math.cos(radianLat1) * Math.cos(radianLat2) * Math.sin(radianDeltaLon / 2) * Math.sin(radianDeltaLon / 2);
        double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));

        double ret = (Waypoint.radiusE * c);

        switch (scale) {
            case STATUTE:
                ret = ret * 0.62137119;
                break;
            case NAUTICAL:
                ret = ret * 0.5399568;
                break;
        }
        return ret;
    }

    /**
     * calculate the initial heading on the circle route between this and the
     * argument waypoints. Return the result.
     */
    public double bearingGCInitTo(Waypoint wp, int scale) {

        double lat1 = this.lat;
        double lon1 = this.lon;

        double lat2 = wp.lat;
        double lon2 = wp.lon;

        double radianLat1 = Math.toRadians(lat1);
        double radianLat2 = Math.toRadians(lat2);
        double radianDeltaLon = Math.toRadians(lon2 - lon1);

        double y = Math.sin(radianDeltaLon) * Math.cos(radianLat2);
        double x = Math.cos(radianLat1) * Math.sin(radianLat2) -
                Math.sin(radianLat1) * Math.cos(radianLat2) * Math.cos(radianDeltaLon);

        return Math.toDegrees(Math.atan2(y, x));
    }

    public void print() {
        System.out.println("Waypoint " + name + ": lat " + lat + " lon " + lon +
                " elevation " + ele);
    }

    public String toJSONString() {
        return this.toJson().toString();
    }

    @Override
    public int compareTo(Object o) {
        return this.name.compareTo(((Waypoint) o).name);
    }

    public JSONObject toJson() {
        JSONObject obj = new JSONObject();
        obj.put("lat", this.lat);
        obj.put("lon", this.lon);
        obj.put("ele", this.ele);
        obj.put("name", this.name);
        obj.put("addr", this.address);
        return obj;
    }

    @Override
    public String toString() {
        return "Waypoint{" +
                "lat=" + lat +
                ", lon=" + lon +
                ", ele=" + ele +
                ", name='" + name + '\'' +
                ", addr='" + address + '\'' +
                '}';
    }
}
