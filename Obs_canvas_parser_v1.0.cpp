#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <filesystem>

// TODO: CLEAN UP REST OF THE MESS AND KEEP ONLY THE NECESSARY FILES.

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct TaskStats {
    int completed;
    int pending;
};

int countAllNeedleOccurrences(const std::string&, const std::unordered_set<std::string>&);

class Dimensions {
public:
    std::string name;
    int x, y, width, height;
    std::string text;
    Dimensions(std::string, int, int, int, int, std::string);   // constructor declaration
    void print_data() const;      // method declaration
    int sum() const;              // method declaration
};

class FileManager {
public:
    static bool createDirectories(const std::filesystem::path&);
    static bool createFile(const std::filesystem::path&);
    static bool writeToFile(const std::filesystem::path&, const std::string&, bool append);
};



int main(int argc, char** argv){
    std::cout << "[INFO] Parsing canvas file...\n";
    std::ifstream f(argv[1]);
    if(!f){
        std::cerr << "Error opening file\n";
        return 1;
    }
    //Dimensions val(2, 4, 5, 7);
    //val.print_data();

    /*
    We need to make two structs, one for keeping the groups and another to keep all the cards in.
    Each struct will have MyData... wait... we do not need structs then...
    We can do the work with two vectors.
    We will keep all the separate MyData vectors separate, check dimensions of one from another, and gain insights about them...
    */


    std::unordered_set<std::string> months_names = {
        "January", "February", "March",    "April",
        "May",     "June",     "July",     "August",
        "September","October", "November", "December"
    };

    json cv = json::parse(f);

    std::vector<Dimensions> groups = {};
    std::vector<Dimensions> cards = {};
    std::map<Dimensions*, std::vector<Dimensions*>> group_of_cards;

    for (auto& node : cv["nodes"]){

        auto& type = node["type"];
        std::string month_name = node.value("label", "");

        int x = node["x"];
        int y = node["y"];
        int w = node["width"];
        int h = node["height"];
        std::string content = node.value("text", "");

        // construction of groups
        if(months_names.count(month_name) && type == "group"){
            groups.emplace_back(month_name, x, y, w, h, "");
        }

        // construction of cards
        if(type == "text"){
            cards.emplace_back("card", x, y, w, h, content);
        }
    }

    // put cards in groups found...
    for(Dimensions& group : groups){
        group_of_cards[&group] = {};
    }

    for(Dimensions& card : cards){
        for(Dimensions& group : groups){
            bool is_inside =
                card.x >= group.x &&
                card.y >= group.y &&
                (card.x + card.width) <= (group.x + group.width) &&
                (card.y + card.height) <= (group.y + group.height);

            if(is_inside){
                group_of_cards[&group].push_back(&card);
            }
        }
    }

    std::map<std::string, TaskStats> total_check_uncheck_count;

    for (const auto& [group_ptr, card_list] : group_of_cards) {
        std::string group_name = group_ptr->name;
        int checks_count = 0, unchecks_count = 0;
        for (const Dimensions* card_ptr : card_list) {
            std::string temp_content = card_ptr->text;

            std::unordered_set<std::string> checked_patt = {
                ". [X] ", "- [X] ",
                ". [x] ", "- [x] "
            };
            std::unordered_set<std::string> unchecked_patt = {
                ". [ ] ", "- [ ] "
            };

            checks_count += countAllNeedleOccurrences(temp_content, checked_patt);
            unchecks_count += countAllNeedleOccurrences(temp_content, unchecked_patt);
            if(checks_count || unchecks_count){
                total_check_uncheck_count[group_name].completed = checks_count;
                total_check_uncheck_count[group_name].pending = unchecks_count;
            }
        }
    }

    std::filesystem::path path = argv[2];
    if (FileManager::createFile(path)) {
        std::cout << "File created successfully!\n";
        std::string create_string = "";

        FileManager::writeToFile(path, "# Stats of This Year \n\n", false); // Overwrite the file. Append is false
        int total_checks = 0, total_unchecks = 0, total_of_year = 0;

        for(const auto& month : months_names){
            auto it = total_check_uncheck_count.find(month);
            if(it != total_check_uncheck_count.end()){
                TaskStats& values = it->second;

                int done   = values.completed;
                int todo   = values.pending;
                int total  = done + todo;
                double pct = total ? (100.0 * done / total) : 0.0;
                total_checks += done;
                total_unchecks += todo;
                total_of_year += done + todo;

                create_string +=
                    "## " + month + "\n\n"
                    "Completed\t\t: " + std::to_string(done)   + "\n"
                    "Pending\t\t\t: " + std::to_string(todo)   + "\n"
                    "Total\t\t\t\t: " + std::to_string(total)    + "\n"
                    "**Percent Done\t: " + std::to_string(pct) + "%**\n\n"
                    "---\n";
            }
        }
        create_string +=
            "#### Yearly Completed : " + std::to_string(total_checks) + "\n"
            "#### Yearly Pending   : " + std::to_string(total_unchecks) + "\n"
            "#### Yearly Total     : " + std::to_string(total_of_year) + "\n"
            "#### Yearly Percentage: " + std::to_string(total_of_year? (100.0 * total_checks / total_of_year) : 0.0) + "\n---\n";
        FileManager::writeToFile(path, create_string, true);
        std::cout << "\nFile Data modified successfully!" << std::endl;
    } else {
        std::cout << "File creation failed.\n";
    }
    return 0;
}


Dimensions::Dimensions(std::string _name, int _a, int _b, int _c, int _d, std::string _text)
    : name(_name), x(_a), y(_b), width(_c), height(_d), text(_text) {}

void Dimensions::print_data() const {
    std::cout << "name: " << name << "\n"
              << "Coordinates { a=" << x
              << ", b=" << y
              << ", c=" << x+width
              << ", d=" << y+height << " }\n"
              << "content: {\n" << text << "\n}\n\n";
}

int Dimensions::sum() const {
    return x + y + width + height;
}

int countAllNeedleOccurrences(const std::string& haystack, const std::unordered_set<std::string>& needles) {
    int totalCount = 0;

    for (const auto& needle : needles) {
        if (needle.empty()) continue; // avoid infinite loop

        size_t pos = 0;
        while ((pos = haystack.find(needle, pos)) != std::string::npos) {
            ++totalCount;
            ++pos; // shift forward by 1 to allow overlapping matches
        }
    }

    return totalCount;
}


// ===== FileManager Method Definitions =====

bool FileManager::createDirectories(const std::filesystem::path& dirPath) {
    try {
        if (!std::filesystem::exists(dirPath)) {
            return std::filesystem::create_directories(dirPath);
        }
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error creating directories: " << e.what() << "\n";
        return false;
    }
}

bool FileManager::createFile(const std::filesystem::path& filePath) {
    try {
        std::filesystem::path parentDir = filePath.parent_path();
        if (!createDirectories(parentDir)) return false;

        std::ofstream ofs(filePath);
        if (!ofs) {
            std::cerr << "Error creating file: " << filePath << "\n";
            return false;
        }
        ofs << "";
        ofs.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception while creating file: " << e.what() << "\n";
        return false;
    }
}

bool FileManager::writeToFile(const std::filesystem::path& filePath, const std::string& content, bool append) {
    std::ios_base::openmode mode = std::ios::out;
    if (append) mode |= std::ios::app;

    std::ofstream out(filePath, mode);
    if (!out) {
        std::cerr << "Failed to open file for writing: " << filePath << "\n";
        return false;
    }

    out << content;
    out.close();
    return true;
}
