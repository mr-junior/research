#include <iostream>
#include "converter.h"

int main(int argc, char* argv[])
{
  po::options_description desc("Options");
  desc.add_options()
    ("help,h", "Print help message")
    ("input,i", po::value<std::pair<char,std::string>>()->required(), "Input graph in following format: {graph type[single character]} {file name}")
    ("output,o", po::value<std::pair<char,std::string>>()->required(), "Output graph in following format: {graph type[single character]} {file name}");

	std::pair<char, std::string> input;
	std::pair<char, std::string> output;

  po::variables_map vm;
  try
  {
    po::store(po::parse_command_line(argc_, argv_, desc), vm);
    if(vm.count("help"))
    {
      std::cout << "Graph Converter v1.0" << std::endl << desc << std::endl;
			return false;
    }
    if(vm.count("input"))
    {
      std::cout << "input = " << vm["input"].as<std::pair<char, std::string>>() << std::endl;
      input = vm["input"].as<std::pair<char, std::string>>();
    }
    if(vm.count("output"))
    {
      std::cout << "output = " << vm["output"].as<std::pair<char, std::string>>() << std::endl;
      output = vm["output"].as<std::pair<char, std::string>>();
    }
    po::notify(vm);
  }
  catch(po::error& e)
  {
    std::cerr << "\nError parsing command line: " << e.what() << std::endl << std::endl;
    std::cerr << desc << std::endl;
		return 0;
  }
	try
	{
		converter::from(input)->to(output);
	}
	catch(base_exception& e)
	{
		std::cerr << "Program terminated after throwing exception.\nError message: " << e.what() << std::endl;
	}
	return 0;
}
