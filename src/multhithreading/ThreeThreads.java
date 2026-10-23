package multhithreading;

import java.util.Arrays;


public class ThreeThreads {


    public static void main(String[] args) throws InterruptedException {

        // Creating a large array of numbers for summation

        int[] numbers = new int[90_000_000];

        Arrays.fill(numbers, 1); // Fill the array with 1s for simplicity

 

        // 1. Summing the array using a single thread

        long startTime = System.currentTimeMillis();

        long singleThreadSum = singleThreadSum(numbers);

        long endTime = System.currentTimeMillis();

        System.out.println("Single-thread sum: " + singleThreadSum);

        System.out.println("Time taken with single thread: " + (endTime - startTime) + " ms");

 

        // 2. Summing the array using three threads

        startTime = System.currentTimeMillis();

        long multiThreadSum = multiThreadSum(numbers);

        endTime = System.currentTimeMillis();

        System.out.println("Multi-thread sum: " + multiThreadSum);

        System.out.println("Time taken with three threads: " + (endTime - startTime) + " ms");

    }

 

    // Single-thread summing function

    private static long singleThreadSum(int[] numbers) {

        long sum = 0;

        for (int number : numbers) {

            sum += number;

        }

        return sum;

    }

 

    // Multi-thread summing function using three threads

    private static long multiThreadSum(int[] numbers) throws InterruptedException {

        final int length = numbers.length;

        final long[] results = new long[3];

 

        // Creating three threads, each summing one-third of the array

        Thread t1 = new Thread( () -> results[0] = partialSum(numbers, 0, length / 3));

        Thread t2 = new Thread(  () -> results[1] = partialSum(numbers, length / 3, 2 * length / 3));

        Thread t3 = new Thread(  () -> results[2] = partialSum(numbers, 2 * length / 3, length));

 

        // Starting and joining threads

        t1.start();

        t2.start();

        t3.start();

        t1.join();

        t2.join();

        t3.join();

 

        // Summing up results from each thread

        return results[0] + results[1] + results[2];

    }

 

    // Helper method for calculating partial sum in a given range

    private static long partialSum(int[] numbers, int start, int end) {

        long sum = 0;

        for (int i = start; i < end; i++) {

            sum += numbers[i];

        }

        return sum;

    }

}

 

/*Result:

Single-thread sum: 90000000

Time taken with single thread: 34 ms

Multi-thread sum: 90000000

Time taken with three threads: 26 ms

 

*/

 



