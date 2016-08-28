#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <armadillo>

#include <cctype>
#include <string>
#include <algorithm>
#include <regex>

inline std::string trim(const std::string &s)
{
	auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
	auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
	return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}

std::vector<std::string> split(const std::string& input, const std::string& regex)
{
	// passing -1 as the submatch index parameter performs splitting
	std::regex re(regex);
	std::sregex_token_iterator
		first{input.begin(), input.end(), re, -1},
		last;
	return {first, last};
}

int main(int argc, char* argv[])
{
  if(3 != argc)
  {
    std::cerr << "Invalid usage." << std::endl;
    return -1;
  }
  std::ifstream graph_file(argv[1]);
  if(!graph_file.is_open())
  {
    std::cerr << "Failed to open file containing graph." << std::endl;
    return -1;
  }

  std::map<unsigned, std::set<unsigned>> connections;
  std::string line;
	std::set<unsigned> nodes;
	std::size_t node_count = 0;
  while(std::getline(graph_file, line))
  {
		line = trim(line); 
		if(line.empty())
		{
			continue;
		}
		if(std::string::npos != line.find_first_of('='))
		{
			std::vector<std::string> tokens = split(line, "=");
			if(tokens.size() != 2)
			{
				continue;
			}
			for(auto& i : tokens)
			{
				i = trim(i);
			}
			if(tokens[0] == "N")
			{
				node_count = stoi(tokens[1]);
			}
			continue;
		}
		if("-" == line)
		{
			continue;
		}
    std::stringstream s(line);
    unsigned node1;
    unsigned node2;
    s >> node1 >> node2;
		nodes.insert(node1);
		nodes.insert(node2);
		connections[node1].insert(node2);
		connections[node2].insert(node1);
  }
  graph_file.close();
	std::size_t s = 0 != node_count ? node_count : nodes.size();

  arma::mat m(s, s, arma::fill::zeros);
  for(auto i = connections.cbegin(); i != connections.cend(); ++i)
  {
    auto i_connections = i->second;
    for(auto j = i_connections.cbegin(); j != i_connections.cend(); ++j)
    {
      m(i->first, *j) = 1;
      m(*j, i->first) = 1;
    }
  }

  arma::vec eigval = arma::eig_sym(m);

  std::ofstream fout(argv[2]);
  if(!fout.is_open())
  {
    std::cout << "Cannot open output file."  << std::endl;
    return -1;
  }
  const arma::vec& eig_val = arma::eig_sym(m);
  fout << eig_val;
  fout.close();

  return 0;
}
