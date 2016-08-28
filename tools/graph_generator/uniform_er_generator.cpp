#include <string>
#include <map>
#include <set>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <functional>

#include <boost/lexical_cast.hpp>
#include <boost/random/variate_generator.hpp>


typedef std::mt19937 random_generator_t;

typedef std::uniform_int_distribution<> distribution_t;

typedef boost::variate_generator<random_generator_t, distribution_t> generator_t;

void generate_with_uniform_distribution(size_t n, double p, const std::string& file);

int main(int argc, char* argv[])
{
	if(4 != argc)
	{
		std::cerr << "Incorrect usage." << std::endl;
		return -1;
	}
	size_t n = boost::lexical_cast<size_t>(argv[1]);
	double p = boost::lexical_cast<double>(argv[2]);
	std::string output = argv[3];
	generate_with_uniform_distribution(n, p, output);
	return 0;
}

void generate_with_uniform_distribution(size_t n, double p, const std::string& file)
{
	std::ofstream out(file.c_str());
	if(!out.is_open())
	{
		std::cerr << "Error openning output file." << std::endl;
	}
	out << "N=" << n << std::endl;
	out << "p=" << p << std::endl;
	out << "-" << std::endl;
	size_t degree = static_cast<size_t>(p*(n-1)/2);
	random_generator_t rand_generator(time(0));
	distribution_t distrib(0, n-1);
	generator_t gen(rand_generator, distrib);
	std::map<size_t, std::set<size_t>> edges;
	for(size_t i = 0; i < n; ++i)
	{
		for(size_t j = 0; j < n; ++j)
		{
			if(j != i)
			{
				edges[i].insert(j);
			}
		}
	}
	std::function<bool(const std::map<size_t, std::set<size_t>>&)> isEmpty = [degree](const std::map<size_t, std::set<size_t>>& edges) -> bool
	{
		std::map<size_t, std::set<size_t>>::const_iterator it = edges.begin();
		for(; it != edges.end(); ++it)
		{
			if(degree < it->second.size())
			{
				std::cout << "degree " << degree << "   size " << it->second.size() << std::endl;
				return false;
			}
		}
		return true;
	};
	while(!isEmpty(edges))
	{
		size_t v1 = gen();
		std::set<size_t>& s1 = edges[v1];
		if(s1.size() > degree)
		{
			std::set<size_t>::const_iterator it = s1.begin();
			while(it != s1.end() && edges[*it].size() <= degree)
			{
				++it;
			}
			assert(it != s1.end());
			if(it != s1.end())
			{
				edges[*it].erase(v1);
				s1.erase(it);
			}
		}
		//std::cout << "s1 size " << s1.size() << " ";
		//if(s1.size() > degree)
		//{
		//	distribution_t distrib1(0, s1.size()-1);
		//	generator_t gen1(rand_generator, distrib1);

		//	size_t v2 = gen1();
		//	std::set<size_t>::iterator qq = s1.begin();
		//	std::advance(qq, v2);

		//	std::cout << "qq " << *qq << " ";
		//	std::set<size_t>& s2 = edges[*qq];
		//	assert(s2.find(v1) != s2.end());

		//std::cout << "s2 size " << s2.size() << " ";
		//	if(s2.size() > degree)
		//	{
		//		std::cout << __LINE__ << std::endl;
		//		s1.erase(qq);
		//		s2.erase(s2.find(v1));
		//	}
		//}
	}
	//std::vector<size_t> degrees(n, degree);
	//std::vector<size_t> empty_degrees(n, 0);
	//random_generator_t rand_generator(time(0));
	//distribution_t distrib(0, n-1);
	//generator_t gen(rand_generator, distrib);
	//while(degrees != empty_degrees)
	//{
	//	size_t v1 = gen();
	//	size_t v2 = gen();
	//	while(v2 == v1 || degrees[v1] == 0 || degrees[v2] == 0)
	//	{
	//		//std::cout << __LINE__ << std::endl;
	//		v1 = gen();
	//		v2 = gen();
	//	}
	//	size_t min = std::min(v1, v2);
	//	size_t max = std::max(v1, v2);
	//	if(edges[min].insert(max).second)
	//	{
	//		std::cout << __LINE__ << " " << degrees[min] << " " << degrees[max] <<  std::endl;
	//		--degrees[min];
	//		--degrees[max];
	//	}
	//	std::copy(degrees.begin(), degrees.end(), std::ostream_iterator<size_t>(std::cout, ", "));
	//	std::cout << std::endl;
	//}
	//std::map<size_t, std::set<size_t>>::const_iterator it = edges.begin();
	//for(; it != edges.end(); ++it)
	//{
	//	std::set<size_t>::const_iterator it2 = it->second.begin();
	//	for(; it2 != it->second.end(); ++it2)
	//	{
	//		out << it->first << " " << *it2 << std::endl;
	//	}
	//}
	//out.close();
	//for(size_t i = 0; i < n; ++i)
	//{
	//	const std::set<size_t>& neighbours = generate_random_values(i, degree, existing_edges[i], 0, n);
	//	std::set<size_t>::const_iterator it = neighbours.begin();
	//	for(; neighbours.end() != it; ++it)
	//	{
	//		existing_edges[*it].insert(i);
	//		existing_edges[i].insert(*it);
	//		if(i >= *it)
	//		{
	//			out << *it << " " << i << std::endl;
	//		}
	//		else
	//		{
	//			out << i << " " << *it << std::endl;
	//		}
	//	}
	//}
}
