public class ExitWait {
    public static void main(String[] args) throws Exception {
        ProcessBuilder pb = new ProcessBuilder("sh", "-c", "exit 42");
        Process p = pb.start();
        System.out.println("Parent waiting for child...");
        int exitCode = p.waitFor();
        System.out.println("Child exited with code: " + exitCode);
    }
}