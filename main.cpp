#include <iostream>
#include <vector>
#include <string>
#include <fstream> // For file operations

// Include nlohmann/json library

#include "json.hpp"

// Use a namespace alias for convenience
using json = nlohmann::json;

// Khai báo struct Task
struct Task {
    int id;                     // Mã số công việc
    std::string description;    // Mô tả công việc
    bool isDone;                // Trạng thái: đã hoàn thành hay chưa
};

// --- JSON Serialization/Deserialization for Task struct ---
// These functions allow nlohmann/json to convert Task objects to/from JSON.

// Function to serialize a Task object to JSON
void to_json(json& j, const Task& t) {
    j = json{{"id", t.id}, {"description", t.description}, {"isDone", t.isDone}};
}

// Function to deserialize a JSON object to a Task object
void from_json(const json& j, Task& t) {
    j.at("id").get_to(t.id);
    j.at("description").get_to(t.description);
    j.at("isDone").get_to(t.isDone);
}

// --- End JSON Serialization/Deserialization for Task struct ---


// Hàm hiển thị tất cả công việc
void displayTasks(const std::vector<Task>& tasks) {
    std::cout << "\n===== DANH SACH CONG VIEC =====\n";
    if (tasks.empty()) {
        std::cout << "Khong co cong viec nao.\n";
    } else {
        for (const Task& task : tasks) {
            std::cout << "ID: " << task.id
                      << " | Mo ta: " << task.description
                      << " | Trang thai: " << (task.isDone ? "Da hoan thanh" : "Chua xong")
                      << std::endl;
        }
    }
}

// Hàm thêm công việc mới
void addTask(std::vector<Task>& tasks, int& nextId) {
    std::cin.ignore(); // Xoá bộ nhớ đệm
    Task newTask;
    newTask.id = nextId++;
    std::cout << "Nhap mo ta cong viec: ";
    std::getline(std::cin, newTask.description);
    newTask.isDone = false;
    tasks.push_back(newTask);
    std::cout << "Da them cong viec moi!\n";
}

// Hàm đánh dấu hoàn thành
void markTaskDone(std::vector<Task>& tasks) {
    int id;
    std::cout << "Nhap ID cong viec can danh dau: ";
    std::cin >> id;
    bool found = false;
    for (Task& task : tasks) {
        if (task.id == id) {
            task.isDone = true;
            std::cout << "Da danh dau hoan thanh!\n";
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Khong tim thay cong viec co ID nay!\n";
    }
}

// Hàm xoá công việc
void deleteTask(std::vector<Task>& tasks) {
    int id;
    std::cout << "Nhap ID cong viec can xoa: ";
    std::cin >> id;
    bool found = false;
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->id == id) {
            tasks.erase(it);
            std::cout << "Da xoa cong viec!\n";
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Khong tim thay cong viec co ID nay!\n";
    }
}

// Hàm lưu công việc vào file JSON
void saveTasksToFile(const std::vector<Task>& tasks, const std::string& filename) {
    json j = tasks; // Convert vector of Task to JSON array
    std::ofstream o(filename);
    if (o.is_open()) {
        o << std::setw(4) << j << std::endl; // Pretty print with 4 spaces indentation
        std::cout << "Da luu cong viec vao file '" << filename << "'\n";
    } else {
        std::cerr << "Loi: Khong the mo file de luu '" << filename << "'\n";
    }
}

// Hàm đọc công việc từ file JSON
void loadTasksFromFile(std::vector<Task>& tasks, int& nextId, const std::string& filename) {
    std::ifstream i(filename);
    if (i.is_open()) {
        try {
            json j;
            i >> j; // Read JSON from file
            tasks = j.get<std::vector<Task>>(); // Convert JSON array to vector of Task

            // Find the maximum ID to set nextId correctly
            nextId = 1; // Reset nextId in case there are no tasks
            if (!tasks.empty()) {
                int maxId = 0;
                for (const auto& task : tasks) {
                    if (task.id > maxId) {
                        maxId = task.id;
                    }
                }
                nextId = maxId + 1;
            }
            std::cout << "Da doc cong viec tu file '" << filename << "'\n";
        } catch (const json::parse_error& e) {
            std::cerr << "Loi phan tich JSON tu file '" << filename << "': " << e.what() << std::endl;
            tasks.clear(); // Clear any partially loaded data
            nextId = 1; // Reset ID counter
        }
    } else {
        std::cout << "Khong tim thay file '" << filename << "'. Tao moi danh sach.\n";
    }
}

int main() {
    std::vector<Task> tasks; // Vector lưu danh sách công việc
    int nextId = 1;          // ID sẽ tự tăng
    const std::string filename = "tasks.json"; // Tên file JSON
    int choice;

    // Load tasks at the beginning of the program
    loadTasksFromFile(tasks, nextId, filename);

    do {
        std::cout << "\n===== MENU TODO LIST =====\n";
        std::cout << "1. Them cong viec\n";
        std::cout << "2. Hien thi cong viec\n";
        std::cout << "3. Danh dau cong viec hoan thanh\n";
        std::cout << "4. Xoa cong viec\n";
        std::cout << "5. Luu cong viec\n"; // New option
        std::cout << "6. Thoat\n";         // Changed option number
        std::cout << "Lua chon cua ban: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                addTask(tasks, nextId);
                break;
            case 2:
                displayTasks(tasks);
                break;
            case 3:
                markTaskDone(tasks);
                break;
            case 4:
                deleteTask(tasks);
                break;
            case 5: // New case for saving
                saveTasksToFile(tasks, filename);
                break;
            case 6: // Changed exit option
                saveTasksToFile(tasks, filename); // Save before exiting
                std::cout << "Tam biet!\n";
                break;
            default:
                std::cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 6); // Changed loop condition

    return 0;
}
