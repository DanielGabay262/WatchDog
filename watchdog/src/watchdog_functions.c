#define _POSIX_C_SOURCE 200809L

#include <stdlib.h> /*execvp*/
#include <unistd.h> /*fork*/
#include <pthread.h> /*thred functions*/
#include <fcntl.h>  /*sem_open*/
#include <semaphore.h> /*sem_post*/
#include <signal.h> /*kill*/
#include <stdatomic.h> /*atomic*/
#include <stdio.h> /*printf*/
#include <time.h> /*time*/

#include "watchdog.h"
#include "utility.h"
#include "scheduler.h"

#define MAX 50
#define TIME_INTERVAL_CHECK 4
#define TIME_INTERVAL_SEND 1
#define TIME_INTERVAL_CHECK_FLAG 1
#define USER_PROCESS 0
#define WATCHDOG_PROCESS 1

atomic_int counter = 0;
atomic_int flag = ON;
sem_t *sem;
pthread_mutex_t mutex;

struct watchdog
{
    pid_t other_process_pid;
    char *other_file_name;
    scheduler_t *scheduler;
    char *my_name;
    int process;
    pthread_t thread;
};


void WriteToFile(char *str)
{
    int res = 0;
    time_t t;
    FILE *fp;

    time(&t);

    pthread_mutex_lock(&mutex);
    fp = fopen("log.txt", "a");
    if (fp == NULL)
    {
        #ifndef NDEBUD
        printf("error opening log file!\n");
        #endif
        return;
    }
    fprintf(fp, "%s\t\t\t\t\t\t %s\n", ctime(&t), str);
    res = fclose(fp);
    pthread_mutex_unlock(&mutex);

    #ifndef NDUBG
    if (res == EOF)
    {
        printf("error closing log file!");
    }
    #endif
}



static void ReviveUser(watchdog_t *data)
{
    pid_t new_user_pid;
    char *open_new_process[2];

    unsetenv("WD_PID");

    kill((data->other_process_pid), SIGKILL);

    open_new_process[0] = data->other_file_name;
    open_new_process[1] = NULL;
    new_user_pid = fork();
    if (new_user_pid == 0)
    {
        if (execvp(open_new_process[0], open_new_process) == -1)
        {
            WriteToFile("[Severity: Error] couldn't revive user program!");
            exit(FAILURE);
        }
        exit(SUCCESS);
    }
    else if (new_user_pid < 0)
    {
        WriteToFile("[Severity: Error] couldn't revive user program!");
        return;
    }
    WriteToFile("[Severity: Info] Revived user process!");

    SchedulerStop(data->scheduler);
    SchedulerDestroy(data->scheduler);
    free(data);

    exit(SUCCESS);
}

static void ReviveWatchdog(watchdog_t *data)
{
    pid_t new_watchdog_pid;
    char *open_new_process[3];
    unsetenv("WD_PID");

    kill((data->other_process_pid), SIGKILL);

    open_new_process[0] = data->other_file_name;
    open_new_process[1] = data->my_name;
    open_new_process[2] = NULL;

    new_watchdog_pid = fork();
    if (new_watchdog_pid == 0)
    {
        if (execvp(open_new_process[0], open_new_process) == -1)
        {
            WriteToFile("[Severity: Error] couldn't revive the watchdog program!");
            exit(FAILURE);
        }
        exit(SUCCESS);
    }
    else if (new_watchdog_pid < 0)
    {
        WriteToFile("[Severity: Error] couldn't revive the watchdog program!");
        return;
    }

    WriteToFile("[Severity: Info] Revived watchdog process!");
    data->other_process_pid = new_watchdog_pid;

    sem_wait(sem);
}

static void SignalUSR1Handler(int signo, siginfo_t *si, void *data)
{
    (void)data;
    (void)signo;
    (void)si;
    ++counter;
}

static void SignalUSR2Handler(int signo, siginfo_t *si, void *data)
{
    (void)data;
    (void)signo;
    (void)si;
    flag = OFF;
}


static void SetSignalMask()
{
    sigset_t signal_mask;
    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGUSR1);
    pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);
}


