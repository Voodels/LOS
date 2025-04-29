import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

public class ProducerConsumerModern {
    public static void main(String[] args) {
        BlockingQueue<Integer> buffer = new ArrayBlockingQueue<>(5);
        
        // Create 2 producers and 2 consumers
        for (int i = 0; i < 2; i++) {
            new Thread(() -> {
                try {
                    int item = 0;
                    while (true) {
                        buffer.put(item++);
                        System.out.println("Produced: " + item);
                        Thread.sleep((long) (Math.random() * 1000));
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }, "Producer-" + (i+1)).start();
            
            new Thread(() -> {
                try {
                    while (true) {
                        int item = buffer.take();
                        System.out.println("Consumed: " + item);
                        Thread.sleep((long) (Math.random() * 1500));
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }, "Consumer-" + (i+1)).start();
        }
    }
}