package multhithreading;

/*The following shows how two threads are created using the second technique.

How a shared variable is created in java

How we can synchronize using the shared variable using the monitor technique which

Java has synchronized keyword.*/

public class Main {

    public static void main(String[] args) throws InterruptedException {

        SharedResource resource = new SharedResource();

        Thread t1 = new Thread(new Worker(resource));

        Thread t2 = new Thread(new Worker(resource));


 

        t1.start();

        t2.start();

        t1.join();

        t2.join();

 

        System.out.println("Final Counter Value: " + resource.getCounter());
    }
}