#pragma once

#include <string>
#include <unordered_map>

#include <ScarlEntt/World.h>
#include <ScarlEntt/EntityHandle.h>

#include "Components.h"

class TaskRegistry
{
public:
    inline static TaskRegistry* Instance() { return mInstance = (mInstance == nullptr ? new TaskRegistry : mInstance); }

    void Init();
    void Destroy();

    void RenderTasks();

    bool AddTaskUi();
    void ViewTask(unsigned int taskId);

    void RegisterTask(const std::string& title, int priority);

    inline ScarlEntt::Scene* GetScene() const { return mScene; }
private:
    TaskRegistry() = default;
    ~TaskRegistry() = default;

    static TaskRegistry* mInstance;

    ScarlEntt::World mWorld;
    ScarlEntt::Scene* mScene = mWorld.GetMainScene();

    std::unordered_map<unsigned int, ScarlEntt::EntityHandle> mTaskIdToEntityMap;

    unsigned int mMaxTaskNumber = 0;
    unsigned int mViewingTaskId = 0;

    void LoadTasksFromDisk();
    void SaveTasksToDisk();

    void CreateTaskFromStringLine(std::string line);
    void CreateTask(unsigned int id, const std::string& title, int priority, char status, const std::string& description);

    std::string GetStatusToString(CompletionStatus status);

    void UpdatePriorityComponent(ScarlEntt::EntityHandle& entHand, int priority, bool remove = false);
    void UpdateStatusComponent(ScarlEntt::EntityHandle& entHand, CompletionStatus status, bool remove = false);
};

