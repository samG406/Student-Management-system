#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <limits>

struct Student {
    int    id;
    std::string name;
    int    age;
    double grade;

    // serialize to binary file
    void writeTo(std::ofstream& out) const {
        out.write(reinterpret_cast<const char*>(&id),    sizeof(id));
        out.write(reinterpret_cast<const char*>(&age),   sizeof(age));
        out.write(reinterpret_cast<const char*>(&grade), sizeof(grade));
        size_t len = name.size();
        out.write(reinterpret_cast<const char*>(&len),   sizeof(len));
        out.write(name.data(), len);
    }

    // deserialize from binary file
    static Student readFrom(std::ifstream& in) {
        Student s;
        size_t len;
        in.read(reinterpret_cast<char*>(&s.id),    sizeof(s.id));
        in.read(reinterpret_cast<char*>(&s.age),   sizeof(s.age));
        in.read(reinterpret_cast<char*>(&s.grade), sizeof(s.grade));
        in.read(reinterpret_cast<char*>(&len),     sizeof(len));
        s.name.resize(len);
        in.read(&s.name[0], len);
        return s;
    }
};

class SMS {
    struct Node {
        Student data;
        Node *L = nullptr, *R = nullptr;
        Node(const Student& s): data(s) {}
    };
    Node* root = nullptr;

    Node* insert(Node* cur, const Student& s) {
        if (!cur) return new Node(s);
        if (s.id < cur->data.id)     cur->L = insert(cur->L, s);
        else if (s.id > cur->data.id) cur->R = insert(cur->R, s);
        return cur;
    }

    Node* findMin(Node* cur) {
        return cur->L ? findMin(cur->L) : cur;
    }

    Node* remove(Node* cur, int id) {
        if (!cur) return nullptr;
        if      (id < cur->data.id) cur->L = remove(cur->L, id);
        else if (id > cur->data.id) cur->R = remove(cur->R, id);
        else {
            if (!cur->L) { Node* r = cur->R; delete cur; return r; }
            if (!cur->R) { Node* l = cur->L; delete cur; return l; }
            Node* m = findMin(cur->R);
            cur->data = m->data;
            cur->R = remove(cur->R, m->data.id);
        }
        return cur;
    }

    Student* search(Node* cur, int id) {
        if (!cur) return nullptr;
        if (id < cur->data.id)     return search(cur->L, id);
        else if (id > cur->data.id) return search(cur->R, id);
        else return &cur->data;
    }

    void inOrder(Node* cur, std::vector<Student>& out) {
        if (!cur) return;
        inOrder(cur->L, out);
        out.push_back(cur->data);
        inOrder(cur->R, out);
    }

    void savePreOrder(Node* cur, std::ofstream& out) {
        if (!cur) return;
        cur->data.writeTo(out);
        savePreOrder(cur->L, out);
        savePreOrder(cur->R, out);
    }

    void destroy(Node* cur) {
        if (!cur) return;
        destroy(cur->L);
        destroy(cur->R);
        delete cur;
    }

public:
    ~SMS() { destroy(root); }

    void add(const Student& s)            { root = insert(root, s); }
    bool removeById(int id) {
        if (!search(root, id)) return false;
        root = remove(root, id);
        return true;
    }
    Student* findById(int id)             { return search(root, id); }

    std::vector<Student> getAll() {
        std::vector<Student> v;
        inOrder(root, v);
        return v;
    }

    void displaySortedById() {
        auto v = getAll();
        if (v.empty()) {
            std::cout << "\n-- No students found in the system --\n";
            return;
        }
        std::cout << "\n-- Students (by ID) --\n";
        for (auto& s : v)
            std::cout << s.id << ": " << s.name
                      << " | Age: " << s.age
                      << " | Grade: " << s.grade << "\n";
    }

    void displaySortedByGrade() {
        auto v = getAll();
        if (v.empty()) {
            std::cout << "\n-- No students found in the system --\n";
            return;
        }
        std::sort(v.begin(), v.end(),
            [](const Student& a, const Student& b) {
                return a.grade > b.grade;
            });
        std::cout << "\n-- Students (by Grade desc) --\n";
        for (auto& s : v)
            std::cout << s.grade << ": " << s.name
                      << " (ID " << s.id << ")\n";
    }

