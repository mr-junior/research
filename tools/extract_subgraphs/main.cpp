#include <iostream>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
// graph inner structure
typedef boost::adjacency_list<boost::setS, boost::vecS
        , boost::undirectedS> undirected_graph;
// graph vertex
typedef boost::graph_traits<undirected_graph>::vertex_descriptor
        vertex_descriptor;
// graph edge
typedef boost::graph_traits<undirected_graph>::edge_descriptor edge_descriptor;
// graph out edge
typedef boost::graph_traits<undirected_graph>::out_edge_iterator 
        out_edge_iterator;
// vertex iterator
typedef boost::graph_traits<undirected_graph>::vertex_iterator 
        vertex_iterator;
// adjacency list iterator
typedef boost::graph_traits<undirected_graph>::adjacency_iterator 
        adjacency_iterator;

void copy_graph(const undirected_graph& input, undirected_graph& output)
{
  std::stringstream ss;
  boost::archive::text_oarchive oa(ss);
  boost::serialization::save(oa, input, 0);
  boost::archive::text_iarchive ia(ss);
  boost::serialization::load(ia, output, 0);
}

int main(int argc, char* argv[])
{
  if(argc < 3)
  {
    std::cerr << "Program usage: [program name] [input graph name] [file name with clusters]." << std::endl;
    return -1;
  }
  std::ifstream fin(argv[1]);
  if(!fin.is_open())
  {
    return -1;
  }
  std::ifstream nodes_file(argv[2]);
  if(!nodes_file.is_open())
  {
    return -1;
  }
  undirected_graph graph;
  int N = 0;
  double p = 0.0;
  boost::archive::text_iarchive ia(fin);
  ia >> N >> p;
  boost::serialization::load(ia, graph, 0);
  std::map<int, std::set<int>> cluster_nodes;
  std::string line;
  std::size_t current_cluster = 0;
  while(std::getline(nodes_file, line))
  {
    if(line.empty())
    {
      ++current_cluster;
    }
    else
    {
      cluster_nodes[current_cluster].insert(std::stoi(line));
    }
  }
  std::cout << "Cluster count: " << cluster_nodes.size() << std::endl;
  int i = 0;
  std::map<vertex_descriptor, int> vertex_indices;
  vertex_iterator next, end;
  for(boost::tie(next, end) = boost::vertices(graph); next != end; ++next, ++i)
  {
    vertex_indices[*next] = i;
  }
  vertex_iterator vi, vi_end;
  for(std::size_t cluster = 0; cluster < cluster_nodes.size(); ++cluster)
  {
    undirected_graph copy;
    copy_graph(graph, copy);
    boost::tie(vi, vi_end) = boost::vertices(copy);
    for(next = vi; vi != vi_end; vi = next)
    {
      ++next;
      if(cluster_nodes[cluster].end() == cluster_nodes[cluster].find(vertex_indices[*vi]))
      {
        boost::clear_vertex(*vi, copy);
        boost::remove_vertex(*vi, copy);
      }
    }
    undirected_graph new_copy;
    copy_graph(copy, new_copy);

    std::stringstream s;
    s << cluster;
    s << ".txt";

    std::ofstream cluster_file(s.str());
    if(!cluster_file.is_open())
    {
      continue;
    }

    boost::archive::text_oarchive oa(cluster_file);
    oa << boost::num_vertices(new_copy) << p;
    boost::serialization::save(oa, new_copy, 0);
    cluster_file.close();
  }

  fin.close();
  nodes_file.close();

  return 0;
}
