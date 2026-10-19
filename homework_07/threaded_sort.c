#include <pthread.h>

#include <stdio.h>

#include <stdlib.h>

#define SIZE 10
#define NUM_SORT_THREADS 4
#define NUM_MERGE_THREADS 3 
int list[SIZE] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};

// stores the final sorted list
int result[SIZE];

typedef struct
{    
	int from_index;
	int to_index;
} parameters;

// data structure for merging
typedef struct {
	int from_index;
	int middle_index;
	int to_index;
} merge_parameters;

void *sorter(void *params);
void *merger(void *params);

int main(void)
{
pthread_t sorter_thread[NUM_SORT_THREADS];
pthread_t merger_thread[NUM_MERGE_THREADS]; 

int boundaries[NUM_SORT_THREADS + 1];

// divide array into 4 segment starting indices + one position 
// after last array index
for (int i = 0; i <= NUM_SORT_THREADS; i++)
	boundaries[i] = i * SIZE / NUM_SORT_THREADS;

// builds parameters data structure and creates associated thread
for (int i = 0; i < NUM_SORT_THREADS; i++) {

	// data pointer to a parameters structure
	parameters *data = (parameters *) malloc(sizeof(parameters));

	// represents starting index of each array segment
	data->from_index = boundaries[i];
	// represents the ending index of each array segment
	data->to_index = boundaries[i + 1] - 1;

	pthread_create(&sorter_thread[i], 0, sorter, data);
}

// pausing until all threads are sorted
for(int i=0; i < NUM_SORT_THREADS; i++)
     pthread_join(sorter_thread[i], NULL);

// Starts 2 merge threads that operate on the sorted arrays
for (int i = 0; i < 2; i++) {

	// data is a pointer to the merge_parameters structure
	merge_parameters *data = (merge_parameters *) malloc(sizeof(merge_parameters));
	
	// keeps subarrays independent of each other
	int subarray = 2 * i; 
	
	// sets indices for merged array
	data->from_index = boundaries[subarray];
	data->middle_index = boundaries[subarray + 1] - 1;
	data->to_index = boundaries[subarray + 2] - 1;

	pthread_create(&merger_thread[i], 0, merger, data);
}

// pausing until both halves are sorted and merged
for (int i = 0; i < 2; i++)
	pthread_join(merger_thread[i], NULL);


// Last merge

merge_parameters *data = (merge_parameters *) malloc(sizeof(merge_parameters));

data->from_index = 0;
data->middle_index = boundaries[2] - 1;
data->to_index = SIZE - 1;

pthread_create(&merger_thread[2], 0, merger, data);

pthread_join(merger_thread[2], 0);
// Testing
printf("Sorted array: ");
for (int i = 0; i < SIZE; i++) {
	printf("%d ", list[i]);
}
printf("\n");

return 0;

}
// parameter structure passed in 
void *sorter(void *params)

{

parameters *p = (parameters *) params;

int begin = p->from_index;
int end   = p->to_index;

int swapped = 1;

int j =0;
int i;
int temp;

while(swapped == 1){
     swapped = 0;
     j++;

     for(i = begin; i <= end - j; i++) {
        if (list[i] > list[i + 1]) {

           temp = list[i];
           list[i] = list[i+1];
           list[i+1] = temp;
           swapped = 1;
           }
    }
}

pthread_exit(0);

}

void *merger(void *params)

{

merge_parameters * p = (merge_parameters *)params;
// local variables to store start, middle, and end 
int i, j, end;

i = p->from_index;
j = p->middle_index + 1;
end = p->to_index;

int position = i;

// Compare elements while both segments have values
while (i <= p->middle_index  && j <= end)
	{
	// compares indices of global list[]
	if (list[i] <= list[j]) {
   		result[position] = list[i];
   		i++;
   		position++;
		}		
	else
	{	
   		result[position] = list[j];
   		j++;
   		position++;
		}
	}	

/* copy the remainder */

if (i <= p->middle_index) {
	
     while(i <= p->middle_index) {
        result[position] = list[i];
        position++;
        i++;
        }
    }
else
{
    while (j <= end) {
     result[position] = list[j];
     position++;
     j++;
	}
}

// copy merged range result[] back to list[]
for (int k = p->from_index; k <= end; k++) {
	list[k] = result[k];
}
pthread_exit(0);
}
