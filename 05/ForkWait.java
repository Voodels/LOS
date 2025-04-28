public class ForkWait {
    public static void main(String[] args) {
        try {
            ProcessBuilder pb = new ProcessBuilder("sleep", "2");
            Process child = pb.start();
            System.out.println("Parent waiting for child...");
            int exitCode = child.waitFor(); // Blocks until child finishes
            System.out.println("Child exited with code: " + exitCode);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}