import java.io.*;
import java.net.*;
import java.util.*;
import java.time.*;
import javax.swing.*;

public class Client {
    private static final String SERVER_IP = "localhost";
    private static final int SERVER_PORT = 12345;
    private static final int FILE_PORT = 12346;
    private static String username;
    private static PrintWriter out;
    private static JFrame frame;

    public static void main(String[] args) {
        initializeGUI();
        connectToServer();
    }

    private static void initializeGUI() {
        frame = new JFrame("Chat Client");
        // Add GUI components here
    }

    private static void connectToServer() {
        try (Socket socket = new Socket(SERVER_IP, SERVER_PORT);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true)) {
            
            Client.out = out;
            handleAuthentication(in);
            
            new Thread(() -> {
                try {
                    String message;
                    while ((message = in.readLine()) != null) {
                        displayMessage(message);
                    }
                } catch (IOException e) {
                    JOptionPane.showMessageDialog(frame, "Connection lost!");
                }
            }).start();

            handleUserInput();
        } catch (IOException e) {
            JOptionPane.showMessageDialog(frame, "Connection error: " + e.getMessage());
        }
    }

    private static void handleAuthentication(BufferedReader in) throws IOException {
        while (true) {
            String response = in.readLine();
            if (response.startsWith("REGISTRATION_SUCCESS") || 
                response.startsWith("LOGIN_SUCCESS")) break;
            // Show authentication dialog
        }
    }

    private static void handleUserInput() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            String input = scanner.nextLine();
            if (input.startsWith("/sendfile")) {
                handleFileTransfer(input);
            } else {
                out.println(input);
            }
        }
    }

    private static void handleFileTransfer(String command) {
        String[] parts = command.split(" ", 3);
        new FileTransferClient(parts[1], parts[2]).start();
    }

    private static void displayMessage(String message) {
        // Update GUI or console output with timestamp
    }
}

class FileTransferClient extends Thread {
    private String recipient;
    private String filePath;

    public FileTransferClient(String recipient, String filePath) {
        this.recipient = recipient;
        this.filePath = filePath;
    }

    public void run() {
        try (Socket socket = new Socket(Client.SERVER_IP, Client.FILE_PORT);
             ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
             FileInputStream fis = new FileInputStream(filePath)) {
            
            File file = new File(filePath);
            Server.FileTransferRequest request = new Server.FileTransferRequest();
            request.sender = Client.username;
            request.recipient = recipient;
            request.fileName = file.getName();
            request.fileSize = file.length();
            
            oos.writeObject(request);
            String response = (String) new ObjectInputStream(socket.getInputStream()).readObject();
            
            if ("READY".equals(response)) {
                byte[] buffer = new byte[4096];
                int count;
                while ((count = fis.read(buffer)) > 0) {
                    oos.write(buffer, 0, count);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}