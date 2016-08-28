#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/graph/clustering_coefficient.hpp>
#include <boost/graph/closeness_centrality.hpp>
#include <boost/graph/betweenness_centrality.hpp>

#include <iostream>
#include <fstream>
#include <string>

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS> undirected_graph;
// graph vertex
typedef boost::graph_traits<undirected_graph>::vertex_descriptor
        vertex;
// graph edge
typedef boost::graph_traits<undirected_graph>::edge_descriptor edge;
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
	if(4 != argc)
	{
		std::cerr << "Incorrect usage." << std::endl;
		return -1;
	}
	std::ifstream graph_file;
	boost::filesystem::path graph_path(argv[1]);
	graph_file.open(graph_path.string());
	if(!graph_file.is_open())
	{
		std::cerr << "Failed to open file containing graph." << std::endl;
		return -1;
	}
	boost::archive::text_iarchive ia(graph_file);
	unsigned N;
	double p;
	ia >> N >> p;
	undirected_graph graph;
	boost::serialization::load(ia, graph, 0);
	int n = boost::num_vertices(graph);

	assert(n == N);

	boost::filesystem::path general = graph_path;
	general.replace_extension(); 

	std::ifstream node_file_1;
	boost::filesystem::path node_path_1(argv[2]);
	node_file_1.open(node_path_1.string());
	if(!node_file_1.is_open())
	{
		std::cerr << "Failed to open file containing first node list." << std::endl;
		return -1;
	}

	std::ifstream node_file_2;
	boost::filesystem::path node_path_2(argv[3]);
	node_file_2.open(node_path_2.string());
	if(!node_file_2.is_open())
	{
		std::cerr << "Failed to open file containing second node list." << std::endl;
		return -1;
	}

	std::set<int> vertex_indexes_1;
	while(!node_file_1.eof())
	{
		int v;
		node_file_1 >> v;
		vertex_indexes_1.insert(v);
	}

	std::set<int> vertex_indexes_2;
	while(!node_file_2.eof())
	{
		int v;
		node_file_2 >> v;
		vertex_indexes_2.insert(v);
	}

	std::set<vertex> vertexes_1;
	std::set<vertex> vertexes_2;
	vertex_iterator next, end;
	int i = 0;
	for(boost::tie(next, end) = boost::vertices(graph); next != end; ++next, ++i)
	{
		if(vertex_indexes_1.end() != vertex_indexes_1.find(i))
		{
			vertexes_1.insert(*next);
		}
		if(vertex_indexes_2.end() != vertex_indexes_2.find(i))
		{
			vertexes_2.insert(*next);
		}
	}

	int between_edges = 0;
	for(std::set<vertex>::const_iterator it = vertexes_1.begin(); it != vertexes_1.end(); ++it)
	{
		adjacency_iterator b, e;
		for(boost::tie(b, e) = boost::adjacent_vertices(*it, graph); b != e; ++b)
		{
			if(vertexes_2.end() != vertexes_2.find(*b))
			{
				++between_edges;
			}
		}
	}
	std::cout << between_edges << std::endl;

	return 0;
}
