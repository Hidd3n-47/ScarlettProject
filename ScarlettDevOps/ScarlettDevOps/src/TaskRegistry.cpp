#include "TaskRegistry.h"

#include <array>
#include <fstream>
#include <imgui.h>
#include <ScarlEntt/EntityHandle.h>

#include "Color.h"
#include "Components.h"
#include "../../ScarlettEngine/deps/ImGui/imgui_internal.h"

TaskRegistry* TaskRegistry::mInstance = nullptr;

void TaskRegistry::Init()
{
    mScene->GetComponentManager()->RegisterComponent<Task>();
    mScene->GetComponentManager()->RegisterComponent<TaskPriority>();
    mScene->GetComponentManager()->RegisterComponent<P1>();
    mScene->GetComponentManager()->RegisterComponent<P2>();
    mScene->GetComponentManager()->RegisterComponent<P3>();
    mScene->GetComponentManager()->RegisterComponent<P4>();
    mScene->GetComponentManager()->RegisterComponent<TaskTitle>();
    mScene->GetComponentManager()->RegisterComponent<TaskStatus>();
    mScene->GetComponentManager()->RegisterComponent<StatusCompleted>();
    mScene->GetComponentManager()->RegisterComponent<StatusTodo>();
    mScene->GetComponentManager()->RegisterComponent<StatusRemoved>();
    mScene->GetComponentManager()->RegisterComponent<TaskDescription>();

    LoadTasksFromDisk();
}

void TaskRegistry::Destroy()
{
    SaveTasksToDisk();
}

void TaskRegistry::RenderTasks()
{
    ImGui::SeparatorText("");
    ImGui::PushStyleColor(ImGuiCol_ChildBg, Colors::bgSecondary.ImColor());

    static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
    if (ImGui::BeginTable("TaskView", 5, flags))
    {
        ImGui::TableSetupColumn("Task ID");
        ImGui::TableSetupColumn("P");
        ImGui::TableSetupColumn("Task");
        ImGui::TableSetupColumn("Status");
        ImGui::TableHeadersRow();
        ImGui::TableNextColumn();

        ScarlEntt::ComponentArray<Task>* tasks = mScene->GetComponentManager()->GetComponentArray<Task>();
        ScarlEntt::ComponentArray<TaskPriority>* tasksP = mScene->GetComponentManager()->GetComponentArray<TaskPriority>();
        ScarlEntt::ComponentArray<TaskTitle>* tasksTitles = mScene->GetComponentManager()->GetComponentArray<TaskTitle>();
        ScarlEntt::ComponentArray<TaskStatus>* tasksStatus = mScene->GetComponentManager()->GetComponentArray<TaskStatus>();

        for (unsigned int i = 0; i < tasks->GetComponentCount(); ++i)
        {
            ImGui::Text(std::to_string((*tasks)[i]->TaskId).c_str());
            ImGui::TableNextColumn();

            ImGui::Text(std::to_string((*tasksP)[i]->Priority).c_str());
            ImGui::TableNextColumn();

            ImGui::Text((*tasksTitles)[i]->Title.c_str());
            ImGui::TableNextColumn();

            ImGui::Text(GetStatusToString((*tasksStatus)[i]->Status).c_str());
            ImGui::TableNextColumn();

            ImGui::PushID(std::to_string(i).c_str());
            if (ImGui::Button("view") && mViewingTaskId == 0)
            {
                mViewingTaskId = (*tasks)[i]->TaskId;
            }
            ImGui::PopID();

            ImGui::TableNextColumn();
        }

        if(mViewingTaskId != 0)
        {
            ViewTask(mViewingTaskId);
        }

        ImGui::EndTable();
    }
    ImGui::PopStyleColor();
}

