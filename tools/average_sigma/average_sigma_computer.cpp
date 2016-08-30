#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>

int main(int argc, char* argv[])
{
  if(5 != argc)
  {
    std::cerr << "Incorrect usage.\n"
                 "Usage: " << argv[0] << " [options]" << std::endl;
    return -1;
  }
  std::string input_file_name = boost::lexical_cast<std::string>(argv[1]);
  long long int step_count = boost::lexical_cast<long long int>(argv[2]);
  long long int sigma_start = boost::lexical_cast<long long int>(argv[3]);
  long long int data_step = boost::lexical_cast<long long int>(argv[4]);
  if(0 == data_step)
  {
    data_step = 1;
  }
  long long int sigma_steps = (step_count - sigma_start) / data_step;
  std::vector<long long int> cycle_count_vector;
  cycle_count_vector.reserve(sigma_steps);
  std::ifstream input_file;
  input_file.open(input_file_name.c_str());
	if(!input_file.is_open())
	{
		std::cerr << "Can not open file " << input_file_name << std::endl;
		return -1;
	}

  // this part is unused here
  int N = 0;
  double p = 0.0;
  double mu = 0.0;
  input_file >> N >> p >> mu;

  int average_for_sigma_delta = 1000;
  int average_for_sigma_count = 0;
  double average_for_sigma_sum = 0;
  double average_for_sigma = 0;
 
  int average_delta = 1000;
  int average_count = 0;
  double average_sum = 0;
  double average = 0;

  long long int step = 0;
  long double cycle_count = 0;

  for(long long i = 0; (i <= step_count) && (!input_file.eof()); i += data_step)
  {
    input_file >> step >> cycle_count;
    assert(step == i);

    average_sum += cycle_count;
    ++average_count;
    if(average_count == average_delta)
    {
      average += average_sum * data_step / static_cast<double>(step_count);
      average_sum = 0;
      average_count = 0;
    }

    if(i > sigma_start)
    {
      cycle_count_vector.push_back(cycle_count);
      average_for_sigma_sum += cycle_count;
      ++average_for_sigma_count;
      if(average_for_sigma_count == average_for_sigma_delta)
      {
        average_for_sigma += average_for_sigma_sum / static_cast<double>(sigma_steps);
        average_for_sigma_sum = 0;
        average_for_sigma_count = 0;
      }
    }
  }

  average += average_sum * data_step/ static_cast<double>(step_count);

  average_for_sigma += average_for_sigma_sum / static_cast<double>(sigma_steps);

  double sigma_sum = 0;
  double sigma  = 0;
  int sigma_delta = 100;
  size_t i;
  size_t vector_size = cycle_count_vector.size();
  for(i = 0; i < vector_size; ++i)
  {
    sigma_sum += (average_for_sigma - cycle_count_vector[i]) * (average_for_sigma - cycle_count_vector[i]);
    //sigma_sum += (average - cycle_count_vector[i]) * (average - cycle_count_vector[i]);
    if(0 == (i % sigma_delta))
    {
      sigma += sigma_sum / static_cast<double>(sigma_steps);
      sigma_sum = 0;
    }
  }
  sigma += sigma_sum / static_cast<double>(sigma_steps);
  sigma = sqrt(sigma);

  std::cout << average << " " << average_for_sigma << " " << sigma << std::endl;
  return 0;
}
