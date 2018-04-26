package Server;

import javax.activation.MimetypesFileTypeMap;
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URL;

/**
 * Copyright 2015 Tim Lindquist,
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
 * A class for client-server connections with a threaded server.
 * The echo server creates a server socket. Once a client arrives, a new
 * thread is created to service all client requests for the connection.
 * The example includes a java client and a C# client. If C# weren't involved,
 * the server and client could use a bufferedreader, which allows readln to be
 * used, and printwriter, which allows println to be used. These avoid
 * playing with byte arrays and encodings. See the Java Tutorial for an
 * example using buffered reader and printwriter.
 * <p>
 * Ser321 Foundations of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 *
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 * Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version August 2015
 */

/**
 * @author Tim Lindquist ASU Polytechnic Department of Engineering
 * @version October 2009
 */
public class WebServer extends Thread {
    private static final boolean debugOn = true;
    private Socket conn;
    private int id;

    public WebServer(Socket sock, int id) {
        this.conn = sock;
        this.id = id;
    }

    public void run() {
        try {
            OutputStream outSock = conn.getOutputStream();
            InputStream inSock = conn.getInputStream();

            BufferedReader in = new BufferedReader(new InputStreamReader(inSock));
            String data = in.readLine();
            data = data.replaceAll("^((http[s]?|ftp):)?\\/\\/?([^:\\/\\s]+)\\.([^:\\/\\s]+)", "");
            data = data.replaceAll("(^\\/|\\/$)", "");

            String requestedUrl;
            if (data.matches("(\\/[^\\.]*?)?$")) {
                requestedUrl = data + "/index.html";
            } else {
                requestedUrl = data;
            }
            requestedUrl = requestedUrl.replaceAll("(^\\/|\\/$)", "");

            URL fileUrl = WebServer.class.getResource("/www/Ser321/" + requestedUrl);
            if (fileUrl == null) {
                fileUrl = WebServer.class.getResource("/www/Ser321/404.html");
            }



            File file = new File(fileUrl.getFile());

            MimetypesFileTypeMap fileTypeMap = new MimetypesFileTypeMap();

            PrintWriter output = new PrintWriter(outSock);
            output.println("Content-Type: "+fileTypeMap.getContentType(file));
            output.flush();


            byte[] bytes = new byte[16 * 1024];
            InputStream fileReader = new FileInputStream(file);

            int count;
            while ((count = fileReader.read(bytes)) > 0) {
                outSock.write(bytes, 0, count);
            }


            conn.close();
        } catch (IOException e) {
            System.out.println("Can't get I/O for the connection.");
        } catch (Exception e) {
            e.printStackTrace();
            debug("94 " + e.getMessage());
        }
    }

    private static void debug(String message) {
        if (debugOn)
            System.out.println("debug: " + message);
    }

    public static void main(String args[]) {
        Socket sock;
        int id = 0;
        try {
            if (args.length != 1) {
                System.out.println("Usage: java Server.WebServer" + " [portNum]");
                System.exit(0);
            }
            int portNo = Integer.parseInt(args[0]);
            if (portNo <= 1024) portNo = 8888;
            ServerSocket serv = new ServerSocket(portNo);
            while (true) {
                System.out.println("Echo server waiting for connects on port " + portNo);
                sock = serv.accept();

                System.out.println("Echo server connected to client: " + id);
                WebServer myServerThread = new WebServer(sock, id++);

                myServerThread.start();
            }
        } catch (Exception e) {
            e.printStackTrace();
            debug("125 " + e.getMessage());
        }
    }
}
