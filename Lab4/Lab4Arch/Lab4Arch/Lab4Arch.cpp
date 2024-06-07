#include <iostream>
#include <filesystem>
#include <string>

using namespace std;
namespace fs = std::filesystem;

// Criterion ¹4 (Hint mode: supported): 
void PrintUsage() {
    cout << "Usage: utility_name source_directory destination_directory file_pattern" << endl;
    cout << "Example: utility_name C:/Users/PC/Lab4/FirstCatalog C:/Users/PC/Lab4/SecondCatalog *.txt" << endl;
}

// Criterion ¹6(The program takes file attributes into account (hidden, read-only, archive)): 
bool CheckFileAttributes(const fs::path& filePath) {
    fs::file_status fileStatus = fs::status(filePath);

    if ((fileStatus.permissions() & fs::perms::owner_write) == fs::perms::none) {
        cerr << "Error: File is read-only: " << filePath.filename().string() << endl;
        return false;
    }

    return true;
}

// Criterion ¹1(Work with the specified directory (passed as a utility parameter)): 
void MoveFiles(const fs::path& sourceDir, const fs::path& destDir, const string& fileName) {
    if (!fs::exists(sourceDir) || !fs::is_directory(sourceDir)) {
        cerr << "Error: Source directory does not exist or is not a directory." << endl;
        return;
    }

    if (!fs::exists(destDir) || !fs::is_directory(destDir)) {
        cerr << "Error: Destination directory does not exist or is not a directory." << endl;
        return;
    }

    bool fileMoved = false; 

    try {
        for (const auto& entry : fs::directory_iterator(sourceDir)) {
            if (fs::is_regular_file(entry.path()) && entry.path().filename().string() == fileName) {
                if (CheckFileAttributes(entry.path())) {
                    fs::rename(entry.path(), destDir / entry.path().filename());
                    cout << "Moved: " << entry.path().filename().string() << " to " << (destDir / entry.path().filename()).string() << endl;
                    fileMoved = true; 
                }
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        cerr << "Error: " << e.what() << endl;
    }

    if (!fileMoved) {
        cerr << "Error: File '" << fileName << "' not found or could not be moved." << endl;
    }
}

int main() {
    // Criterion ¹3(Parsing and interpreting command line parameters: variable number of parameters): 
    fs::path sourcePath = "C:/Users/PC/Lab4/FirstCatalog";
    fs::path destPath = "C:/Users/PC/Lab4/SecondCatalog";
    string fileName = "First catalog.txt"; // Criterion ¹2(Work with the specified files according to the template (for example, *.exe))
    string filePattern = "*.txt";

    fs::path sourceFilePath = sourcePath / fileName;
    fs::path destFilePath = destPath / fileName;

    cout << "Source Directory: " << sourcePath << endl;
    cout << "Destination Directory: " << destPath << endl;
    cout << "File Name: " << fileName << endl;

    MoveFiles(sourcePath, destPath, fileName);

    return 0; // Criterion ¹5(The program generates completion codes)
}
