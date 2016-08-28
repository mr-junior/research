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

int main(int argc, char* argv[])
{
	if(2 != argc)
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
	int e = boost::num_edges(graph);

	assert(n == N);

	boost::filesystem::path general = graph_path;
	general.replace_extension(); 

	//! Density
	double density = 2.0*(double)e/((double)n*((double)n-1.0));

	//! Average path length
	// http://www.boost.org/doc/libs/1_51_0/libs/graph/doc/floyd_warshall_shortest.html
	// http://www.boost.org/doc/libs/1_51_0/libs/graph/doc/johnson_all_pairs_shortest.html
	// http://www.boost.org/doc/libs/1_46_1/libs/graph/example/eccentricity.cpp
	typedef boost::exterior_vertex_property<undirected_graph, int> DistanceProperty;
	typedef DistanceProperty::matrix_type DistanceMatrix;
	typedef DistanceProperty::matrix_map_type DistanceMatrixMap;
	typedef boost::graph_traits<undirected_graph>::edge_descriptor Edge;
	typedef boost::constant_property_map<Edge, int> WeightMap;
	DistanceMatrix distances(n);
	DistanceMatrixMap dm(distances, graph);
	WeightMap wm(1);
	// dependend on the graph density, calculate distances with different algorithm
	if(density > 0.5)
	{
		// calculate all shortest distances between all vertices with Floyd–Warshall algorithm O(n^3)
		boost::floyd_warshall_all_pairs_shortest_paths(graph, dm, boost::weight_map(wm));
	}
	else
	{
		boost::johnson_all_pairs_shortest_paths(graph, dm, boost::weight_map(wm));
	}

	int sum_of_distances{};
	int diameter{};
	for(int u=0; u<n; u++)
	{
		for(int v=0; v<n; v++)
		{
			if(distances[u][v] < std::numeric_limits<int>::max())
			{
				sum_of_distances += distances[u][v];
				// Diameter of the Graph is the maximum distance between any two nodes
				if(distances[u][v] > diameter)
				{
					diameter = distances[u][v];
				}
				//std::cout<< "["<<u<<"]["<<v<<"] = "<<distances[u][v]<<std::endl;
			}
		}
	}

	//! Mean local Clustering coefficinent
	// http://www.boost.org/doc/libs/1_47_0/libs/graph/example/clustering_coefficient.cpp
	typedef boost::exterior_vertex_property<undirected_graph, float> Property;
	typedef Property::container_type Container;
	typedef Property::map_type Map;

	Container coefs(n);
	Map clusteringMap(coefs, graph);
	float cc = boost::all_clustering_coefficients(graph, clusteringMap);

	std::ofstream  clust_coeff_file(general.string() + "_clustering_coefficients.txt");
	if(clust_coeff_file.is_open())
	{
		boost::graph_traits<undirected_graph>::vertex_iterator i, end;
		for(std::tie(i, end) = vertices(graph); i != end; ++i) {
			clust_coeff_file << *i << " " << boost::get(clusteringMap, *i) << std::endl;
		}
		clust_coeff_file.close();
	}

	std::ofstream global_props_file(general.string() + "_global_properties.txt");
	if(global_props_file.is_open())
	{
		//boost::all_clustering_coefficients returned the mean clustering coefficient
		//! Mean clustering coefficient
		global_props_file << cc << std::endl;

		//! Average Path Length
		global_props_file	<< ((double)sum_of_distances/((double)n*((double)n-1))) << std::endl;

		//! Diameter of Graph
		global_props_file << diameter << std::endl;

		global_props_file.close();
	}

	//! Closeness Centrality
	// http://www.boost.org/doc/libs/1_47_0/libs/graph/example/closeness_centrality.cpp

	Container cents(n);
	Map closenessMap(cents, graph);
	boost::all_closeness_centralities(graph, dm, closenessMap);

	std::ofstream close_centr_file(general.string() + "_closeness_centralities.txt");
	if(close_centr_file.is_open())
	{
		boost::graph_traits<undirected_graph>::vertex_iterator i, end;
		for(std::tie(i, end) = vertices(graph); i != end; ++i)
		{
			close_centr_file << *i << " " << boost::get(closenessMap, *i) << std::endl;
		}
		close_centr_file.close();
	}

	//! Betweenness Centrality
	// http://www.boost.org/doc/libs/1_51_0/libs/graph/doc/betweenness_centrality.html
	Container betwcents(n);
	Map betweennessMap(betwcents, graph);
	boost::brandes_betweenness_centrality(graph, betweennessMap);

	std::ofstream betw_centr_file(general.string() + "_betweenness_centralities.txt");
	if(betw_centr_file.is_open())
	{
		boost::graph_traits<undirected_graph>::vertex_iterator i, end;
		for(std::tie(i, end) = vertices(graph); i != end; ++i) {
			betw_centr_file << *i << " " << boost::get(betweennessMap, *i) << std::endl;
		}
		betw_centr_file.close();
	}
	return 0;
}