static void UnmaskAndInitSignalHandler()
{
    struct sigaction newhandler = {0};
    struct sigaction newhandler2 = {0};
    sigset_t signal_mask;
    sigemptyset(&signal_mask);
    sigaddset(&signal_mask, SIGUSR1);
    pthread_sigmask(SIG_UNBLOCK, &signal_mask, NULL);

    newhandler.sa_flags = SA_NODEFER | SA_SIGINFO;
    newhandler.sa_sigaction = SignalUSR1Handler;
    sigemptyset(&newhandler.sa_mask);
    sigaction(SIGUSR1, &newhandler, NULL);

    newhandler2.sa_flags = SA_NODEFER | SA_SIGINFO;
    newhandler2.sa_sigaction = SignalUSR2Handler;
    sigemptyset(&newhandler2.sa_mask);
    sigaction(SIGUSR2, &newhandler2, NULL);
}

static int CheckTask(void *param)
{
    watchdog_t *data = (watchdog_t *)param;
    if (counter <= 1 && flag == ON)
    {
        if (data->process == WATCHDOG_PROCESS)
        {
            WriteToFile("[Severity: Info] need to revive user process!");
            ReviveUser(param);
        }
        else if (data->process == USER_PROCESS)
        {
            WriteToFile("[Severity: Info] need to revive dog process!!!");
            ReviveWatchdog(param);
        }
    }
    counter = 0;
    return SUCCESS;
}

static int SendSignalTask(void *param)
{
    kill(*(pid_t *)param, SIGUSR1);
    return SUCCESS;
}

static int CheckFlagTask(void *param)
{
    watchdog_t *watchdog = (watchdog_t *)param;
    if (flag == OFF)
    {
        SchedulerStop(watchdog->scheduler);
        unsetenv("WD_PID");
        return 1;
    }
    return SUCCESS;
    
}

static int InitScheduler(watchdog_t *watchdog)
{
    if (UIDIsSame(SchedulerAddTask(watchdog->scheduler, TIME_INTERVAL_CHECK, CheckTask, watchdog), g_BadUid) == TRUE)
    {
        return FAILURE;
    }

    if (UIDIsSame(SchedulerAddTask(watchdog->scheduler, TIME_INTERVAL_SEND, SendSignalTask, &watchdog->other_process_pid), g_BadUid) == TRUE)
    {
        return FAILURE;
    }

    if (UIDIsSame(SchedulerAddTask(watchdog->scheduler, TIME_INTERVAL_CHECK_FLAG, CheckFlagTask, watchdog), g_BadUid) == TRUE)
    {
        return FAILURE;
    }

    return SUCCESS;
}


static void *Scheduler_Thread(void *watchdog_data)
{
    watchdog_t *watchdog = (watchdog_t *)watchdog_data;
    UnmaskAndInitSignalHandler();

    if (InitScheduler(watchdog) == FAILURE)
    {
        SchedulerDestroy(watchdog->scheduler);
        free(watchdog);
        #ifndef NDEBUG
        WriteToFile("[Severity: Error] couldn't init user scheduler!");
        #else
        WriteToFile("[Severity: Error] Watchdogstart failed!");
        #endif
        return NULL;
    }

    if (sem_wait(sem) != 0)
    {
        #ifndef NDEBUG
        WriteToFile("[Severity: Error] user sem_wait is failed");
        #else
        WriteToFile("[Severity: Error] problem with Watchdogstart!");
        #endif
    }

    if (SchedulerRun(watchdog->scheduler) != 0)
    {
        #ifndef NDEBUG
        WriteToFile("[Severity: Error] user scheduler run failed!");
        #else
        WriteToFile("[Severity: Error] problem with watchdog!");
        #endif
    }
    else
    {
        WriteToFile("[Severity: info] user Watchdog stopped successfully!");
    }
    SchedulerDestroy(watchdog->scheduler);
    
    return NULL;
}


