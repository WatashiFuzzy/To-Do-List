#include <iostream>
#include <vector>
#include <string>

// Khai báo struct Task
struct Task {
    int id;                     // Mã số công việc
    std::string description;    // Mô tả công việc
    bool isDone;                // Trạng thái: đã hoàn thành hay chưa
};

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
    for (Task& task : tasks) {
        if (task.id == id) {
            task.isDone = true;
            std::cout << "Da danh dau hoan thanh!\n";
            return;
        }
    }
    std::cout << "Khong tim thay cong viec co ID nay!\n";
}

// Hàm xoá công việc
void deleteTask(std::vector<Task>& tasks) {
    int id;
    std::cout << "Nhap ID cong viec can xoa: ";
    std::cin >> id;
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->id == id) {
            tasks.erase(it);
            std::cout << "Da xoa cong viec!\n";
            return;
        }
    }
    std::cout << "Khong tim thay cong viec co ID nay!\n";
}

int main() {
    std::vector<Task> tasks; // Vector lưu danh sách công việc
    int nextId = 1;          // ID sẽ tự tăng
    int choice;

    do {
        std::cout << "\n===== MENU TODO LIST =====\n";
        std::cout << "1. Them cong viec\n";
        std::cout << "2. Hien thi cong viec\n";
        std::cout << "3. Danh dau cong viec hoan thanh\n";
        std::cout << "4. Xoa cong viec\n";
        std::cout << "5. Thoat\n";
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
            case 5:
                std::cout << "Tam biet!\n";
                break;
            default:
                std::cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 5);

    return 0;
}