bool TaskRegistry::AddTaskUi()
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowSize(ImVec2(464, 440));
    ImGui::SetNextWindowPos(ImVec2(main_viewport->GetCenter().x, main_viewport->GetCenter().y), 0, { 0.5f, 0.5f });

    ImGui::PushStyleColor(ImGuiCol_TitleBg | ImGuiCol_TitleBgActive, Colors::bgSecondary.ImColor());
    ImGui::PushStyleColor(ImGuiCol_WindowBg, Colors::bgPrimary.ImColor());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::redColor.ImColor());
    ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors::bgSecondary.ImColor());

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding | ImGuiStyleVar_FrameBorderSize | ImGuiStyleVar_WindowPadding, { 5.0f, 5.0f });

    bool windowOpen = true;
    ImGui::Begin("Add Task", &windowOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    std::array<const char*, 4> priorities{ "1", "2", "3", "4" };
    static int selectedPriority = 3;

    ImGui::BeginColumns("Priority and Category", 2, ImGuiOldColumnFlags_NoBorder | ImGuiOldColumnFlags_NoResize);

    ImGui::Text("Priority: ");
    ImGui::SameLine();
    ImGui::PushItemWidth(65);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 3.0f, 3.0f });
    if (ImGui::BeginCombo("##Priority", priorities[selectedPriority]))
    {
        for (int i = 0; i < static_cast<int>(priorities.size()); ++i)
        {
            if(ImGui::Selectable(priorities[i], selectedPriority == i))
            {
                selectedPriority = i;
            }
        }

        ImGui::EndCombo();
    }
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

    ImGui::NextColumn();
    ImGui::SetColumnWidth(0, 164.0f);

    ImGui::Text("Category: ");
    ImGui::SameLine();
    static char buffer[20] = "";
    ImGui::InputText("##Category", buffer, 20);

    ImGui::EndColumns();

    ImGui::PushItemWidth(430);

    ImGui::Text("Title: ");

    static char bufferTitle[400] = "";
    ImGui::InputText("##Title", bufferTitle, 400);

    ImGui::Text("Description: ");

    static char bufferDescription[3000] = "";
    ImGui::InputTextMultiline("##Description", bufferDescription, 3000);

    ImGui::PopItemWidth();

    if(ImGui::Button("Create Task"))
    {
        RegisterTask(std::string(bufferTitle), selectedPriority + 1);
        windowOpen = false;
    }

    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(4);

    return !windowOpen;
}

void TaskRegistry::ViewTask(unsigned int taskId)
{
    ScarlEntt::EntityHandle entity = mTaskIdToEntityMap[taskId];

    TaskPriority* priority = nullptr;
    //TaskCategory
    TaskTitle* title = nullptr;
    TaskDescription* description = nullptr;
    TaskStatus* status = nullptr;

    entity.GetComponents(priority, title, description, status);

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowSize(ImVec2(464, 450));
    ImGui::SetNextWindowPos(ImVec2(main_viewport->GetCenter().x, main_viewport->GetCenter().y), 0, { 0.5f, 0.5f });

    ImGui::PushStyleColor(ImGuiCol_TitleBg | ImGuiCol_TitleBgActive, Colors::bgSecondary.ImColor());
    ImGui::PushStyleColor(ImGuiCol_WindowBg, Colors::bgPrimary.ImColor());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::redColor.ImColor());
    ImGui::PushStyleColor(ImGuiCol_FrameBg, Colors::bgSecondary.ImColor());

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding | ImGuiStyleVar_FrameBorderSize | ImGuiStyleVar_WindowPadding, { 5.0f, 5.0f });

    bool windowOpen = true;
    ImGui::Begin(("Edit Task: #" + std::to_string(taskId)).c_str(), &windowOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Status: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(110);

    std::array<const char*, 3> completion = { "Completed", "Removed", "To-Do" };
    int completionValue = (int)status->Status;
    if(ImGui::BeginCombo("##Status", completion[completionValue]))
    {
        for (int i = 0; i < static_cast<int>(completion.size()); ++i)
        {
            if (ImGui::Selectable(completion[i], completionValue == i))
            {
                UpdateStatusComponent(entity, status->Status, true);
                status->Status = (CompletionStatus)i;
                UpdateStatusComponent(entity, status->Status);
            }
        }

        ImGui::EndCombo();
    }

    std::array<const char*, 4> priorities{ "1", "2", "3", "4" };
    int selectedPriority = priority->Priority - 1;

    ImGui::BeginColumns("Priority and Category", 2, ImGuiOldColumnFlags_NoBorder | ImGuiOldColumnFlags_NoResize);

    ImGui::Text("Priority: ");
    ImGui::SameLine();
    ImGui::PushItemWidth(65);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 3.0f, 3.0f });
    if (ImGui::BeginCombo("##Priority", priorities[selectedPriority]))
    {
        for (int i = 0; i < static_cast<int>(priorities.size()); ++i)
        {
            if (ImGui::Selectable(priorities[i], selectedPriority == i))
            {
                UpdatePriorityComponent(entity, priority->Priority, true);
                priority->Priority = i + 1;
                UpdatePriorityComponent(entity, priority->Priority);
            }
        }

        ImGui::EndCombo();
    }
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

    ImGui::NextColumn();
    ImGui::SetColumnWidth(0, 164.0f);

    ImGui::Text("Category: ");
    ImGui::SameLine();
    static char buffer[20] = "";
    ImGui::InputText("##Category", buffer, 20);

    ImGui::EndColumns();

    ImGui::PushItemWidth(430);

    ImGui::Text("Title: ");

    static char bufferTitle[400] = "";
    strcpy(bufferTitle, title->Title.c_str());
    ImGui::InputText("##Title", bufferTitle, 400);
    title->Title = std::string(bufferTitle);

    ImGui::Text("Description: ");

    static char bufferDescription[3000] = "";
    strcpy(bufferTitle, description->Descrip.c_str());
    ImGui::InputTextMultiline("##Description", bufferDescription, 3000);
    description->Descrip = std::string(bufferDescription);

    ImGui::PopItemWidth();

    if (ImGui::Button("Save"))
    {
        //RegisterTask(std::string(bufferTitle), selectedPriority + 1);
    }

    ImGui::SameLine();

    if(ImGui::Button("Close"))
    {
        windowOpen = false;
    }

    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(4);

    if(!windowOpen)
    {
        mViewingTaskId = 0;
    }
}

