#include <fstream>
#include <iostream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <armadillo>

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS> undirected_graph;

typedef boost::graph_traits<undirected_graph>::vertex_descriptor vertex;
typedef boost::graph_traits<undirected_graph>::edge_iterator edge_iterator;

int main(int argc, char* argv[])
{
  if(2 != argc)
  {
    std::cerr << "invalid usage." << std::endl;
    return -1;
  }
  const std::string input = argv[1];
  std::ifstream graph_file;
  graph_file.open(input);
  if(!graph_file.is_open())
  {
    std::cerr << "Failed to open file containing graph." << std::endl;
    return -1;
  }
  std::size_t N = 0;
  double p = 0;
  boost::archive::text_iarchive ia(graph_file);
  ia >> N >> p;
  undirected_graph graph;
  boost::serialization::load(ia, graph, 0);
  graph_file.close();

  arma::mat m(N, N);
  edge_iterator begin, end;
  for(boost::tie(begin, end) = boost::edges(graph); begin != end; ++begin)
  {
    vertex vs = boost::source(*begin, graph);
    vertex vt = boost::target(*begin, graph);
    m(vs, vt) = 1;
    m(vt, vs) = 1;
  }

  arma::vec eigval = arma::eig_sym(m);
  std::cout << eigval << std::endl;

  return 0;
}
