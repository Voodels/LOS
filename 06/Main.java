import java.io.IOException;

public class Main {
    public static void main(String[] args) throws IOException, InterruptedException {
        // execvp equivalent (PATH search)
        ProcessBuilder pb1 = new ProcessBuilder("ls", "-l");
        pb1.inheritIO();
        pb1.start().waitFor();

        // execv equivalent (absolute path)
        ProcessBuilder pb2 = new ProcessBuilder("/bin/ls", "-l");
        pb2.inheritIO();
        pb2.start().waitFor();

        // execve equivalent (custom environment)
        ProcessBuilder pb3 = new ProcessBuilder("ls", "-l");
        pb3.environment().put("MY_ENV", "test");
        pb3.inheritIO();
        pb3.start().waitFor();
    }
}