#include <QImage>
#include <QGraphicsScene>
#include <QPainter>
#include <QApplication>

#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <sstream>

#include <cctype>
#include <string>
#include <algorithm>
#include <regex>

inline std::string trim(const std::string &s)
{
	auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
	auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
	return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}

std::vector<std::string> split(const std::string& input, const std::string& regex)
{
	// passing -1 as the submatch index parameter performs splitting
	std::regex re(regex);
	std::sregex_token_iterator
		first{input.begin(), input.end(), re, -1},
		last;
	return {first, last};
}

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		std::cerr << "Invalid usage." << std::endl;
		return 0;
	}
  std::ifstream graph_file(argv[1]);
  if(!graph_file.is_open())
  {
    std::cout << "error" << std::endl;
    return -1;
  }
  std::map<unsigned, std::set<unsigned>> connections;
  std::string line;
	std::set<unsigned> nodes;
	std::size_t node_count = 0;
  while(std::getline(graph_file, line))
  {
		line = trim(line); 
		if(line.empty())
		{
			continue;
		}
		if(std::string::npos != line.find_first_of('='))
		{
			std::vector<std::string> tokens = split(line, "=");
			if(tokens.size() != 2)
			{
				continue;
			}
			for(auto& i : tokens)
			{
				i = trim(i);
			}
			if(tokens[0] == "N")
			{
				node_count = stoi(tokens[1]);
			}
			continue;
		}
		if("-" == line)
		{
			continue;
		}
    std::stringstream s(line);
    unsigned node1;
    unsigned node2;
    s >> node1 >> node2;
		nodes.insert(node1);
		nodes.insert(node2);
		connections[node1].insert(node2);
		connections[node2].insert(node1);
  }

	// Construct adjacency matrix from edge list
	std::size_t s = 0 != node_count ? node_count : nodes.size();
	std::vector<std::vector<bool>> matrix(s, std::vector<bool>(s, false));
	for(auto i = connections.begin(); i != connections.end(); ++i)
	{
		auto s = i->first;
		for(auto j = i->second.begin(); j != i->second.end(); ++j)
		{
			auto t = *j;
			matrix[s][t] = true;
			matrix[t][s] = true;
		}
	}

	QApplication app(argc, argv);
	unsigned r = 6;
	QPen pen(Qt::black);
	QBrush brush(Qt::red);
	QGraphicsScene* scene = new QGraphicsScene();
	for(unsigned i = 0; i < matrix.size(); ++i)
	{
		QGraphicsLineItem* line1 = scene->addLine(QLine(0, i*r, matrix.size()*r, i*r), pen);
		QGraphicsLineItem* line2 = scene->addLine(QLine(i*r, 0, i*r, matrix.size()*r), pen);
		for(unsigned j = 0; j < matrix.size(); ++j)
		{
			if(matrix[i][j])
			{
				QGraphicsRectItem* it = scene->addRect(QRect(i*r, j*r, r, r), pen, brush);
			}
		}
	}
	QGraphicsLineItem* line1 = scene->addLine(QLine(0, matrix.size()*r, matrix.size()*r, matrix.size()*r), pen);
	QGraphicsLineItem* line2 = scene->addLine(QLine(matrix.size()*r, 0, matrix.size()*r, matrix.size()*r), pen);

	scene->clearSelection();                                                  // Selections would also render to the file
	scene->setSceneRect(scene->itemsBoundingRect());                          // Re-shrink the scene to it's bounding contents

	QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
	image.fill(Qt::white);                                              // Start all pixels transparent

	QPainter painter(&image);
	scene->render(&painter);
	image.save(argv[2]);
	return 0;
}
