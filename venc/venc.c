/*
 *******************************************************************************
 * $Header: $
 *
 *  Copyright (c) 2007-2010 VN Inc. All rights reserved.
 *
 *  +-----------------------------------------------------------------+
 *  | THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY ONLY BE USED |
 *  | AND COPIED IN ACCORDANCE WITH THE TERMS AND CONDITIONS OF SUCH  |
 *  | A LICENSE AND WITH THE INCLUSION OF THE THIS COPY RIGHT NOTICE. |
 *  | THIS SOFTWARE OR ANY OTHER COPIES OF THIS SOFTWARE MAY NOT BE   |
 *  | PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY OTHER PERSON. THE   |
 *  | OWNERSHIP AND TITLE OF THIS SOFTWARE IS NOT TRANSFERRED.        |
 *  |                                                                 |
 *  | THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT   |
 *  | ANY PRIOR NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY |
 *  | VN INC.                                                     |
 *  +-----------------------------------------------------------------+
 *
 * $History: $
 *
 *******************************************************************************
 */

/*!
 *******************************************************************************
 * Copyright 2007-2010 VN, Inc. All rights reserved.
 *
 * \file
 * venc.c
 *
 * \brief
 * Main file of video process.
 *
 * \date
 * 2009/08/06
 *
 * \author
 * Acer Tsai
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <syslog.h>
#include <sys/types.h>


#include "venc_app.h"

static const char id[] = "$Id: venc.c 34207 2009-08-06 09:00:57Z acer.tsai $";

HANDLE hVencObject;
BOOL 	g_bTerminate;

/* ============================================================================================= */
void print_usage(void)
{
    fprintf(stderr, "Venc process\n"
           "Usage:\n"
           "    venc <-m dev_num> [-D] [-h] [-c config_file] [-f command_fifo] [-p pidfile]\n"
           "Options:\n"
           "    -m                 Device number venc used, the Mozart support to 8(0~7)\n"
           "    -D                 Run as Daemon\n"
           "    -c config_file     Configuration file of this module\n"
           "    -f command_fifo    Command fifo path of venc\n"
           "    -p pid_file        Write PID to this file\n"
           "    -h                 This help\n");
    exit(1);
}