void TaskRegistry::RegisterTask(const std::string& title, int priority)
{
    CreateTask(++mMaxTaskNumber, title, priority, 'T', "");
}

void TaskRegistry::LoadTasksFromDisk()
{
    std::ifstream fin;
    fin.open("Tasks.sdo");

    std::string line;
    while (std::getline(fin, line))
    {
        CreateTaskFromStringLine(line);
    }

    fin.close();
}

void TaskRegistry::SaveTasksToDisk()
{
    std::ofstream fout;
    fout.open("Tasks.sdo");
    
    ScarlEntt::ComponentArray<Task>* tasks = mScene->GetComponentManager()->GetComponentArray<Task>();

    for (ComponentId i = 0; i < tasks->GetComponentCount(); ++i)
    {
        ScarlEntt::EntityHandle entHandle = mTaskIdToEntityMap[(*tasks)[i]->TaskId];
        
        TaskPriority* priorityComponent = nullptr;
        TaskTitle* titleComponent = nullptr;
        TaskStatus* statusComponent = nullptr;
        TaskDescription* descriptionComponent = nullptr;

        entHandle.GetComponents(priorityComponent, titleComponent, statusComponent, descriptionComponent);
        
        // - Task ID.
        std::string line = std::to_string((*tasks)[i]->TaskId) + ",";

        // - Task Priority.
        line += std::to_string(priorityComponent->Priority) + ",";

        // - Task title.
        line += "\"" + titleComponent->Title + "\",";

        // - Task Status.
        line += GetStatusToString(statusComponent->Status).substr(0, 1) + ",";

        // - Task Description.
        line += "\"" + descriptionComponent->Descrip + "\"\n";
        
        fout << line;
    }

    fout.close();
}

