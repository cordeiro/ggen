/* Copyright Swann Perarnau 2009
*
*   contributor(s) :  
*
*   contact : firstname.lastname@imag.fr	
*
* This software is a computer program whose purpose is to help the
* random generation of graph structures and adding various properties
* on those structures.
*
* This software is governed by the CeCILL  license under French law and
* abiding by the rules of distribution of free software.  You can  use, 
* modify and/ or redistribute the software under the terms of the CeCILL
* license as circulated by CEA, CNRS and INRIA at the following URL
* "http://www.cecill.info". 
* 
* As a counterpart to the access to the source code and  rights to copy,
* modify and redistribute granted by the license, users are provided only
* with a limited warranty  and the software's author,  the holder of the
* economic rights,  and the successive licensors  have only  limited
* liability. 
* 
* In this respect, the user's attention is drawn to the risks associated
* with loading,  using,  modifying and/or developing or reproducing the
* software by the user in light of its specific status of free software,
* that may mean  that it is complicated to manipulate,  and  that  also
* therefore means  that it is reserved for developers  and  experienced
* professionals having in-depth computer knowledge. Users are therefore
* encouraged to load and test the software's suitability as regards their
* requirements in conditions enabling the security of their systems and/or 
* data to be ensured and,  more generally, to use and operate it in the 
* same conditions as regards security. 
* 
* The fact that you are presently reading this means that you have had
* knowledge of the CeCILL license and that you accept its terms.
*/
/* GGen is a random graph generator :
 * it provides means to generate a graph following a
 * collection of methods found in the litterature.
 *
 * This is a research project founded by the MOAIS Team,
 * INRIA, Grenoble Universities.
 */


#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

/* We use extensively the BOOST library for 
 * handling output, program options and random generators
 */
#include <boost/config.hpp>
#include <boost/program_options.hpp>
#include <boost/graph/graphviz.hpp>

#include "ggen.hpp"
#include "random.hpp"
#include "dynamic_properties.hpp"

using namespace boost;

////////////////////////////////////////////////////////////////////////////////
// Generation Methods
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////
// Utils
// Methods for standard pre or post operations on graphs
// Contains :
// 	-- edge elimination
///////////////////////////////////////

/* There is probably a number of questions that need to be asked before considering
 * this good code.
 */
void edge_limitation(Graph& g,int wanted_edge_number)
{
	if(num_edges(g) > wanted_edge_number)
	{
		// choose randomly an edge

		// delete it
	}
}



///////////////////////////////////////
// Adjacency Matrix
// Standard procedure to generate a graph
// Supports :
// 	-- dag option
// 	-- limited edge number by edge by post edge elimination
// 	-- adding edges (TODO, how ??)
///////////////////////////////////////

/* Random generation by the adjacency matrix method :
 * Run through the adjacency matrix
 * and at each i,j decide if matrix[i][j] is an edge by tossing a coin
 */
void gg_matrix_do(Graph& g,int num_vertices,ggen_rnd& rnd, bool do_dag)
{
	// generate the matrix
	bool matrix[num_vertices][num_vertices];
	int i,j;
	boost::any *coin = new boost::any[2];
	coin[0] = boost::any(true);
	coin[1] = boost::any(false);
	// coin toss at each (i,j)
	boost::any toss;
	for(i=0; i < num_vertices; i++)
	{
		for(j = 0; j < num_vertices ; j++)
		{
			// this test activate always if do_dag is false,
			// only if i < j if do_dag is true
			if(i < j || !do_dag)
			{
				rnd.choose(&toss,1,coin,2,sizeof(boost::any));
				matrix[i][j] = boost::any_cast<bool>(toss);
			}
			else
				matrix[i][j] = false;
		}
	}

	// translate the matrix to a graph
	g = Graph(num_vertices);
	std::map < int, Vertex > vmap;
	std::pair < Vertex_iter, Vertex_iter > vp;
	i = 0;
	for(vp = boost::vertices(g); vp.first != vp.second; vp.first++)
		vmap[i++] = *vp.first;

	for(i=0;i < num_vertices; i++)
		for(j=0; j < num_vertices; j++)
			if(matrix[i][j])
				add_edge(vmap[i],vmap[j],g);

}

/* Wrapper function aroud gg_matrix_do and edge_elmination
 * This is what we call for a generation by adjacency matrix
 */
void gg_adjacency_matrix(Graph& g, int num_vertices, int num_edges, ggen_rnd& rnd, bool do_dag)
{
	gg_matrix_do(g,num_vertices,rnd,do_dag);
	edge_limitation(g,num_edges);
}

