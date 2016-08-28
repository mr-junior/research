#include <fstream>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <sstream>

int main(int argc, char* argv[])
{
  if(argc < 4)
  {
    std::cout << "Invalid usage" << std::endl;
    return -1;
  }

  std::ifstream graph(argv[1]);
  if(!graph.is_open())
  {
    std::cout << "error" << std::endl;
    return -1;
  }

  std::ifstream numbering(argv[2]);
  if(!numbering.is_open())
  {
    std::cout << "error" << std::endl;
    return -1;
  }

  std::list<std::pair<int, int>> connections;
  std::string line;
  while(std::getline(graph, line))
  {
    std::stringstream s(line);
    int node1;
    int node2;
    s >> node1 >> node2;
		connections.emplace_back(node1, node2);
  }

	std::map<int, int> mapping;
  while(std::getline(numbering, line))
  {
    if(!line.empty())
    {
      std::stringstream s(line);
      int num;
			int new_num;
      s >> num >> new_num;
		  mapping[num] = new_num;
    }
  }

  graph.close();
  numbering.close();

	std::ofstream fout(argv[3]);
	if(!fout.is_open())
	{
		std::cout << "Error!" << std::endl;
		return -1;
	}

	for(auto i = connections.begin(); i != connections.end(); ++i)
	{
		int v1 = i->first;
		int v2 = i->second;
		fout << mapping[v1] << " " << mapping[v2] << '\n';
	}
	fout.close();

  return 0;
}
