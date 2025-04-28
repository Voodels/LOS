public class Main{
    public static void main(String[] args) {
        int a = 5;
        int b = 10;

        for (int i = 0; i < 5; i++) {
            final int idx = i;
            /*
             * explain Final : 
             *  Final is a keyword in Java that is used to declare constants.
             *  When a variable is declared as final, its value cannot be changed once it has been assigned.
             *  This means that the variable becomes a constant and cannot be reassigned to a different value.
             *  This is useful when you want to ensure that a variable's value remains constant throughout the program.
             *  For example, if you have a constant value that should not be changed, you can declare it as final.
             *  This can help prevent accidental changes to the variable's value and make the code more readable.
             *  In the above code, the variable idx is declared as final, which means that its value cannot be changed once it has been assigned.
             *  This is useful when you want to ensure that the value of idx remains constant throughout the program.
             * 
             */
            new Thread(()->{
                switch (idx) {
                    case 0: System.out.println("Child 0 : " + (a+b));                        
                        break;
                    case 1: System.out.println("Child 1 : " + (a-b));
                        break;
                    case 2: System.out.println("Child 2 : " + (a*b));
                        break;
                    case 3: System.out.println("Child 3 : " + (a/b));
                        break;
                    case 4: System.out.println("Child 4 : " + (a%b));
                        break;
                }
            }).start();
            /*
             * explain Thread :
             *  A thread is a lightweight process that can run concurrently with other threads in a program.
             *  Threads are used to perform multiple tasks simultaneously, which can improve the performance and responsiveness of a program.
             *  In Java, threads can be created by extending the Thread class or implementing the Runnable interface.
             *  Once a thread is created, it can be started using the start() method, which will invoke the run() method of the thread.
             *  Threads can also be synchronized to ensure that shared resources are accessed safely
             *  and to prevent
             */ 
        }
    }
}