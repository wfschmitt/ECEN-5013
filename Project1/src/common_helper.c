/**
 * @brief 
 * 
 * @file common_helper.c
 * @author Gunj Manseta
 * @date 2018-03-10
 */

#include "common_helper.h"
#include "main_task.h"
#include "logger_task.h"



const char* const task_identifier_string[NUM_CHILD_THREADS+1] =
{
    (const char*)"Logger Task",
    (const char*)"Socket Task",
    (const char*)"Main Task",

};

mqd_t get_queue_handle(TASK_IDENTIFIER_T taskid)
{
    mqd_t queueHandle;
    switch(taskid)
    {
        case(MAIN_TASK_ID):
            queueHandle = getHandle_MainTaskQueue();
            break;
        case(LOGGER_TASK_ID):
            queueHandle = getHandle_LoggerTaskQueue();
            break;
        // case(SOCKET_TASK_ID):
        //     queueHandle = getHandle_SocketTaskQueue();
        //     break;
        default:
            queueHandle = 0;
            break;
    }

    return queueHandle;

}