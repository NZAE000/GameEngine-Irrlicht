#include "memoryviewer.hpp"


namespace MEMVIEWER {

std::size_t calculateNumLines(std::size_t size, std::size_t width)
{
    std::size_t lines { size/width };
    
    // Verify if size is multiple of width (exact division):
        /*
            As the value 16 in binary is 00010000, and all multipe of 16 are 32, 48, 64, ...
            so always the four first bits are 0: 32 = 00100000, 48 = 00110000, 64 = 01000000.
        */
    if (size & 0xF) ++lines; // Therefore, if any bits is 1 in the four first bits, the value is'nt multiple of 16.

    return lines;
}

void showMemLine(uint8_t const* ptr, std::size_t width)
{
    std::printf("%16p | ", (void*)ptr);
    uint8_t const* ptr2 { ptr };

// Show values
    for (std::size_t i{}; i<width; ++i) {
        std::printf("%02X ", *ptr2);
        ++ptr2;
    } std::printf("| ");

// Show values as characters
    ptr2 = ptr;  char c{};
    for (std::size_t i{}; i<width; ++i) {
        c = (*ptr2 > 31 && *ptr2 < 128)? *ptr2 : '.'; // If values isn't printable, print '.'.
        std::printf("%c", c);
        ++ptr2;
    } std::printf("\n");

}

void showMemory(uint8_t const* ptr, std::size_t size, std::size_t width)
{
    std::size_t numLines { calculateNumLines(size, width) };

    std::printf("     ADDRESS     | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
    std::printf("-----------------|------------------------------------------------\n");
    
    for (std::size_t i{}; i<numLines; ++i) {
        showMemLine(ptr, width);
        ptr += width;
    }
    std::printf("------------------------------------------------------------------\n\n");
}

} // namespace VIEWER {