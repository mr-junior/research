#include "converter.h" 

#include <map>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/lexical_cast.hpp>
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

const const char* base_exception::what() const noexcept
{
	return m_message;
}

base_exception::base_exception(const std::string& message)
: m_message(message)
{
}

// exception messages.
const std::string wrong_flag("Wrong flag given, flag must be or 's' or 'b'");
const std::string cannot_open_boost_graph_file("Failed to open boost graph file.");
const std::string cannot_open_standart_graph_file("Failed to open standart graph file.");
const std::string problem_on_reading_boost_graph_file("Problem on reading boost graph file");
const std::string problem_on_writing_boost_graph_file("Problem on writing in boost graph file");
const std::string wrong_format_of_standart_graph_file("Wrong format of standart graph file");

const std::vector<char> supported_formats = {'b', 'a', 'm'};

std::unique_ptr<converter> converter::from(const std::pair<char, std::string>& input)
{
	const char format = input.first;
	if(supported_formats.end() == std::find(supported_formats.begin(), supported_formats.end(), format))
	{
		throw base_exception(unsupported_format);
	}
	std::unique_ptr<converter> conv(new converter(format));
	std::ifstream input_file(input.second);
	if(!input_file.is_open())
	{
		throw base_exception(error_openning_input_file);
	}
	typedef std::map<char, std::function<void(const std::ifstream&)>> readers_map;
	readers_map from_readers =
		{
			{supported_formats[0], &std::bind(&converter::from_boost, *conv, _1)},
			{supported_formats[1], &std::bind(&converter::from_adjacency_list, *conv, _1)},
			{supported_formats[2], &std::bind(&converter::from_matrix, *conv, _1}
		};
	readers_map::iterator it = from_readers.find(format);
	assert(from_readers.end() != it);
	try
	{
		(it->second)(input_file);
	}
	catch(...)
	{
		input_file.close();
		throw;
	}
	input_file.close();
}

converter::converter(const char input_format) :
	m_input_format(input_format)
{
}

void converter::read_file(const std::ifstream& in)
{
	m_input_file << in.rdbuf();
}

void converter::from_matrix(const std::ifstream& in)
{
	assert(in.is_open());
	read_file(in);
	check_for_matrix();
}

void converter::check_for_matrix() const
{
	std::regex exp("((([01] )+[01]\n)+(([01] )+[01]))");
	if(!std::regex_match(m_input_file.str(), exp))
	{
		throw base_exception(not_matrix);
	}
}

void converter::to_boost(

	void convert(char flag, const std::string& f_f_n
			, const std::string& s_f_n)
	{
		if ('s' == flag) {
			convert_from_standart_graph(f_f_n, s_f_n);
		} else if ('b' == flag) {
			convert_from_boost_graph(f_f_n, s_f_n);
		} else {
			throw exception(wrong_flag);
		}
	}

	void convert_from_boost_graph(const std::string& b_g_f_n
			, const std::string& s_g_f_n)
	{
		std::ifstream b_g_f;
		b_g_f.open(b_g_f_n.c_str());
		if(!b_g_f.is_open())
		{
			throw exception(cannot_open_boost_graph_file);
		}
		std::ofstream s_g_f;
		s_g_f.open(s_g_f_n.c_str());
		if(!s_g_f.is_open())
		{
			throw exception(cannot_open_standart_graph_file);
		}
		undirected_graph graph;
		int N = 0;
		double p = 0.0;
		try {
			boost::archive::text_iarchive ia(b_g_f);
			ia >> N >> p;
			boost::serialization::load(ia, graph, 0);
		} catch (const boost::archive::archive_exception&){
			throw exception(problem_on_reading_boost_graph_file);
		}
		//s_g_f << "ModelName=ERModel" << std::endl;
		//s_g_f << "Vertices=" << N << std::endl;
		//s_g_f << "Probability=" << p << std::endl;
		//s_g_f << "-\n";
		s_g_f << "Source Target" << std::endl;
		undirected_graph::edge_iterator e, e_end;
		for(boost::tie(e, e_end) = boost::edges(graph)
				; e_end != e; ++e) {
			vertex s = boost::source(*e, graph);
			vertex t = boost::target(*e, graph);
			vertex_iterator next, end;
			int i = 0;
			int s_index = 0, t_index = 0;
			for (boost::tie(next, end) = vertices(graph)
					; next != end; ++next, ++i) {
				if (s == *next) {
					s_index = i;
				}
				if (t == *next) {
					t_index = i;
				}
			}
			s_g_f << (s_index < t_index ? s_index : t_index)
				<< " " << (s_index < t_index ? t_index : s_index)
				<< std::endl; 
		}
		s_g_f.close();
		b_g_f.close();
	}

	namespace {

		void parse_property_line(const std::string& line
				, boost::archive::text_oarchive& out_a, int& N)
		{
			int assign_pos = line.find_first_of('=');
			if (std::string::npos == assign_pos
					|| 0 == assign_pos) {
				throw exception(wrong_format_of_standart_graph_file);
			}
			std::string prop_name = line.substr(0, assign_pos);
			std::string prop_value = line.substr(assign_pos+1);
			if (0 == prop_value.size()) {
				throw exception(wrong_format_of_standart_graph_file);
			}
			if ("N" == prop_name) {
				N = boost::lexical_cast<int>(prop_value);
				out_a << N;
			} else if ("p" == prop_name) {
				double p = boost::lexical_cast<double>(prop_value);
				out_a << p;
			} else {
				throw exception(wrong_format_of_standart_graph_file);
			}
		}

		void parse_edge_line(const std::string& line
				, boost::archive::text_oarchive& out_a
				, const int N, undirected_graph& g)
		{
			int space_pos = line.find_first_of(' ');
			if (std::string::npos == space_pos
					|| 0 == space_pos) {
				throw exception(wrong_format_of_standart_graph_file);
			}
			int u = boost::lexical_cast<int>(line.substr(0, space_pos));
			int v = boost::lexical_cast<int>(line.substr(space_pos + 1));
			if (u >= v || v >= N) {
				throw exception(wrong_format_of_standart_graph_file);
			}
			vertex s;
			vertex t;
			vertex_iterator next, end;
			int i = 0;
			for (boost::tie(next, end) = vertices(g)
					; next != end; ++next, ++i) {
				if (u == i) {
					s = *next;
				}
				if (v == i) {
					t = *next;
				}
			}
			boost::add_edge(s, t, g);
		}

	}

	void convert_from_standart_graph(const std::string& s_g_f_n
			, const std::string& b_g_f_n)
	{
		std::ofstream b_g_f;
		b_g_f.open(b_g_f_n.c_str());
		if(!b_g_f.is_open())
		{
			throw exception(cannot_open_boost_graph_file);
		}
		std::ifstream s_g_f;
		s_g_f.open(s_g_f_n.c_str());
		if(!s_g_f.is_open())
		{
			throw exception(cannot_open_standart_graph_file);
		}
		try {
			boost::archive::text_oarchive oa(b_g_f);
			std::string line;
			int N;
			while (s_g_f.good()) {
				std::getline(s_g_f, line);
				if (line == "-") {
					break;
				}
				parse_property_line(line, oa, N);
			}
			if (!s_g_f.good() && line != "-") {
				throw exception(
						wrong_format_of_standart_graph_file);
			}
			undirected_graph graph(N);
			while (s_g_f.good()) {
				std::getline(s_g_f, line);    
				if (line == "") {
					break;
				}
				parse_edge_line(line, oa, N, graph);
			}
			boost::serialization::save(oa, graph, 0);
		} catch (const boost::archive::archive_exception&) {
			throw exception(problem_on_writing_boost_graph_file);
		} catch (const boost::bad_lexical_cast&) {
			throw exception(wrong_format_of_standart_graph_file);
		}
		b_g_f.close();
		s_g_f.close();
	}

}
