package 08;

public public class SignalDemo {

    static class ChildProcess extends Thread {
        public void run() {
            try {
                System.out.println("Child Process: Waiting for signal...");
                Thread.sleep(5000);  // Simulate waiting for a signal
                System.out.println("Child Process: Received signal, terminating.");
            } catch (InterruptedException e) {
                System.out.println("Child Process: Interrupted by signal.");
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        ChildProcess child = new ChildProcess();
        child.start();  // Start child process thread

        // Simulate a small delay before sending signal
        Thread.sleep(2000);
        System.out.println("Parent Process: Sending signal to child.");
        
        child.interrupt();  // Simulating sending a signal (Interrupt the thread)
        child.join();  // Wait for child to terminate
    }
}
 {
    
}
