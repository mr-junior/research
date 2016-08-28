#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>

#include <armadillo>

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

  std::ifstream clusters(argv[2]);
  if(!clusters.is_open())
  {
    std::cout << "error" << std::endl;
    return -1;
  }

  std::map<int, std::set<int>> connections;
  std::string line;
  while(std::getline(graph, line))
  {
    std::stringstream s(line);
    int node1;
    int node2;
    s >> node1;
    s >> node2;
    connections[node1].insert(node2);
    connections[node2].insert(node1);
  }

	std::map<int, int> node_in_cluster;
  while(std::getline(clusters, line))
  {
    if(!line.empty())
    {
      std::stringstream s(line);
      int cluster_index;
      int node;
      s >> node >> cluster_index;
			node_in_cluster[node] = cluster_index;
    }
  }

  graph.close();
  clusters.close();

  std::map<int, std::set<int>> sec_level;

	for(auto i = connections.begin(); i != connections.end(); ++i)
	{
		int source = i->first;
		int source_cluster = node_in_cluster[source];
		for(auto j = i->second.begin(); j != i->second.end(); ++j)
		{
			int target = *j;
			int target_cluster = node_in_cluster[target];
			if(source_cluster != target_cluster)
			{
				sec_level[source_cluster].insert(target_cluster);
				sec_level[target_cluster].insert(source_cluster);
			}
		}
	}

	std::map<int, int> distrib;
	for(auto i = sec_level.begin(); i != sec_level.end(); ++i)
	{
		int degree = i->second.size();
		if(distrib.find(degree) == distrib.end())
		{
			distrib[degree] = 1;
		}
		else
		{
			distrib[degree]++;
		}
	}

	std::ofstream fout(argv[3]);
	if(!fout.is_open())
	{
		std::cout << "Error!" << std::endl;
		return -1;
	}
	for(auto i = sec_level.begin(); i != sec_level.end(); ++i)
	{
		for(auto j = i->second.begin(); j != i->second.end(); ++j)
		{
			fout << i->first << " " << *j << '\n';
		}
	}
	fout.close();

	if(argc > 4)
	{
		std::ofstream fout(argv[4]);
		if(!fout.is_open())
		{
			std::cout << "Error!" << std::endl;
			return -1;
		}
		for(auto i = distrib.begin(); i != distrib.end(); ++i)
		{
			fout << i->first << " " << i->second << '\n';
		}
		fout.close();
	}

  return 0;
}
