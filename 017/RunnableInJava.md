In Java, the `Runnable` interface represents a task that can be executed concurrently by a thread. It is often used for defining a task to be run by a thread. Instead of extending the `Thread` class, you can implement the `Runnable` interface and pass it to a `Thread` object. This allows for greater flexibility, as your class can extend another class while still being able to execute concurrently.

### Key Points:
- **Runnable Interface**: The `Runnable` interface defines a single method, `run()`, which contains the code to be executed by the thread.
- **Threading Model**: When a `Runnable` is passed to a `Thread` object, the `run()` method of the `Runnable` is invoked when the thread is started using the `start()` method.

### Syntax:
```java
public interface Runnable {
    public void run();
}
```

### Why Use `Runnable`?
1. **Separation of Concerns**: You can implement `Runnable` in a class without needing to extend `Thread`, which allows you to extend other classes simultaneously.
2. **Flexibility**: You can pass the `Runnable` to different thread execution mechanisms, such as `Thread`, `ExecutorService`, or thread pools.
3. **Better Resource Management**: Using `Runnable` with thread pools is generally more efficient than creating new threads manually.

### Example of Using `Runnable`:
Here’s a basic example to demonstrate how `Runnable` works:

```java
public class MyRunnable implements Runnable {
    @Override
    public void run() {
        System.out.println("Task is running in thread: " + Thread.currentThread().getName());
    }

    public static void main(String[] args) {
        // Create an instance of MyRunnable
        MyRunnable task = new MyRunnable();
        
        // Create a Thread and pass the Runnable object to it
        Thread thread = new Thread(task);
        
        // Start the thread
        thread.start();
        
        // Output from the main thread
        System.out.println("Main thread: " + Thread.currentThread().getName());
    }
}
```

### Output:
```
Main thread: main
Task is running in thread: Thread-0
```

### Breakdown of the Example:
1. **Implementing `Runnable`**: The class `MyRunnable` implements the `Runnable` interface and overrides the `run()` method. This method contains the task that should be executed by the thread.
2. **Creating and Starting a Thread**: In the `main` method, a new `Thread` is created, and an instance of `MyRunnable` is passed to it. The `start()` method is called on the thread, which invokes the `run()` method in a new thread.
3. **Main Thread Output**: The main thread prints its own name, and the newly created thread executes the task defined in the `run()` method.

### Using `Runnable` with Executors (Thread Pool):
Using `Runnable` with a thread pool (via `ExecutorService`) is more efficient and scalable than manually creating threads:

```java
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class RunnableWithExecutor {
    public static void main(String[] args) {
        ExecutorService executor = Executors.newFixedThreadPool(2); // Create a thread pool

        // Submit tasks to the thread pool
        executor.submit(new MyRunnable());
        executor.submit(new MyRunnable());

        // Shutdown the executor service
        executor.shutdown();
    }
}

class MyRunnable implements Runnable {
    @Override
    public void run() {
        System.out.println("Task is running in thread: " + Thread.currentThread().getName());
    }
}
```

### Output:
```
Task is running in thread: pool-1-thread-1
Task is running in thread: pool-1-thread-2
```

In this example:
- An `ExecutorService` with a fixed thread pool is used to execute the tasks.
- Each task is submitted to the pool, and the pool assigns the tasks to available threads.

### Summary:
- The `Runnable` interface is used to define a task that can be executed by a thread.
- It allows you to define tasks in classes that don’t need to extend the `Thread` class.
- You can pass a `Runnable` object to a `Thread` or use thread pools for more efficient thread management.
