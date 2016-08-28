#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <cmath>
#include <algorithm>

inline std::string trim(const std::string &s)
{
	auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
	auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
	return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}

int main(int argc, char* argv[])
{
  if(3 != argc)
  {
    std::cerr << "Invalid usage." << std::endl;
    return -1;
  }
  std::ifstream input(argv[1]);
  if(!input.is_open())
  {
    std::cerr << "Failed to open input file." << std::endl;
    return -1;
  }

  std::vector<double> values;
  std::string line;
  while(std::getline(input, line))
  {
    line = trim(line);
    values.push_back(stod(line));
  }
  input.close();

  std::vector<double> distances;
  for(std::size_t i = 0; i < values.size(); ++i)
  {
    for(std::size_t j = i+1; j < values.size(); ++j)
    {
      distances.push_back(std::abs(values[i]-values[j]));
    }
  }

  std::ofstream output(argv[2]);
  if(!output.is_open())
  {
    std::cout << "Cannot open output file."  << std::endl;
    return -1;
  }
  std::ostream_iterator<double> out_it(output, "\n");
  std::copy(distances.begin(), distances.end(), out_it);
  output.close();

  return 0;
}
