#include <string>
#include "graph.hpp"

using namespace adf;

graph_overlay my_graph;

#if defined(__AIESIM__) || defined(__X86SIM__)

int main(int argc, char **argv)
{

	
	std::cout << "[INFO] Running Graph..." << '\n';
	my_graph.init();
	std::cout << "[INFO] Graph initialized, moving to run..."
			  << "\n";
	my_graph.run(1);
	std::cout << "[INFO] Graph finished running, moving to ending..."
			  << "\n";
	my_graph.end();
	std::cout << "[INFO] Graph ended..."
			  << "\n";
	return 0;
}
#endif
