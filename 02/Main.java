public class Main {
    static String username;
    
    public static void main(String[] args) throws InterruptedException {
        Thread child = new Thread(() -> {
            System.out.print("Enter username: ");
            java.util.Scanner scanner = new java.util.Scanner(System.in);
            username = scanner.nextLine();
        });
        child.start();
        child.join(); // Wait for child thread
        
        System.out.print("Enter password: ");
        String password = new java.util.Scanner(System.in).nextLine();
        
        if ("secret".equals(password)) {
            System.out.println("Login successful for user: " + username);
        } else {
            System.out.println("Invalid password!");
        }
    }
}