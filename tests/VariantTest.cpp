#include "util/Variant.hpp"

int main() {
    Variant<int, double, std::string> var1(10);
    std::cout << var1.get<int>() << std::endl; // Outputs: 10

    var1 = std::string("Hello, Variant!");
    std::cout << var1.get<std::string>() << std::endl; // Outputs: Hello, Variant!

    try {
        std::cout << var1.get<int>() << std::endl; // Throws std::bad_variant_access
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl; // Outputs: std::bad_variant_access
    }

    return 0;
}
