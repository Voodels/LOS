import sun.misc.Signal;
import sun.misc.SignalHandler;
import java.io.IOException;

public class SignalDemo {
    public static void main(String[] args) throws IOException, InterruptedException {
        if (args.length == 0) {  // Parent
            ProcessBuilder pb = new ProcessBuilder("java", "SignalDemo", "child");
            Process child = pb.start();
            
            // Get PID from child output
            byte[] buffer = new byte[100];
            int bytesRead = child.getInputStream().read(buffer);
            String pidLine = new String(buffer, 0, bytesRead).trim();
            int pid = Integer.parseInt(pidLine.split(":")[1].trim());
            
            Thread.sleep(1000);  // Allow child setup
            System.out.println("Parent: Sending SIGUSR1");
            Runtime.getRuntime().exec("kill -SIGUSR1 " + pid).waitFor();
            child.waitFor();
        } else {  // Child
            Signal.handle(new Signal("USR1"), sig -> {
                System.out.println("Child: Received SIGUSR1");
                System.out.println("Child: Exiting");
                System.exit(0);
            });
            System.out.println("Child: PID: " + ProcessHandle.current().pid());
            while (true) Thread.sleep(1000);
        }
    }
}