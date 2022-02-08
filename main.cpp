#define _CRT_SECURE_NO_WARNINGS
#include "Layout_comparator.h"
#include "LayoutReader_MSK.hpp"
#include <iostream>


int main(int argc, char* argv[]) {
    LayoutData  layout;
    //freopen("log.txt", "w", stdout);
    //std::wstring fileName = L"tests/inv.gds";
    //std::wstring fileName = L"output.gds";
    //std::wstring fileName = L"tests/nand2.gds";
    //std::wstring fileName = L"tests/xor.gds";
    //std::wstring fileName = L"tests/1Kpolyg.gds";
    //std::wstring fileName = L"tests/testDesign.gds";
    std::wstring fileName = L"C:/microwind3 lite/carryCell_tst.MSK";

    LayoutReader* p_reader = GetReader(fileName);
    if (!p_reader) {
        std::wcerr << "__err__ : Can't file appropriate reader for given file '" << fileName << "'." << std::endl;
        return EXIT_FAILURE;
    }
    if (!p_reader->Read(&layout)) {
        std::wcerr << "__err__ : Can't read file '" << fileName << "'." << std::endl;
        FreeReader(p_reader);
        return EXIT_FAILURE;
    }
    FreeReader(p_reader);
    

    std::wcout << "Input file " << fileName << " has " << layout.libraries.size() << " library(ies)" << std::endl;

    for (size_t i = 0; i < layout.libraries.size(); ++i) {
       std::cout << "  - Library [" << i << "] has name '" << layout.libraries[i]->name << "' and contains " << layout.libraries[i]->elements.size() << " elements:" << std::endl;
       for (size_t j = 0; j < layout.libraries[i]->elements.size(); ++j)
           std::cout << "      * " << layout.libraries[i]->elements[j]->name << " (contains " << layout.libraries[i]->elements[j]->geometries.size() << " geometries)" << std::endl;
       std::cout << "    Library [" << i << "] also contains " << layout.libraries[i]->layers.size() << " layers (in order of appearance):" << std::endl;
       std::cout << "      { ";
       for (size_t j = 0; j < layout.libraries[i]->layers.size(); ++j)
           std::cout << layout.libraries[i]->layers[j].layer << " ";
       std::cout << " }" << std::endl;
   }


    LayoutBitmapGenerator generator;
    Coord leftTop{48,150}, rightBot{108,94};
    std::vector<int16_t> layers;
    //layers.push_back(1);
    layers.push_back(16);
    layers.push_back(17);
    layers.push_back(13);
    layers.push_back(19);
    layers.push_back(23);
    layers.push_back(27);
    layers.push_back(34);
    layers.push_back(36);
    layers.push_back(53);
    layers.push_back(55);
    generator.init(&layout, leftTop, rightBot, layers);
    generator.process(32, 32);
    /*
    LayoutWriter *p_writer = GetWriter(LayoutFileFormat::GDSII_bin);
    if (!p_writer) {
      std::cerr << "__err__ : Can't file appropriate writer for format  'gdsiibin'." << std::endl;
      return EXIT_FAILURE;
    }
    if (!p_writer->Write("output.gds", &layout)) {
      std::cerr << "__err__ : Can't write file '" << "output.gdsii" << "'." << std::endl;
      FreeWriter(p_writer);
      return EXIT_FAILURE;
    }
    FreeWriter(p_writer);
    //*/

    return EXIT_SUCCESS;
}