// Todo: Look into factory pattern and if I can use the factory pattern to initialise the components.
void TaskRegistry::CreateTaskFromStringLine(std::string line)
{
    /*** - - - - - - - - - - - - - - - - - - - - - - - - - - -
    | Tasks Serialised in order of:
    | - Task ID
    | - Task Priority
    | - Task Title
    | - Task Status
    | - Task Description
    |
    | Exmample:
    | ---------
    | 123,4,"This is a title",T,"This is a description"
    | - - - - - - - - - - - - - - - - - - - - - - - - - - - **/
    const char DELIMITER = ',';

    size_t delimiterPos = line.find(DELIMITER);

    // - Task ID ==========================================
    unsigned int id = std::stoi(line.substr(0, delimiterPos));
    
    mMaxTaskNumber = std::max(mMaxTaskNumber, id);

    line = line.substr(delimiterPos + 1);

    // - Task Priority ====================================
    // Guaranteed to be 1 character.
    int priority = line[0] - '0';
    
    line = line.substr(line.find(DELIMITER) + 1);

    // - Task Title  ======================================
    int earlyOut = 0;
    delimiterPos = 0;
    do
    {
        delimiterPos = line.find(DELIMITER, delimiterPos + 1);
        
    }
    while (line[delimiterPos - 1] != '"' && earlyOut++ < 20);

    if (earlyOut == 20)
    {
        // Error! Could not find end of task title.
    }

    std::string title = line.substr(1, delimiterPos - 2);
    line = line.substr(delimiterPos + 1);

    // - Status  ==========================================
    // Guaranteed to be 1 character.
    char status = line[0];
    
    line = line.substr(line.find(DELIMITER) + 1);

    // - Description ======================================
    if (line[0] != '"' || line[line.size() - 1] != '"')
    {
        // Error! Cannot find the description.
    }
    std::string description = line.substr(1, line.length() - 2);

    CreateTask(id, title, priority, status, description);
}

void TaskRegistry::CreateTask(unsigned int id, const std::string& title, int priority, char status, const std::string& description)
{
    ScarlEntt::EntityHandle entHand = mScene->CreateEntityHandle();
    mTaskIdToEntityMap[id] = entHand; // Todo Christian could check to see that id is not in map.
    
    entHand.AddComponent<Task>()->TaskId = id;
    
    entHand.AddComponent<TaskTitle>()->Title = title;
    
    entHand.AddComponent<TaskPriority>()->Priority = priority;
    UpdatePriorityComponent(entHand, priority);
    
    TaskStatus* statusComponent = entHand.AddComponent<TaskStatus>();
    switch (status)
    {
    case 'C':
        statusComponent->Status = CompletionStatus::Completed;
        entHand.AddComponent<StatusCompleted>();
        break;
    case 'T':
        statusComponent->Status = CompletionStatus::Todo;
        entHand.AddComponent<StatusTodo>();
        break;
    case 'R':
        statusComponent->Status = CompletionStatus::Removed;
        entHand.AddComponent<StatusRemoved>();
        break;
    default:
        // error.
        break;
    }

    entHand.AddComponent<TaskDescription>()->Descrip = description;
}

std::string TaskRegistry::GetStatusToString(CompletionStatus status)
{
    switch (status)
    {
        case CompletionStatus::Completed:
            return "Completed";
        case CompletionStatus::Todo:
            return "To-Do";
        case CompletionStatus::Removed:
            return "Removed";
    }
    // Todo Christian error.
    return "?";
}

void TaskRegistry::UpdatePriorityComponent(ScarlEntt::EntityHandle& entHand, int priority, bool remove /* = false */)
{
    switch (priority)
    {
    case 1:
        if (remove)
        {
            entHand.RemoveComponent<P1>();
        }
        else
        {
            entHand.AddComponent<P1>();
        }
        break;
    case 2:
        if (remove)
        {
            entHand.RemoveComponent<P2>();
        }
        else
        {
            entHand.AddComponent<P2>();
        }
        break;
    case 3:
        if (remove)
        {
            entHand.RemoveComponent<P3>();
        }
        else
        {
            entHand.AddComponent<P3>();
        }
        break;
    case 4:
        if (remove)
        {
            entHand.RemoveComponent<P4>();
        }
        else
        {
            entHand.AddComponent<P4>();
        }
        break;
    }
}

void TaskRegistry::UpdateStatusComponent(ScarlEntt::EntityHandle& entHand, CompletionStatus status, bool remove)
{
    switch (status)
    {
    case CompletionStatus::Completed:
        if (remove)
        {
            entHand.RemoveComponent<StatusCompleted>();
        }
        else
        {
            entHand.AddComponent<StatusCompleted>();
        }
        break;
    case CompletionStatus::Removed:
        if (remove)
        {
            entHand.RemoveComponent<StatusRemoved>();
        }
        else
        {
            entHand.AddComponent<StatusRemoved>();
        }
        break;
    case CompletionStatus::Todo:
        if (remove)
        {
            entHand.RemoveComponent<StatusTodo>();
        }
        else
        {
            entHand.AddComponent<StatusTodo>();
        }
        break;
    }
}
