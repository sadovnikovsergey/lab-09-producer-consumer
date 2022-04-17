#include <parser.hpp>

int main(int argc, char** argv)
{
  Arguments arguments = {"www.ozon.ru", 1, 1, 1, "./output.txt"};
  std::vector<std::string> links;
  std::vector<Item> itemList;

  boost::program_options::options_description desc("Parcer");
  desc.add_options()
      ("help", "produce help message")
          ("url",boost::program_options::value<std::string>(), "Enter url of page without http/ or https/")
              ("depth",boost::program_options::value<size_t>(), "Enter depth of parsing")
                  ("network_threads",boost::program_options::value<size_t>(), "Enter number of threads for getting page")
                      ("parser_threads",boost::program_options::value<size_t>(), "Enter threads for parsing of page")
                          ("output",boost::program_options::value<std::string>(), "Enter name of output file");
  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }
  if (vm.count("url")){
    arguments.url = vm["url"].as<std::string>();
  }
  if (vm.count("depth")){
    arguments.depth = vm["depth"].as<size_t>();
  }
  if (vm.count("network_threads")){
    arguments.network_threads = vm["url"].as<size_t>();
  }
  if (vm.count("parser_threads")){
    arguments.parser_threads = vm["parser_threads"].as<size_t>();
  }
  if (vm.count("output")){
    arguments.output = vm["output"].as<std::string>();
  }
  std::string first_page = get_page(arguments.url, "/");
  find_links(first_page, links);


  //**
  std::vector<std::vector<std::string>> vector;
  std::vector<std::thread> threads;
  size_t step = links.size()/2;
  for (size_t i = 0; i < 2; i++)
  {
    std::vector<std::string> temp;
    for (size_t j = i * step; j < step * (i + 1); j++){
      temp.push_back(links[j]);
    }
    vector.push_back(temp);
    threads.emplace_back(fulling_vector,std::ref(temp), std::ref(arguments.depth));
  }
  for (int i = 0; i < 2; i++) {
    try {
      threads[i].join();
    } catch (int num) {
      continue;
    }

  }
  //**


  //fulling_vector(links, arguments.depth);
  item_filling(itemList, links);

  for (size_t i = 0; i < links.size(); i++)
  {
    std::cout << itemList[i].host << " " << itemList[i].target << std::endl;
  }
}
