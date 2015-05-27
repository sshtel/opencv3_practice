#ifndef __WORK_THREAD_WIN32_H
#define __WORK_THREAD_WIN32_H

void display_thread(void* params);
void video_thread_CL(void* pParams);
void setThreadUseCL(bool isCL);
bool isThreadUseCL();

#endif