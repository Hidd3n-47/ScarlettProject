#pragma once

/***
 * Component for each task.
 * All Tasks will contain this component.
 * Contains the ID of the task.
 **/
struct Task
{
    unsigned int TaskId = 0;
};

/***
 * Component for each task's title.
 * All Tasks will contain this component.
 **/
struct TaskTitle
{
    std::string Title;
};

/***
 * Component for each task's priority.
 * All Tasks will contain this component.
 * Priority ranges from [1, 4] and is default to 4.
 * Priority 1 is highest, whilst 4 is lowest.
 **/
struct TaskPriority
{
    int Priority = 4;
};

/***
 * Component for Priority 1.
 **/
struct P1 { /* Empty Component */ };

/***
 * Component for Priority 2.
 **/
struct P2 { /* Empty Component */ };

/***
 * Component for Priority 3.
 **/
struct P3 { /* Empty Component */ };

/***
 * Component for Priority 4.
 **/
struct P4 { /* Empty Component */ };

/***
 * Enum to represent the status of the task.
 **/
enum class CompletionStatus
{
    Completed = 0,
    Removed = 1,
    Todo = 2
};

/***
 * Component for each task's status.
 * All Tasks will contain this component.
 * Task Status: Done / To-Do
 **/
struct TaskStatus
{
    CompletionStatus Status;
};

/***
 * Component for Status of Completed.
 **/
struct StatusCompleted { /* Empty Component */ };

/***
 * Component for Status of To-Do.
 **/
struct StatusTodo { /* Empty Component */ };

/***
 * Component for Status of Removed.
 **/
struct StatusRemoved { /* Empty Component */ };

/***
 * Component for each task's description.
 * All Tasks will contain this component.
 * This component has optional information about the task. By default, the description is "".
 **/
struct TaskDescription
{
    std::string Descrip;
};