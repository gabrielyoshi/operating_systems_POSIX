package multhithreading;

public class SharedResource {

    private int sharedCounter = 0;

 

    // Synchronized method to safely increment the counter
    // JVM will give lock to the thread that is running incrementCounter

    public synchronized void incrementCounter() {

        sharedCounter++; // critical section

    }

    // Synchronized getter to ensure consistent read access

    public synchronized int getCounter() {

        return sharedCounter; // critical section

    }

}