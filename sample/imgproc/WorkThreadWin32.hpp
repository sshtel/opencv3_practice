#ifndef __WORK_THREAD_WIN32_H
#define __WORK_THREAD_WIN32_H

void setThreadUseCL(bool isCL);
bool isThreadUseCL();
int finishedTaskCount();

void sobel_thread_CL(void* pParams);
void video_thread_CL(void* pParams);

#endif