package multhithreading;


public class Worker implements Runnable {

    private SharedResource resource;

 

    public Worker(SharedResource resource) {

        this.resource = resource;

    }

 

    @Override

    public void run() {

        for (int i = 0; i < 1000000; i++) {

            resource.incrementCounter();

        }

    }

}
