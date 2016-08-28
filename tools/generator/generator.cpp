#include <fstream>
#include <random>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/archive/text_oarchive.hpp>

// random generator
typedef std::mt19937 random_generator;

// graph inner structure
typedef boost::adjacency_list<boost::setS, boost::listS, boost::undirectedS> undirected_graph;

// Erdos-Renyi generator
typedef boost::erdos_renyi_iterator<random_generator, undirected_graph> erdos_renyi_iterator;

int main(int argc, char* argv[])
{

  if(4 != argc)
  {
    std::cerr << "Incorrect usage.\n"
                 "Usage: " << argv[0] << " [options]" << std::endl;
    return -1;
  }
  int N = boost::lexical_cast<int>(argv[1]);
  double p = boost::lexical_cast<double>(argv[2]);
  std::string file_name = boost::lexical_cast<std::string>(argv[3]);
  random_generator generator(time(0));
  undirected_graph graph(erdos_renyi_iterator(generator, N, p), erdos_renyi_iterator(), N);
  std::ofstream file;
  file.open(file_name.c_str());
  if(file.is_open())
  {
    boost::archive::text_oarchive oa(file);
    oa << N << p;
    boost::serialization::save(oa, graph, 0);
  }
  file.close();
}
