#include <iostream>
#include <chrono>

#include "planner.hpp"
#include "dotwriter.hpp"
#include "io.hpp"
#include "argparse.hpp"

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("Assembly Planner");

    program.add_argument("Filename")
        .help("Path to the XML assembly description");
    program.add_argument("-v", "--verbose")
        .help("Print debug output")
        .default_value(false)
        .implicit_value(true);

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

    std::cout << "+---------------------------------------------------+\n";
    std::cout << "|                 ASSEMBLY PLANNER                  |\n";
    std::cout << "+---------------------------------------------------+\n\n";

    // Assembly Planning Block.
    auto t1 = std::chrono::high_resolution_clock::now();

    Graph<AssemblyData,EdgeData> assembly;
    InputReader rdr;
    config::Configuration config;
    bool result;

    std::tie(assembly, config, result) = rdr.read(input);

    if (!result)
    {
        std::cout << "ERROR: Could not read " << input << std::endl;
        return false;
    }
    
    if(program.get<bool>("--verbose"))
        std::cout << config << std::endl;

    Planner planner;
    auto assembly_plan = planner(assembly, config);

    rdr.write(assembly_plan, "asd");

    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "Duration: "<< duration << "ms.";

    return 0;
}