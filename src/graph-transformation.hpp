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

#include <boost/config.hpp>
#include <boost/graph/properties.hpp>

#include "types.hpp"
#include "dynamic_properties.hpp"

#ifndef GRAPH_TRANSFORMATION_HPP
#define GRAPH_TRANSFORMATION_HPP

using namespace boost;
using namespace std;


namespace ggen {

// if there is more than one source, create a new node and make it the only source.
void add_dummy_source(Graph *g,dynamic_properties* dp,std::string name);

// if there is more than one sink, create a new node and make it the only one.
void add_dummy_sink(Graph *g,dynamic_properties* dp,std::string name);

// Remove the sources present in the graph when it is passed to the function.
// We must be carefull to not remove too many nodes
void remove_sources(Graph* g);

// Remove the sinks present in the graph when it is passed to the function.
// We must be carefull to not remove too many nodes
void remove_sinks(Graph* g);

};

#endif
