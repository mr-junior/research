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

int main(int argc, char* argv[])
{
  if(argc < 4)
  {
    std::cerr << "Program usage: [program name] [input graph name] [file name with nodes to retain] [output graph file name]." << std::endl;
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
  std::ofstream fout(argv[3]);
  if(!fout.is_open())
  {
    return -1;
  }
  undirected_graph graph;
  int N = 0;
  double p = 0.0;
  boost::archive::text_iarchive ia(fin);
  ia >> N >> p;
  boost::serialization::load(ia, graph, 0);
  std::set<int> nodes;
  std::string line;
  while(std::getline(nodes_file, line))
  {
    nodes.insert(std::stoi(line));
  }
  int i = 0;
  std::map<vertex_descriptor, int> vertex_indices;
  vertex_iterator next, end;
  for(boost::tie(next, end) = boost::vertices(graph); next != end; ++next, ++i)
  {
    vertex_indices[*next] = i;
  }
  vertex_iterator vi, vi_end;
  boost::tie(vi, vi_end) = boost::vertices(graph);
  for(next = vi; vi != vi_end; vi = next)
  {
    ++next;
    if(nodes.end() == nodes.find(vertex_indices[*vi]))
    {
      boost::clear_vertex(*vi, graph);
      //boost::remove_vertex(*vi, graph);
    }
  }

  boost::archive::text_oarchive oa(fout);
  oa << boost::num_vertices(graph) << p;
  boost::serialization::save(oa, graph, 0);

  fin.close();
  nodes_file.close();
  fout.close();

  return 0;
}
