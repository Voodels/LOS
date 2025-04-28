import java.io.IOException;

public class Main {
    public static void main(String[] args) throws IOException {
        // Create a new instance of the GUI class
        try{
            System.out.println("Opening Calculator GUI...");
            Runtime.getRuntime().exec("gnome-calculator");
        }catch(IOException e){
            System.out.println("Error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