watchdog_t *WatchdogStart(char *user_program_path)
{
    pid_t other_pid;
    pthread_t scheduler_thread;
    char *open_watchdog_process[3];
    watchdog_t *watchdog_data = (watchdog_t *)malloc(sizeof(watchdog_t));

    if (watchdog_data == NULL)
    {
        WriteToFile("[Severity: Error] WatchdogStart failed, not enough place!");
        return NULL;
    }

    flag = ON;
    counter = 0;

    watchdog_data->scheduler = SchedulerCreate();
    if (watchdog_data->scheduler == NULL)
    {
        WriteToFile("[Severity: Error] WatchdogStart failed, not enough place!");
        free(watchdog_data);
        return NULL;
    }

    if (getenv("WD_PID") == NULL)
    {
        #ifndef NDEBUG
        printf("user process just opened!!\n");
        printf("user pid: %d\n", getpid());
        #endif
        SetSignalMask();
        sem_unlink("watch_semaphore");
        sem = sem_open("watch_semaphore", O_CREAT, 0644, 0);
        if (sem == SEM_FAILED)
        {
            #ifndef NDEBUG
            WriteToFile("[Severity: Error] WatchdogStart failed, sem_unlink failed!");
            #else
            WriteToFile("[Severity: Error] WatchdogStart failed!");
            #endif
            return NULL;
        }
        watchdog_data->other_file_name = "./watchdog.out";
        watchdog_data->my_name = user_program_path;
        watchdog_data->process = USER_PROCESS;
        open_watchdog_process[0] = "./watchdog.out";
        open_watchdog_process[1] = user_program_path;
        open_watchdog_process[2] = NULL;

        other_pid = fork();
        if (other_pid == 0)
        {
            if (execvp(open_watchdog_process[0], open_watchdog_process) == -1)
            {
                WriteToFile("[Severity: Error] couldn't start the watchdog program!");
                exit(FAILURE);
            }
            exit(SUCCESS);
        }
        else if (other_pid < 0)
        {
            WriteToFile("[Severity: Error] couldn't start the watchdog program!");
            return NULL;
        }
        watchdog_data->other_process_pid = other_pid;
        if (pthread_create(&scheduler_thread, NULL, Scheduler_Thread, watchdog_data) != 0)
        {
            #ifndef NDEBUG
            WriteToFile("[Severity: Error] WatchdogStart failed, couldn't create new thread!");
            #else
            WriteToFile("[Severity: Error] WatchdogStart failed!");
            #endif
            unsetenv("WD_PID");
            return NULL;
        }

        watchdog_data->thread = scheduler_thread;
        WriteToFile("[Severity: Info] Watchdog start!");

        return watchdog_data;
    }

    else
    {
        sem = sem_open("watch_semaphore", O_CREAT, 0644, 0);
        if (sem == SEM_FAILED)
        {
            #ifndef NDEBUG
            WriteToFile("[Severity: Error] WatchdogStart program start failed, couldn't open semaphore!");
            #else
            WriteToFile("[Severity: Error] Watchdog program start failed!");
            #endif
            unsetenv("WD_PID");
            return NULL;
        }
        UnmaskAndInitSignalHandler();
        watchdog_data->other_process_pid = getppid();
        watchdog_data->other_file_name = user_program_path;
        watchdog_data->process = WATCHDOG_PROCESS;
        watchdog_data->my_name = NULL;
        watchdog_data->thread = 0;

        if (InitScheduler(watchdog_data) == FAILURE)
        {
            SchedulerDestroy(watchdog_data->scheduler);
            free(watchdog_data);
            #ifndef NDEBUG
            WriteToFile("[Severity: Error] WatchdogStart program start failed, couldn't init scheduler!");
            #else
            WriteToFile("[Severity: Error] Watchdog program start failed!");
            #endif
            unsetenv("WD_PID");
            return NULL;
        }

        if (sem_post(sem) != 0)
        {
            #ifndef NDEBUG
            WriteToFile("[Severity: Error] WatchdogStart program sem_post is failed");
            #else
            WriteToFile("[Severity: Error] problem with Watchdog program start!");
            #endif
        }

        if (SchedulerRun(watchdog_data->scheduler) != 0)
        {
            #ifndef NDEBUG
            WriteToFile("[Severity: Error] scheduler run failed!");
            #else
            WriteToFile("[Severity: Error] some problem with watchdog!");
            #endif
        }
        else
        {
            WriteToFile("[Severity: info] Watchdog stopped successfully!");
        }

        SchedulerDestroy(watchdog_data->scheduler);
        if (sem_close(sem) != 0)
        {
            #ifndef NDEBUG
            WriteToFile("[Severity: Error] sem_close failed!");
            #endif
        }
        free(watchdog_data);
        return NULL;

    }
}



void WatchdogStop(watchdog_t *watchdog)
{
    flag = OFF;
    kill(watchdog->other_process_pid, SIGUSR2);
    pthread_join(watchdog->thread, NULL);
    sem_close(sem);
    free(watchdog);
}







