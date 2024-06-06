#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <conio.h> 

void removeSpaces(std::string &str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
}

void removeSemicolons(std::string &str) {
    str.erase(std::remove(str.begin(), str.end(), ';'), str.end());
}

bool isValidHex(const std::string &str) {
    return str.size() > 2 && str.substr(0, 2) == "0x" &&
           std::all_of(str.begin() + 2, str.end(), ::isxdigit);
}

std::string formatAddress(const std::string &address) {
    try {
        unsigned long long intAddress = std::stoull(address, nullptr, 16);
        std::stringstream ss;
        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(6) << intAddress;
        return ss.str();
    } catch (const std::exception &) {
        return "000000";
    }
}

int main() {
    std::ifstream inputFile("GeometryDash.bro");
    std::ofstream outputFile("IDAnames.txt");

    if (!inputFile.is_open()) {
        std::cerr << "The file \"GeometryDash.bro\" was not found, please make sure to add it inside the folder and try again" << std::endl;
        getch(); 
        return 1;
    }

    if (!outputFile.is_open()) {
        std::cerr << "Could not create the output file." << std::endl;
        getch(); 
        return 1;
    }

    std::string line;
    std::string className;

    outputFile << "auto base = get_imagebase();" << std::endl;

    while (std::getline(inputFile, line)) {
        std::size_t found_class = line.find("class");
        if (found_class != std::string::npos) {
            std::size_t end_class_name = line.find(" :");
            if (end_class_name != std::string::npos) {
                className = line.substr(found_class + 5, end_class_name - found_class - 5);
                removeSpaces(className);
            }
        }

        std::size_t found_equal = line.find("= win ");
        if (found_equal != std::string::npos) {
            std::size_t pos = line.find("(");
            if (pos != std::string::npos) {
                std::size_t start_pos = line.rfind(" ", pos);
                std::string methodName = line.substr(start_pos + 1, pos - start_pos - 1);
                removeSpaces(methodName);
                removeSemicolons(methodName);

                std::string address = line.substr(found_equal + 6, line.find(",") - (found_equal + 6));
                removeSpaces(address);
                if (isValidHex(address)) {
                    std::string formattedAddress = formatAddress(address);
                    if (formattedAddress != "000000") {
                        if (!className.empty()) {
                            outputFile << "set_name(base + 0x" << formattedAddress << ", \"" << className << "::" << methodName << "\");" << std::endl;
                        } else {
                            outputFile << "set_name(base + 0x" << formattedAddress << ", \"" << methodName << "\");" << std::endl;
                        }
                    }
                }
            }
        }
    }

    std::cout << "Successfully generated \"IDAnames.txt\" file" << std::endl;
    std::cout << "Press any key to exit..." << std::endl;
    getch(); 

    inputFile.close();
    outputFile.close();

    return 0;
}
