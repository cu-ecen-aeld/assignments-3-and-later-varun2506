#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

#define ERROR 		(-1)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    
    int retval = 0;
    
   struct thread_data* thread_func_args = (struct thread_data *) thread_param; 
   thread_func_args -> thread_complete_success = true;
   
    int wait_before_mutex_us = thread_func_args->wait_before_mutex*1000;
    int wait_after_mutex_us = thread_func_args->wait_after_mutex*1000;
    

     // Wait for mutex
    retval = usleep(wait_before_mutex_us);
   if (retval == ERROR)
   {
      ERROR_LOG("usleep failed\n");
      thread_func_args -> thread_complete_success = false;
      return thread_param;
   }
   DEBUG_LOG("slept for %d msec before mutex succesfull\n", thread_func_args->wait_before_mutex);
   
    // Obtain mutex lock
   retval = pthread_mutex_lock(thread_func_args->mutex_thread);
    if (retval == ERROR)
   {
      ERROR_LOG("Obtain mutex lock failed with error code %d\n", retval);
      thread_func_args->thread_complete_success = false;
      return thread_param;
   }
   DEBUG_LOG("Successfully obtained mutex lock\n");
   
   // Wait some time after mutex
   retval = usleep(wait_after_mutex_us);
   if (retval == ERROR)
    {
      ERROR_LOG("Sleep after mutex failed for %d msec.\n", thread_func_args->wait_after_mutex);
      thread_func_args->thread_complete_success = false;
      return thread_param;
   }
   DEBUG_LOG("Successfully slept for %d msec after mutex\n", thread_func_args->wait_after_mutex);
   
   // Obtain mutex lock
   retval = pthread_mutex_unlock(thread_func_args->mutex_thread);
   if (retval == ERROR)
    {
      ERROR_LOG("Unlock mutex lock failed with error code %d\n", retval);
      thread_func_args->thread_complete_success = false;
      return thread_param;
   }
   DEBUG_LOG("Successfully unlocked mutex lock\n");

   DEBUG_LOG("Successfully completed thread\n");
   thread_func_args->thread_complete_success = true;
   
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     * 
     * See implementation details in threading.h file comment block
     */
     
     int retval = 0;

    struct thread_data *threadparam;
    threadparam = (struct thread_data *) malloc( sizeof(struct thread_data) );
    
    threadparam -> wait_before_mutex  = wait_to_obtain_ms;
    threadparam -> wait_after_mutex = wait_to_release_ms;
    threadparam -> mutex_thread = mutex;
    
    threadparam -> thread_complete_success = false;
    
    retval = pthread_create(thread, NULL, threadfunc, (void *)threadparam);
    if(retval == ERROR)
    {
       ERROR_LOG("pthread_create failed with error code = %d\n", retval);
       return false;
    }
    
    DEBUG_LOG("pthread_create was successful\n");
    return true;
     
}