/* ============================================================================================= */
SCODE genPIDfile(char *szPidFile)
{
    FILE *pid_fp = fopen(szPidFile, "w");

    if (pid_fp == NULL) {
        fprintf(stderr, "Can't open %s\n", szPidFile);
        return S_FAIL;
    } else {
        fprintf(pid_fp, "%d\n", getpid());
        fchmod(fileno(pid_fp), S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
        fclose(pid_fp);
    }
    return S_OK;
}

/* ============================================================================================= */
void delPIDfile(char *szPidFile)
{
    FILE *pid_fp = fopen(szPidFile, "r");
    int   iStoredPid;
    if (pid_fp == NULL) {
        fprintf(stderr, "Can't open %s\n", szPidFile);
    }
    else {
        fscanf (pid_fp, "%d\n", &iStoredPid);
        fclose(pid_fp);
        if (iStoredPid == getpid()) {
            remove(szPidFile);
        }
    }
}

/* ============================================================================================= */
void Venc_HANDLE_SIGNAL(int signum)
{
    switch(signum) {
        case SIGHUP:
                Venc_LoadConfig(hVencObject);
                break;
        case SIGTERM:
		case SIGINT:        	
				g_bTerminate = TRUE;
                break;
		//case SIGFPE:
			//	printf("Caught signal %d (%s)\n", signum, "SIGFPE");
				//sleep(1);
				//break;
        default:
                break;
    }
    return;
}

/* ============================================================================================= */
void Venc_Init_Signals(void)
{
    struct sigaction sigaInst;
	sigemptyset(&sigaInst.sa_mask);
    sigaInst.sa_flags = SA_RESTART;
    

    sigaddset(&sigaInst.sa_mask, SIGTERM);
    sigaddset(&sigaInst.sa_mask, SIGINT);
    sigaddset(&sigaInst.sa_mask, SIGHUP);
	sigaddset(&sigaInst.sa_mask, SIGFPE);
	
    sigaInst.sa_handler = Venc_HANDLE_SIGNAL;
    sigaction(SIGTERM, &sigaInst, NULL);
    sigaction(SIGINT, &sigaInst, NULL);
    sigaction(SIGHUP, &sigaInst, NULL);
	sigaction(SIGFPE, &sigaInst, NULL);
}

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

static void daemonize(void)
{
    pid_t pid, sid;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* At this point we are executing as the child process */

    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory.  This prevents the current
       directory from being locked; hence not being able to remove it. */
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    /* Redirect standard files to /dev/null */
    freopen( "/dev/null", "r", stdin);
    freopen( "/dev/null", "w", stdout);
    freopen( "/dev/null", "w", stderr);
}



/* ============================================================================================= */
int main(int argc, char *argv[])
{
	static CHAR* szPidFile = NULL;
    BOOL bIsDaemon = FALSE;
    SCODE scRes = S_FAIL;	
    int ch = -1;
    TVencInitOpt tVencInitOpt;
	
	tVencInitOpt.szConfigFile = NULL;
    tVencInitOpt.szCommandFIFO = NULL;
	tVencInitOpt.dwDevNum = -1;
	
	
	//memset(&tVencInitOpt, 0, sizeof(TVencInitOpt));
    while ((ch = getopt(argc, argv, "m:Dc:f:p:h")) != -1) {
        switch(ch) {
            case 'm':
                tVencInitOpt.dwDevNum = atoi(optarg);
                break;
            case 'D':
                bIsDaemon = TRUE;
                break;
            case 'c':
                tVencInitOpt.szConfigFile = strdup(optarg);
                break;
            case 'f':
                tVencInitOpt.szCommandFIFO = strdup(optarg);
                break;
            case 'p':
                szPidFile = strdup(optarg);
                break;
            default:
                print_usage();
                exit(1);
        }
    }

    // run as Daemon
    if (bIsDaemon)
    {
      daemonize();
    }
     // daemon(0, 0);

    syslog(LOG_INFO, "[VENC] Start venc process with Pid : %d\n", getpid());
    syslog(LOG_INFO,"[VENC]Highest static priority (FIFO,RR,OTHER)=(%d,%d,%d)\n",sched_get_priority_max(SCHED_FIFO) \
					   ,sched_get_priority_max(SCHED_RR) \
					   ,sched_get_priority_max(SCHED_OTHER) \
					   );
	// patch by Acer.Tsai 2011/07/21 - set venc to highest priority 
	{
		struct sched_param sp = { .sched_priority = 50 };
		int ret;

		ret = sched_setscheduler(0, SCHED_FIFO, &sp);
		if (ret == -1) {
			perror("sched_setscheduler");
			return 1;
		}
	}
    // end of patch
     
    // If the szPidFile is assigned write pid to szPidFile 
    if (szPidFile != NULL) {
        if (genPIDfile(szPidFile) != S_OK) {
            fprintf(stderr, "[VENC] Generate pid file fail !!\n");
            //exit(1);
	    	goto exit;
        }
    }

    scRes = Venc_Initial(&hVencObject, tVencInitOpt);

    if (scRes != S_OK) {
        fprintf(stderr, "[VENC] Initial venc process err !!\n");
        //exit(1);
		goto exit;
    }

	Venc_Init_Signals();
	
	Venc_Start(hVencObject);

	// after catch SIGINT or SIGTERM
	Venc_Stop(hVencObject);	
	Venc_Release(&hVencObject);
exit:
    if (szPidFile != NULL) {
        delPIDfile(szPidFile);
    }
    if (tVencInitOpt.szConfigFile != NULL)
    {
		free(tVencInitOpt.szConfigFile);
		tVencInitOpt.szConfigFile = NULL;
    }
    if (tVencInitOpt.szCommandFIFO != NULL)
    {
		free(tVencInitOpt.szCommandFIFO);
		tVencInitOpt.szCommandFIFO = NULL;
    }
    if (szPidFile != NULL)
    {
		free(szPidFile);
		szPidFile = NULL;
    }
    if (bIsDaemon)
    {
		syslog(LOG_INFO, "[VENC] Stop Venc process !!\n");
    }
    else
    {
		printf("[VENC](%d) Stop Venc process !!\n",__LINE__);
    }
    return 0;
}

