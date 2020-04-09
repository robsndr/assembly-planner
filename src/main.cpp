#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>

#include "planner.hpp"
#include "dotwriter.hpp"
#include "input_reader.hpp"
#include "argparse.hpp"

int main(int argc, char *argv[])
{
    auto t1 = std::chrono::high_resolution_clock::now();
    argparse::ArgumentParser program("MSRM Assembly Planner");
    program.add_argument("Filename")
        .help("Path to the XML assembly description.");

    // Parse Input Block
    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err)
    {
        std::cout << err.what() << std::endl;
        std::cout << program;
        exit(0);
    }

    auto input = program.get<std::string>("Filename");

    // Assembly Planning Block.
    Graph<AssemblyData,EdgeData> assembly;
 
    InputReader rdr;
    config::Configuration config;
    bool result;

    std::tie(assembly, config, result) = rdr.read(input);

    if (!result)
    {
        std::cout << "ERROR: Could not read " << input << " file" << std::endl;
        return false;
    }
    
    std::cout << config << std::endl;
    Planner planner;
    auto assembly_plan = planner(assembly, config);


    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "Duration: "<< duration << "ms.";

    return 0;
}