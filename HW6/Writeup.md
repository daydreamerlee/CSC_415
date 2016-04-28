# CSC415 HW6
BY Cheng Li 916422876

### Requirement
Implement the LRU and FIFO algorithm and compare their performances.


### Performance analysis
The performance of FIFO and LRU with 100 random accesses is shown as below:
There 
![](https://raw.githubusercontent.com/daydreamerlee/CSC_415/master/HW6/performance.png)

### FIFO algorithm
```c
/*Return the number of faults*/
int fifo(int* r_table, int reference_num, int* p_table, int p_number, int p_size)
{
    int num_page_fault=0;
    int page_id=0;
    for (int i = 0; i < reference_num; ++i)
    {
        page_id= r_table[i] >> p_size;
        if (find_page_inTable(p_table,p_number,page_id) == -1) // page fault
         {
                num_page_fault++;
                int first_empty = find_page_inTable(p_table,p_number,0);
                if(first_empty != -1)
                { //still blcnk slot
                    p_table[first_empty]=page_id;
                } else 
                {
                    p_table[num_page_fault % p_number] = page_id;
                }
         } 
    }
    return num_page_fault;
}
```

### LRU algorithm
Note that LRU uses a time_of_change array to track the latest time that the page was accessed. Locating the least item and the index of that item can find the least recent used page.
```C
int lru(int* r_table, int reference_num, int* p_table, int p_number, int p_size)
{
    int num_page_fault = 0;
    int page_id = 0;
    int time_stamp = 0;
    int* time_of_change = (int*) malloc(p_number * sizeof(int)); // tranking the time that every page in the table is called
    for (int i = 0; i < p_number; ++i) //INI all time of change @ 0 
     {
        time_of_change[i] = 0;
     } 

    for (int i = 0; i < reference_num; ++i) // for each page call ,do the lru swap
     {
         int least_used_page_index = 0;
         int least_used_timestamp = time_of_change[0];
         time_stamp++; // update timestamp every time a reference call for a page
         page_id = r_table[i] >> p_size;
         int position_of_found= find_page_inTable(p_table,p_number,page_id);
         if (position_of_found != -1 ) // page hit, just need to update the time stamp for the hitted page
         {
            time_of_change[position_of_found] = time_stamp;
         }else  // find the minimal in the time stamp array, which is the least used page, and swap it.
         /*Be note if the p_table is not full ,then will return the first position for 0 to place the new item*/
         {
            num_page_fault++;
            /*WILL return the least time stamp and the least recent used index*/
            for (int i = 0; i < p_number; ++i)
            {
                if(time_of_change[i] < least_used_timestamp){
                    least_used_timestamp=time_of_change[i];
                    least_used_page_index=i;
                }  
            }
            /*___________________*/

            // printf("Change %d for %d. \n",p_table[least_used_page_index],page_id );
            p_table[least_used_page_index] = page_id; //update the page table and its time stamp
            time_of_change [least_used_page_index] = time_stamp;
         }
     } 

     free(time_of_change);
     return num_page_fault;
}
```

### Search for the item in the page table
If the func dont't find the target item, then return -1.
```C
int find_page_inTable(int* page_table, int table_size, int target){
    /*Find the page number in the page table, return the index if found, else return -1*/
    int i=0;
    for  (i = 0; i < table_size; ++i)
    {
        if(page_table[i] == target) return i;
    }
    return -1;
}

```