#include <iostream>
#include <chrono>

#include "planner.hpp"
#include "dotwriter.hpp"
#include "io.hpp"
#include "argparse.hpp"

int main(int argc, char *argv[])
{
    // Input handling
    argparse::ArgumentParser program("Assembly Planner");
    program.add_argument("input")
        .help("Path to the XML assembly description ")
        .required();
    program.add_argument("output")
        .help("Path to output assembly plan ")
        .required();
    program.add_argument("-d", "--dot")
        .help("Write output in graphviz format [].dot]")
        .default_value(false)
        .implicit_value(true);           
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
    auto input_path = program.get<std::string>("input");
    auto output_path = program.get<std::string>("output");

    std::cout << "+---------------------------------------------------+\n";
    std::cout << "|                 ASSEMBLY PLANNER                  |\n";
    std::cout << "+---------------------------------------------------+\n\n";

    // Assembly Planning Block.
    // Planning data structures
    Graph<AssemblyData,EdgeData> assembly;
    IoXml xml;
    config::Configuration config;
    bool result;

    // Read input XML
    std::tie(assembly, config, result) = xml.read(input_path);
    if (!result)
    {
        std::cout << "ERROR: Could not read " << input_path << std::endl;
        return false;
    }
    if(program.get<bool>("--verbose"))
        std::cout << config << std::endl;

    // Run planner
    Planner planner;
    auto assembly_plan = planner(assembly, config);

    // Output result
    if(program.get<bool>("--dot"))
    {
        DotWriter dot;
        dot.write(assembly_plan, output_path);
    }
    else
    {
        xml.write(assembly_plan, output_path);
    }

    std::cout << "+---------------------------------------------------+" << std::endl;
    return 0;
}