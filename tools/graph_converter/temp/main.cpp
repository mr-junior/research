#include <map>
#include <fstream>
#include <exception>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/algorithm/string.hpp>
// graph inner structure
typedef boost::adjacency_list<boost::setS, boost::listS
, boost::undirectedS> undirected_graph;
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

double round(double num, int digits)
{
	static int scale[] = { 1, 10, 100, 1000, 10000, 100000, 1000000 };

	assert( digits >= 0 && digits < int(sizeof(scale) / sizeof(int)) );
	if(num < 0)
	{
		return std::ceil( num * scale[digits] - 0.5 ) / scale[digits];
	}
	return std::floor( num * scale[digits] + 0.5 ) / scale[digits];
}

class exception : public std::exception
{
	public:
		exception(const std::string& str) :
			m_str(str) {}
		const char* what() const noexcept
		{
			return m_str.c_str();
		}
	private:
		std::string m_str;
};

void add_edge(size_t source, size_t target, undirected_graph& g)
{
	vertex s;
	vertex t;
	vertex_iterator next, end;
	int i = 0;
	for(boost::tie(next, end) = vertices(g); next != end; ++next, ++i)
	{
		if(source == i)
		{
			s = *next;
		}
		if(target == i)
		{
			t = *next;
		}
	}
	boost::add_edge(s, t, g);
}

void convert_matrix_to_boost(const std::string& in, const std::string& out)
{
	std::ofstream b_g_f;
	b_g_f.open(out.c_str());
	if(!b_g_f.is_open())
	{
		throw exception("Cannot open boost graph file.");
	}
	std::ifstream s_g_f;
	s_g_f.open(in.c_str());
	if(!s_g_f.is_open())
	{
		throw exception("Cannot open adjacency matrix file.");
	}
	try
	{
		boost::archive::text_oarchive oa(b_g_f);
		std::string line;
		int N;
		std::vector<boost::dynamic_bitset<>> matrix;
		size_t current_line = 0;
		size_t edge_count = 0;
		while(std::getline(s_g_f, line))
		{
			std::vector<std::string> bits;
			int a = line.find("\r");
			if(std::string::npos != a)
			{
				line.erase(a, 1);
			}
			a = line.find("\n");
			if(std::string::npos != a)
			{
				line.erase(a, 1);
			}
			boost::trim(line);
			boost::split(bits, line, boost::is_any_of(" "), boost::token_compress_on);
			if(matrix.empty())
			{
				N = bits.size();
				matrix.resize(N);
				for(size_t i = 0; i < matrix.size(); ++i)
				{
					matrix[i].resize(N);
				}
			}
			for(size_t i = 0; i < bits.size(); ++i)
			{
				int v = stoi(bits[i]);
				assert(0 == v || 1 == v);
				matrix[current_line][i] = v;
				if(1 == v)
				{
					++edge_count;
				}
			}
			++current_line;
		}
		undirected_graph graph(N);
		std::cout << "edge count " << edge_count << std::endl;
		for(size_t i = 0; i < matrix.size(); ++i)
		{
			assert(matrix.size() == matrix[i].size());
			for(size_t j = 0; j < matrix[i].size(); ++j)
			{
				if(matrix[i][j])
				{
					add_edge(i, j, graph);
				}
			}
		}
		double p = edge_count/(double)(N*(N-1));
		std::cout << "p = " << p << std::endl;
		p = round(p, 2);
		std::cout << "p = " << p << std::endl;
		oa << N << p;
		boost::serialization::save(oa, graph, 0);
	}
	catch(const boost::archive::archive_exception&)
	{
		b_g_f.close();
		s_g_f.close();
		throw exception("Problem writing boost graph file.");
	}
	catch(const std::invalid_argument&)
	{
		b_g_f.close();
		s_g_f.close();
		throw exception("Wrong format of adjacency matrix file.");
	}
	b_g_f.close();
	s_g_f.close();
}

int main(int argc, char* argv[])
{
	if(3 != argc)
	{
		std::cerr << "Incorrect argument count." << std::endl;
		return 0;
	}
	std::string in(argv[1]);
	std::string out(argv[2]);
	convert_matrix_to_boost(in, out);
	return 0;
}
