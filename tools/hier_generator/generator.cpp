#include <fstream>
#include <random>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/graph/random.hpp>

// random generator
typedef std::mt19937 random_generator;

// graph inner structure
typedef boost::adjacency_list<boost::setS, boost::listS, boost::undirectedS> undirected_graph;

// Erdos-Renyi generator
typedef boost::erdos_renyi_iterator<random_generator, undirected_graph> erdos_renyi_iterator;

// graph vertex
typedef boost::graph_traits<undirected_graph>::vertex_descriptor vertex;
// graph edge
typedef boost::graph_traits<undirected_graph>::edge_descriptor edge;
// graph out edge
typedef boost::graph_traits<undirected_graph>::out_edge_iterator out_edge_iterator;
// graph edge
typedef boost::graph_traits<undirected_graph>::edge_iterator edge_iterator;
// vertex iterator
typedef boost::graph_traits<undirected_graph>::vertex_iterator vertex_iterator;
// adjacency list iterator
typedef boost::graph_traits<undirected_graph>::adjacency_iterator adjacency_iterator;

int main(int argc, char* argv[])
{

  if(6 != argc)
  {
    std::cerr << "Incorrect usage.\n"
                 "Usage: " << argv[0] << " [options]" << std::endl;
    return -1;
  }
  int N1 = boost::lexical_cast<int>(argv[1]);
  double p1 = boost::lexical_cast<double>(argv[2]);
  int N2 = boost::lexical_cast<int>(argv[3]);
  double p2 = boost::lexical_cast<double>(argv[4]);
  std::string file_name = boost::lexical_cast<std::string>(argv[5]);
  random_generator generator(time(0));
  std::vector<undirected_graph> first_level_graphs;
  undirected_graph second_level_graph(erdos_renyi_iterator(generator, N2, p2), erdos_renyi_iterator(), N2);
  for(int i = 0; i < N2; ++i)
  {
    first_level_graphs.emplace_back(erdos_renyi_iterator(generator, N1, p1), erdos_renyi_iterator(), N1);
  }
  undirected_graph final_graph(N1*N2);
  std::map<int, vertex> final_graph_vertices;
  vertex_iterator vit, vit_end;
  int index = 0;
  for(std::tie(vit, vit_end) = boost::vertices(final_graph); vit != vit_end; ++vit, ++index)
  {
    final_graph_vertices.insert(std::make_pair(index, *vit));
  }
  std::vector<std::map<vertex, int>> first_level_graph_vertices(N2);
  for(int i = 0; i < N2; ++i)
  {
    undirected_graph& g = first_level_graphs[i];
    vertex_iterator vit, vit_end;
    int index = 0;
    for(std::tie(vit, vit_end) = boost::vertices(g); vit != vit_end; ++vit, ++index)
    {
      first_level_graph_vertices[i].insert(std::make_pair(*vit, index));
    }
    std::map<vertex, int>& indices = first_level_graph_vertices[i];
    edge_iterator it, it_end;
    for(std::tie(it, it_end) = boost::edges(g); it != it_end; ++it)
    {
      vertex s = boost::source(*it, g);
      vertex t = boost::target(*it, g);
      boost::add_edge(final_graph_vertices[indices[s]+i*N1], final_graph_vertices[indices[t]+i*N1], final_graph);
    }
  }

  std::map<vertex, int> second_level_graph_vertices;
  index = 0;
  for(std::tie(vit, vit_end) = boost::vertices(second_level_graph); vit != vit_end; ++vit, ++index)
  {
    second_level_graph_vertices.insert(std::make_pair(*vit, index));
  }

  edge_iterator eit, eit_end;
  for(std::tie(eit, eit_end) = boost::edges(second_level_graph); eit != eit_end; ++eit)
  {
    vertex s = boost::source(*eit, second_level_graph);
    vertex t = boost::target(*eit, second_level_graph);
    int s_ind = second_level_graph_vertices[s];
    int t_ind = second_level_graph_vertices[t];
    undirected_graph& g_s = first_level_graphs[s_ind];
    undirected_graph& g_t = first_level_graphs[t_ind];
    vertex v_s = boost::random_vertex(g_s, generator);
    vertex v_t = boost::random_vertex(g_t, generator);
    assert(v_s != v_t);
    int ind1 = first_level_graph_vertices[s_ind][v_s];
    int ind2 = first_level_graph_vertices[t_ind][v_t];
    boost::add_edge(final_graph_vertices[s_ind*N1 + ind1], final_graph_vertices[t_ind*N1+ind2], final_graph);
  }
  std::ofstream file;
  file.open(file_name.c_str());
  if(file.is_open())
  {
    boost::archive::text_oarchive oa(file);
    oa << N1*N2 << p1*p2;
    boost::serialization::save(oa, final_graph, 0);
  }
  file.close();
}
