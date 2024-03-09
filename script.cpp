//Made by BitZ 03/09/2024
//Please if you are going to take something from here
//At least make sure to leave credits, please and thank you.

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <iomanip> 
#include <sstream> 


void eliminarEspacios(std::string &str) {
    str.erase(std::remove_if(str.begin(), str.end(), [](unsigned char x) { return std::isspace(x); }), str.end());
}

// Función para eliminar todos los ';' de una cadena
void eliminarPuntoYComa(std::string &str) {
    str.erase(std::remove_if(str.begin(), str.end(), [](char c) { return c == ';'; }), str.end());
}

// Función para formatear la dirección
std::string formatearDireccion(const std::string &direccion) {

    unsigned int dirEntero = std::stoi(direccion, nullptr, 16);

    std::stringstream ss;

    ss << std::hex << std::uppercase << std::setfill('0') << std::setw(6) << dirEntero;

    return ss.str();
}

int main() {
    std::ifstream inputFile("GeometryDash.bro");
    std::ofstream outputFile("IDAnames.txt");

    if (!inputFile.is_open()) {
        std::cerr << "The file \"GeometryDash.bro\" was not found, please make sure to add it inside the folder and try again" << std::endl;
        std::cin.get(); 
        return 1;
    }

    if (!outputFile.is_open()) {
        std::cerr << "No se pudo crear el archivo de salida." << std::endl;
        return 1;
    }

    std::string linea;
    std::string nombre_clase;

    outputFile << "auto base = get_imagebase();" << std::endl;

    while (std::getline(inputFile, linea)) {
       
        std::size_t found_class = linea.find("class");
        if (found_class != std::string::npos) {
            std::size_t end_class_name = linea.find(" :");
            if (end_class_name != std::string::npos) {
                nombre_clase = linea.substr(found_class + 5, end_class_name - found_class - 5); 
                eliminarEspacios(nombre_clase); 
            }
        }

       
        std::size_t found_equal = linea.find("= win ");
        if (found_equal != std::string::npos) {
            std::size_t pos = linea.find("("); 
            if (pos != std::string::npos) {
                std::size_t start_pos = linea.rfind(" ", pos); 
                std::string nombre_metodo = linea.substr(start_pos + 1, pos - start_pos - 1); 
                eliminarEspacios(nombre_metodo);
                eliminarPuntoYComa(nombre_metodo);
                std::string direccion = linea.substr(found_equal + 6); 
                if (!nombre_clase.empty()) {
                    outputFile << "set_name(base + 0x" << formatearDireccion(direccion) << ", \"" << nombre_clase << "::" << nombre_metodo << "\");" << std::endl;
                } else {
                    outputFile << "set_name(base + 0x" << formatearDireccion(direccion) << ", \"" << nombre_metodo << "\");" << std::endl;
                }
            }
        }
    }

    std::cout << "Successfully generated \"IDAnames.txt\" file" << std::endl;
    std::cin.get(); 

    inputFile.close();
    outputFile.close();

    return 0;
}
