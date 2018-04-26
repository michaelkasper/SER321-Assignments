package Client;

import javax.swing.*;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;
import javax.swing.text.EditorKit;
import java.awt.event.*;
import java.io.*;
import java.net.Socket;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.util.Stack;
import java.util.regex.Pattern;

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
 * Purpose: Sample Java Swing controller class. BrowserGUI constructs the view components
 * for a simple broswer GUI. This class is extends the GUI to provide the controller.
 * It contains sample control functions that respond to button clicks and hotline clicks
 * <p>
 * This software is meant to run on Debian Wheezy Linux
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 *
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 * IAFSE, ASU at the Polytechnic campus
 * @file BrowserStudent.java
 * @date February, 2018
 **/
public class BrowserController extends BrowserGUI implements ActionListener, HyperlinkListener {

    private static final boolean debugOn = true;
    private final String host;
    private final int portNo;
    private final String protocol = "http";
    private Stack<String> history = new Stack<>();
    private static final String helpUrl = "app://help";
    private static final String homeUrl = "/index.html";


    public BrowserController(String host, int portNo) {
        super(".");
        this.host = host;
        this.portNo = portNo;

        WindowListener wl = new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }
        };
        this.addWindowListener(wl);
        displayButt.addActionListener(this);
        homeButt.addActionListener(this);
        backButt.addActionListener(this);
        init();
        htmlPane.setEditable(false);
        htmlPane.addHyperlinkListener(this);
    }

    public void hyperlinkUpdate(HyperlinkEvent e) {
        if (HyperlinkEvent.EventType.ACTIVATED.equals(e.getEventType())) {
//            debug("activated a hyperlink.");
            this.displayURL(e.getDescription());
            // Do something with e.getURL() here
        } else if (HyperlinkEvent.EventType.ENTERED.equals(e.getEventType())) {
//            debug("entered a hyperlink.");
            // Do something?
        } else if (HyperlinkEvent.EventType.EXITED.equals(e.getEventType())) {
//            debug("exited a hyperlink.");
            // Do something?
        }
    }


    public void actionPerformed(ActionEvent e) {
        try {
            if (e.getActionCommand().equals("Show/Refresh")) {
                goRefresh();
            } else if (e.getActionCommand().equals("Home")) {
                goHomeUrl();
            } else if (e.getActionCommand().equals("Back")) {
                goPreviousUrl();
            }
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this, "Exception: " + ex.getMessage());
            ex.printStackTrace();
        }
    }

    private void goPreviousUrl() {
        String prevUrl = helpUrl;
        if (!history.isEmpty()) {
            String currentUrl = history.pop();

            if (!history.isEmpty()) {
                prevUrl = history.pop();
            }
        }

        this.displayURL(prevUrl);
    }

    private void goHomeUrl() {
        this.displayURL(homeUrl);
    }


    private void goRefresh() {
        this.displayURL(history.peek());
    }


    private void goHelp() {
        String s = null;

        try {
            s = BrowserController.class.getResource("/www/help.html").toString();
            debug("Help URL is " + s);
            urlTF.setText(helpUrl);
            htmlPane.setPage(new URL(s));
            this.logHistory(helpUrl);
        } catch (Exception e) {
            System.err.println("Couldn't create help URL: " + s + " exception: "
                    + e.getMessage());
        }
    }


    private void logHistory(String urlStr) {
        if (history.isEmpty() || !history.peek().equals(urlStr)) {
            history.add(urlStr);
        }
    }

    private void init() {
        goHelp();
    }

    private void displayURL(String urlStr) {
        if (urlStr.equals(helpUrl)) {
            goHelp();
            return;
        }

        urlStr = cleanUrl(urlStr);

        urlTF.setText(urlStr);

        Page page = getContent(urlStr);
        htmlPane.setContentType(page.getContentType());

        if (page.getContentType().equals("application/rtf")) {
            EditorKit rtf = htmlPane.getEditorKitForContentType("application/rtf");
            try {
                InputStream stream = new ByteArrayInputStream(page.getContent().getBytes(StandardCharsets.UTF_8));
                rtf.read(stream, htmlPane.getDocument(), 0);
            } catch (Exception e) {
                errorPage(e.getMessage());
            }
        } else {
            htmlPane.setText(page.getContent());
        }

        this.logHistory(urlStr);
    }

    private Page getContent(String targetURL) {
        Socket conn;
        debug("GoTo: " + targetURL);

        try {
            conn = new Socket(host, portNo);
            OutputStream outSock = conn.getOutputStream();
            InputStream inSock = conn.getInputStream();


            PrintWriter out = new PrintWriter(outSock, true);
            out.println(targetURL);
            out.flush();

            String data;
            String contentType = "text/html";
            StringBuilder htmlPage = new StringBuilder();
            BufferedReader in = new BufferedReader(new InputStreamReader(inSock));

            int lineCount = 0;
            while ((data = in.readLine()) != null) {
                if (lineCount == 0) {
                    contentType = data;
                } else {
                    htmlPage.append(data);
                }
                lineCount++;
            }
            conn.close();

            return new Page(contentType, htmlPage.toString());
        } catch (Exception e) {
            e.printStackTrace();
            return errorPage(e.getMessage());
        }
    }

    private Page errorPage(String msg) {
        Page page = new Page("test/html", "<html><body><h1>Error</h1><h3>" + msg + "</h3></body></html>");

        htmlPane.setText(page.getContent());
        htmlPane.setContentType(page.getContentType());

        return page;
    }

    private String cleanUrl(String url) {
        String requestUrl = url;
        Pattern p;

        requestUrl = requestUrl.replaceAll("^(http[s]?:)?\\/\\/?" + host + "\\/", "");

        p = Pattern.compile("^((http[s]?|ftp):)?\\/\\/?([^:\\/\\s]+)\\.([^:\\/\\s]+)", Pattern.CASE_INSENSITIVE);
        if (p.matcher(requestUrl).find()) {
            return url;
        }
        requestUrl = trimSlash(requestUrl);

        p = Pattern.compile("(\\/[^\\.]*?)?$", Pattern.CASE_INSENSITIVE);
        if (!p.matcher(requestUrl).find()) {
            requestUrl = trimSlash(requestUrl);
            requestUrl = requestUrl + "/index.html";
        }

        if (url.charAt(0) == '.') {
            String bassUrl = history.peek().replaceAll("(\\/[^\\/]*\\.[^\\/]*)*$", "");
            bassUrl = trimSlash(bassUrl);

            requestUrl = requestUrl.replaceAll("^(\\.\\/)", "");
            requestUrl = bassUrl + "/" + requestUrl;
        }
        requestUrl = "/" + requestUrl;


        p = Pattern.compile("\\/[^\\/]*\\/\\.\\.\\/", Pattern.CASE_INSENSITIVE);
        while (p.matcher(requestUrl).find()) {
            requestUrl = requestUrl.replaceAll("\\/[^\\/]*\\/\\.\\.\\/", "/");
        }

        p = Pattern.compile("(^\\/\\/)", Pattern.CASE_INSENSITIVE);
        while (p.matcher(requestUrl).find()) {
            requestUrl = requestUrl.replaceAll("(^\\/\\/)", "/");
        }

        return requestUrl;
    }

    private String trimSlash(String stg) {
        return stg.replaceAll("(^\\/|\\/$)", "");
    }

    private void debug(String message) {
        if (debugOn)
            System.out.println("debug: " + message);
    }

    public static void main(String[] args) {
        System.out.println("syntax: java -cp classes ser321.http.client.BrowserStudent");
        if (args.length != 2) {
            System.out.println("Usage: java ser321.sockets.EchoClient hostName " +
                    "portNumber");
            System.exit(0);
        }
        new BrowserController(args[0], Integer.parseInt(args[1]));
    }
}