    void saveToFile(const std::string& fname) {
        std::ofstream out(fname, std::ios::binary);
        if (!out) throw std::runtime_error("Cannot open file for saving");
        savePreOrder(root, out);
    }

    void loadFromFile(const std::string& fname) {
        std::ifstream in(fname, std::ios::binary);
        if (!in) {
            std::cout << "No existing data file found. Starting fresh.\n";
            return;  
        }
        if (in.peek() == EOF) {
            std::cout << "Data file is empty. Starting fresh.\n";
            return;  
        }
        while (true) {
            try {
                if (in.peek() == EOF) break;  
                Student s = Student::readFrom(in);
                if (!in) break;  
                add(s);
            } catch (...) {
                break;  
            }
        }
    }    
};

int main(){
    SMS sms;
    const char* DATAFILE = "students.dat";
    sms.loadFromFile(DATAFILE);

    while (true) {
        std::cout << "\n\n--- Student Management ---\n"
                     "1) Add    2) Display by ID   3) Display by Grade\n"
                     "4) Search 5) Update         6) Delete\n"
                     "7) Save & Exit\n"
                     "Choose> ";
        int cmd;
        if (!(std::cin >> cmd)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (cmd == 1) {
            Student s;

           
        // ID
        std::cout << "Enter ID: ";
        bool validId = false;
        
        while (!validId) {
            while (!(std::cin >> s.id)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid ID. Please enter an integer: ";
            }
            
            // Checks if ID already exists
            if (sms.findById(s.id)) {
                std::cout << "Student ID already exists, enter a valid ID: ";
            } else {
                validId = true;
            }
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // Name
            std::cout << "Name: ";
            std::getline(std::cin, s.name);

            while (s.name.empty() || s.name.size() > 40) {
                std::cout << "Invalid name. Please enter a valid name: ";
                std::getline(std::cin, s.name);
            }
            // Age
            std::cout << "Age: ";
            bool validAge = false;
            while (!validAge) {
            while (!(std::cin >> s.age)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid age. Please enter an integer: ";
            }
            if (s.age <= 0 || s.age > 120) {
                std::cout << "Invalid age. Must be between 1 and 120: ";
            } else {
                validAge = true;
            }
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // Grade
            std::cout << "Grade: ";
            while (!(std::cin >> s.grade)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid grade. Please enter a number: ";
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            sms.add(s);
            std::cout << "Student added!\n";
        }
        else if (cmd == 2) {
            sms.displaySortedById();
        }
        else if (cmd == 3) {
            sms.displaySortedByGrade();
        }
        else if (cmd == 4) {
            int id;
            std::cout << "Search ID: ";
            std::cin >> id;
            if (auto p = sms.findById(id))
                std::cout << "Found: " << p->name
                          << " | Age " << p->age
                          << " | Grade " << p->grade << "\n";
            else
                std::cout << "No student with ID " << id << "\n";
        }
        else if (cmd == 5) {
            int id;
            std::cout << "Update ID: ";
            std::cin >> id;
            if (auto p = sms.findById(id)) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "New Name (blank to keep): ";
                std::string tmp; std::getline(std::cin, tmp);
                if (!tmp.empty()) p->name = tmp;

                std::cout << "New Age (0 to keep): ";
                int a; std::cin >> a;
                if (a > 0) p->age = a;

                std::cout << "New Grade (-1 to keep): ";
                double g; std::cin >> g;
                if (g >= 0) p->grade = g;
            } else {
                std::cout << "Not found\n";
            }
        }
        else if (cmd == 6) {
            int id;
            std::cout << "Delete ID: ";
            std::cin >> id;
            std::cout << (sms.removeById(id) ? "Deleted\n" : "Not found\n");
        }
        else if (cmd == 7) {
            sms.saveToFile(DATAFILE);
            std::cout << "Succesfully saved the file.\n";
            break;
        }
        else {
            std::cout << "Invalid option\n";
        }
    }

    return 0;
}