///////////////////////////////////////
// Random Vertex Pairs
// choose randomly two vertices to add an edge to
// Supports :
// 	-- dag option
// 	-- limited edge number by edge by post edge elminitation
// 	-- adding edges (TODO, how ??)
///////////////////////////////////////


/* Random generation by choosing pairs of vertices.
*/ 
void gg_random_vertex_pairs(Graph& g,int num_vertices, int num_edges, ggen_rnd& rnd, bool allow_parallel = false, bool self_edges = false) {	

	g = Graph(num_vertices);

	// create a two arrays for ggen_rnd::choose
	boost::any *src = new boost::any[num_vertices];
	boost::any *dest = new boost::any[2];
	
	// add all vertices to src
	int i = 0;
	std::pair<Vertex_iter, Vertex_iter> vp;
	for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first)
		src[i++] = boost::any_cast<Vertex>(*vp.first);

	int added_edges = 0;
	while(added_edges < num_edges ) {
		if( !self_edges)
		{
			rnd.choose(dest,2,src,num_vertices,sizeof(boost::any));
		}
		else
		{
			// TODO rnd.shuffle
		}
		Vertex u,v;
		u = boost::any_cast<Vertex>(dest[0]);
		v = boost::any_cast<Vertex>(dest[1]);
		std::pair<Edge,bool> result = add_edge(u,v,g);
		if(!allow_parallel && result.second)
			added_edges++;
	}

	//delete src;
	//delete dest;
}

////////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////////

namespace po = boost::program_options;
dynamic_properties properties(&create_property_map);
Graph *g;

/* Main program
 */
int main(int argc, char** argv)
{
	int nb_vertices,nb_edges,mean_degree;
	bool do_dag;

	// Handle command line arguments
	////////////////////////////////////
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		
		/* I/O options */
		("output,o", po::value<std::string>(), "Set the output file")
	
		/* Generation options */
		("nb-vertices,n", po::value<int>(&nb_vertices)->default_value(10),"set the number of vertices in the generated graph")
		("nb-edges,m", po::value<int>(&nb_edges)->default_value(10),"set the number of edges in the generated graph")
		("mean-degree", po::value<int>(),"Set the mean degree expected for the graph")
		("dag", po::value<bool>(&do_dag)->zero_tokens()->default_value(false),"When possible, alter the generation method to only generate Directed Acyclic Graphs")
		/* Generation methods */
		("matrix", po::value<bool>()->zero_tokens(),"Generate with the adjacency matrix method")
	;

	po::options_description all;
	po::options_description ro = random_add_options();


	all.add(desc).add(ro);


	po::variables_map vm;
	po::store(po::parse_command_line(argc,argv,all),vm);
	po::notify(vm);
	
	if (vm.count("help")) {
		std::cout << all << "\n";
		        return 1;
	}
	
	if (vm.count("output")) 
	{
		// create a new file with 344 file permissions
		int out = open(vm["output"].as<std::string>().c_str(),O_RDWR | O_CREAT,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
	
		// redirect stdout to it
		dup2(out,STDOUT_FILENO);
		close(out);
	}

	random_options_state rs;
	random_options_start(vm,rs);
	
	// Graph options handling
	////////////////////////////////
	
	// This option force the number of edges, so if both are activated and values differ, we need to crash
	if(vm.count("mean-degree"))
	{
		if(vm.count("nb-edges"))
			if(vm["nb-edges"].as<int>() / nb_vertices != vm["mean-degree"].as<int>())
				exit(3); // TODO ERROR
		
		nb_edges = vm["mean-degree"].as<int>() / nb_vertices;
	}

	// Graph generation
	////////////////////////////////
	
	g = new Graph();
	
	if(vm.count("matrix"))
	{	
		gg_adjacency_matrix(*g,nb_vertices,nb_edges,*rs.rnd,do_dag);
	}

	//gg_random_vertex_pairs(*g,nb_vertices,nb_edges,*rs.rnd);
	
	// since we created the graph from scratch we need to add a property for the vertices
	std::string name("node_id");
	vertex_std_map *m = new vertex_std_map();
	vertex_assoc_map *am = new vertex_assoc_map(*m);
	properties.property(name,*am);
	
	int i = 0;
	std::pair<Vertex_iter,Vertex_iter> vp;
	for(vp = vertices(*g); vp.first != vp.second; vp.first++)
		put(name,properties,*vp.first,boost::lexical_cast<std::string>(i++));

	// Write graph
	////////////////////////////////////	
	write_graphviz(std::cout, *g,properties);

	random_options_end(vm,rs);
	
	delete g;
	return 0;
}
