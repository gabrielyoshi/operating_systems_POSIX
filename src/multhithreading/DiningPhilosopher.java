package multhithreading;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.ThreadLocalRandom;

public class DiningPhilosopher implements DiningServer {
	static final int NUM_PHILOSOPHERS = 5;
	
	public enum StateNames {
		THINKING,
		HUNGRY,
		EATING
	}
	
	// Creates SHARED array of philosophers of type StateNames
	private final StateNames[] philosopher = new StateNames[NUM_PHILOSOPHERS];
	
	// Creates a lock
	private final ReentrantLock lock = new ReentrantLock();
	// Creates array for unique condition variables defined in constructor
	private final Condition[] self = new Condition[NUM_PHILOSOPHERS];
	
	/*
	 * Constructor assigns a condition to each Dining Philosopher
	 * Each philosopher is initialized to "THINKING"
	 */
	public DiningPhilosopher() {
		for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
			// initializes an array of conditions
			self[i] = lock.newCondition();
			philosopher[i] = StateNames.THINKING;
		}
	}
	
	public StateNames getPhilosopherState(int num) {
		return philosopher[num];
	}
	
	/*
	 * Acquires mutex lock, sets the StateName of philosopher indexed at
	 * philosopherNumber to HUNGRY. A while-loop releases lock and waits when either left
	 * or right philosopher StateName is EATING. 
	 * 
	 * When both left AND right philosopher StateName is not EATING, the StateName of 
	 * philosopher indexed at philosopherNumber is set to EATING
	 * Finally block releases mutex lock.
	 * 
	 * @param philospherNumber the index of the philosopher
	 */
	@Override
	public void takeForks(int philosopherNumber) {
		// DONE Auto-generated method stub
		
		int left = (philosopherNumber + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS;
		
		int right = (philosopherNumber + 1) % NUM_PHILOSOPHERS;
		
		lock.lock();
		try {
			philosopher[philosopherNumber] = StateNames.HUNGRY;
			System.out.printf("Philosopher %d is %s.%n", philosopherNumber, philosopher[philosopherNumber]);
			while (philosopher[left] == StateNames.EATING || philosopher[right] == StateNames.EATING) {
				System.out.printf("Philosopher %d must wait.%n", philosopherNumber);
				self[philosopherNumber].await();
			}
			philosopher[philosopherNumber] = StateNames.EATING;
			
		} catch (InterruptedException ie) {}
		finally {
			lock.unlock();
		}
	}
	/*
	 * Acquires lock, sets the StateName of philosopher indexed at philosopherNumber to THINKING.
	 * 
	 * Signals neighboring philosopher threads before returning the mutex lock.
	 * 
	 * @param philosopherNumber the index of the philosopher
	 */
	@Override
	public void returnForks(int philosopherNumber) {
		// TODO Auto-generated method stub
		int left = (philosopherNumber + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS;
		int right = (philosopherNumber + 1) % NUM_PHILOSOPHERS;
		
		lock.lock();
		try {
			philosopher[philosopherNumber] = StateNames.THINKING;
			System.out.printf("Philosopher %d returns left fork and signals to philosopher %d.%n", 
					philosopherNumber, left);
			self[left].signal();
			System.out.printf("Philosopher %d returns right fork and signals to philosopher %d.%n", 
					philosopherNumber, right);
			self[right].signal();
		} finally {
			lock.unlock();
		}
	}

public static void main(String args[]) throws InterruptedException {
	
	DiningPhilosopher server = new DiningPhilosopher(); // shared resource
	Thread[] threads = new Thread[NUM_PHILOSOPHERS];
	
	System.out.printf("%d philosophers sit down around a table.%n", NUM_PHILOSOPHERS);
	
	for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
		
		final int id = i;
		
		Runnable task = () -> {
			try {
				// Lambda expression the defines the functional operation of each thread
				while (true) {
					System.out.printf("Philosopher %d is %s.%n", id, server.getPhilosopherState(id));
					int seconds = ThreadLocalRandom.current().nextInt(1,4);
					Thread.sleep(seconds * 1000L);
					server.takeForks(id);
					try {
						System.out.printf("Philosopher %d is %s.%n", id, server.getPhilosopherState(id));
						int pseconds = ThreadLocalRandom.current().nextInt(1,4);
						Thread.sleep(pseconds * 1000L);
					} finally {
						server.returnForks(id);
					}
				}
			} catch (InterruptedException e) {
				Thread.currentThread().interrupt();
			}
		};
			
		threads[i] = new Thread(task);
		threads[i].start();
	}
	
	for (Thread thread : threads) {
		thread.join();
		}	
	}
}
	
