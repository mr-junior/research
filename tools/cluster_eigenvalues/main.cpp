#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>

#include <armadillo>

int main(int argc, char* argv[])
{
  if(argc < 3)
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

  std::map<int, std::set<int>> cluster_nodes;
  while(std::getline(clusters, line))
  {
    if(!line.empty())
    {
      std::stringstream s(line);
      int cluster_index;
      int node;
      s >> node >> cluster_index;
      cluster_nodes[cluster_index].insert(node);
    }
  }

  //int current_cluster = 0;
  //while(std::getline(clusters, line))
  //{
  //  if(line.empty())
  //  {
  //    ++current_cluster;
  //  }
  //  else
  //  {
  //    std::stringstream s(line);
  //    int node;
  //    s >> node;
  //    cluster_nodes[current_cluster].insert(node);
  //  }
  //}

  graph.close();
  clusters.close();

  for(std::size_t i = 0; i < cluster_nodes.size(); ++i)
  {
    std::set<int>& nodes = cluster_nodes[i];
    std::map<int, int> node_mapping;
    int val = 0;
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
    {
      node_mapping[*it] = val++;
    }
    arma::mat m(nodes.size(), nodes.size(), arma::fill::zeros);
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
    {
      if(connections.end() != connections.find(*it))
      {
        std::set<int>& c = connections[*it];
        for(auto a = c.begin(); a != c.end(); ++a)
        {
          if(nodes.end() != nodes.find(*a))
          {
            m(node_mapping[*it], node_mapping[*a]) = 1;
            m(node_mapping[*a], node_mapping[*it]) = 1;
          }
        }
      }
    }
    std::stringstream s;
    s << (i+1);
    s << "_eigenvalues.txt";
    std::ofstream fout(s.str());
    if(!fout.is_open())
    {
      std::cout << "Error!" << std::endl;
      continue;
    }
    const arma::vec& eig_val = arma::eig_sym(m);
    fout << eig_val;
    fout.close();
  }
  return 0;
}
