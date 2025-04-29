import java.io.*;
import java.net.*;
import java.util.*;
import java.time.*;
import java.security.*;
import java.util.concurrent.*;
import java.time.format.DateTimeFormatter;

public class Server {
    private static final int PORT = 12345;
    private static final int FILE_PORT = 12346;
    private static final Map<String, User> users = new ConcurrentHashMap<>();
    private static final Map<String, ChatRoom> rooms = new ConcurrentHashMap<>();
    private static final Map<String, String> credentials = new ConcurrentHashMap<>();

    public static void main(String[] args) {
        rooms.put("general", new ChatRoom("general"));
        System.out.println("Chat Server is running...");
        
        // File transfer server
        new Thread(() -> {
            try (ServerSocket fileServer = new ServerSocket(FILE_PORT)) {
                while (true) {
                    Socket fileSocket = fileServer.accept();
                    new FileTransferHandler(fileSocket).start();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();

        // Main chat server
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            while (true) {
                Socket clientSocket = serverSocket.accept();
                new ClientHandler(clientSocket).start();
            }
        } catch (IOException e) {
            System.err.println("Server exception: " + e.getMessage());
        }
    }

    // File transfer handler
    static class FileTransferHandler extends Thread {
        private Socket socket;
        
        public FileTransferHandler(Socket socket) {
            this.socket = socket;
        }
        
        public void run() {
            try (ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
                 ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream())) {
                
                FileTransferRequest request = (FileTransferRequest) ois.readObject();
                User recipient = users.get(request.recipient);
                
                if (recipient != null) {
                    oos.writeObject("READY");
                    forwardFile(request, recipient.getOutStream());
                } else {
                    oos.writeObject("USER_NOT_FOUND");
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        private void forwardFile(FileTransferRequest request, PrintWriter out) throws IOException {
            try (BufferedInputStream fileIn = new BufferedInputStream(new FileInputStream(request.fileName))) {
                byte[] buffer = new byte[4096];
                int bytesRead;
                while ((bytesRead = fileIn.read(buffer)) != -1) {
                    out.write(new String(buffer, 0, bytesRead));
                    out.flush();
                }
                out.println("File transfer completed.");
            }
        }
    }

    // Client handler for managing client connections
    static class ClientHandler extends Thread {
        private Socket socket;
        private User user;
        private PrintWriter out;
        private BufferedReader in;

        public ClientHandler(Socket socket) {
            this.socket = socket;
        }

        public void run() {
            try {
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                out = new PrintWriter(socket.getOutputStream(), true);

                // Authentication
                while (true) {
                    String authCommand = in.readLine();
                    if (authCommand.startsWith("/register")) {
                        handleRegistration(authCommand);
                    } else if (authCommand.startsWith("/login")) {
                        if (handleLogin(authCommand)) break;
                    }
                }

                // Main chat loop
                String input;
                while ((input = in.readLine()) != null) {
                    if (input.startsWith("/")) {
                        handleCommand(input);
                    } else {
                        broadcastMessage(user.getRoom(), formatMessage(input));
                    }
                }
            } catch (IOException e) {
                System.err.println("Client handler error: " + e.getMessage());
            } finally {
                cleanup();
            }
        }

        private void handleCommand(String command) throws IOException {
            String[] parts = command.split(" ", 3);
            switch (parts[0]) {
                case "/pm":
                    sendPrivateMessage(parts[1], parts[2]);
                    break;
                case "/join":
                    joinRoom(parts[1]);
                    break;
                case "/sendfile":
                    initiateFileTransfer(parts[1], parts[2]);
                    break;
                // Add more commands here as needed
            }
        }

        private void sendPrivateMessage(String recipient, String message) {
            User target = users.get(recipient);
            if (target != null) {
                target.send(formatPrivateMessage(message));
            }
        }

        private void joinRoom(String roomName) {
            rooms.computeIfAbsent(roomName, k -> new ChatRoom(roomName));
            user.setRoom(roomName);
            rooms.get(roomName).addUser(user);
        }

        private void initiateFileTransfer(String recipient, String filePath) {
            new FileTransferClient(user, users.get(recipient), filePath).start();
        }

        private String formatMessage(String text) {
            return "[" + LocalDateTime.now().format(DateTimeFormatter.ISO_LOCAL_TIME) + "] " + 
                   user.getUsername() + ": " + text;
        }

        private String formatPrivateMessage(String text) {
            return "(PM) " + formatMessage(text);
        }

        private void broadcastMessage(String room, String message) {
            rooms.get(room).broadcast(message);
        }

        private void cleanup() {
            if (user != null) {
                rooms.get(user.getRoom()).removeUser(user);
                users.remove(user.getUsername());
            }
            try {
                if (socket != null && !socket.isClosed()) {
                    socket.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        // Authentication helpers
        private void handleRegistration(String command) {
            String[] parts = command.split(" ", 3);
            if (parts.length == 3) {
                credentials.put(parts[1], hashPassword(parts[2]));
                out.println("REGISTRATION_SUCCESS");
            }
        }

        private boolean handleLogin(String command) {
            String[] parts = command.split(" ", 3);
            if (credentials.containsKey(parts[1]) && 
                credentials.get(parts[1]).equals(hashPassword(parts[2]))) {
                user = new User(parts[1], out);
                users.put(parts[1], user);
                joinRoom("general");
                out.println("LOGIN_SUCCESS");
                return true;
            }
            out.println("LOGIN_FAILED");
            return false;
        }

        private String hashPassword(String password) {
            try {
                MessageDigest md = MessageDigest.getInstance("SHA-256");
                byte[] hash = md.digest(password.getBytes());
                return Base64.getEncoder().encodeToString(hash);
            } catch (NoSuchAlgorithmException e) {
                e.printStackTrace();
                return password; // Return password if hashing fails
            }
        }
    }

    // Helper classes
    static class User {
        private String username;
        private PrintWriter out;
        private String room;

        public User(String username, PrintWriter out) {
            this.username = username;
            this.out = out;
            this.room = "general"; // Default room
        }

        public String getUsername() {
            return username;
        }

        public PrintWriter getOutStream() {
            return out;
        }

        public void setRoom(String room) {
            this.room = room;
        }

        public String getRoom() {
            return room;
        }

        public void send(String message) {
            out.println(message);
        }
    }

    static class ChatRoom {
        private String name;
        private Set<User> users = ConcurrentHashMap.newKeySet();

        public ChatRoom(String name) {
            this.name = name;
        }

        public void addUser(User user) {
            users.add(user);
        }

        public void removeUser(User user) {
            users.remove(user);
        }

        public void broadcast(String message) {
            users.forEach(u -> u.send(message));
        }
    }

    static class FileTransferRequest implements Serializable {
        String sender;
        String recipient;
        String fileName;
        long fileSize;

        public FileTransferRequest(String sender, String recipient, String fileName, long fileSize) {
            this.sender = sender;
            this.recipient = recipient;
            this.fileName = fileName;
            this.fileSize = fileSize;
        }
    }

    // File transfer client
    static class FileTransferClient extends Thread {
        private User sender;
        private User recipient;
        private String filePath;

        public FileTransferClient(User sender, User recipient, String filePath) {
            this.sender = sender;
            this.recipient = recipient;
            this.filePath = filePath;
        }

        @Override
        public void run() {
            try (FileInputStream fileIn = new FileInputStream(filePath);
                 BufferedInputStream bufferedIn = new BufferedInputStream(fileIn);
                 ObjectOutputStream out = new ObjectOutputStream(recipient.getOutStream().getOutputStream())) {

                byte[] buffer = new byte[4096];
                int bytesRead;
                while ((bytesRead = bufferedIn.read(buffer)) != -1) {
                    out.write(buffer, 0, bytesRead);
                    out.flush();
                }
                out.writeObject("File Transfer Complete");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
