from ggen import lib
import pygsl.rng

class client():
    """Client for GGen library.

    Returns a wrapper for the GGen library that always uses the given
    PyGSL random number generator. If the random number generator is
    not explicitly given, the client will choose a default generator
    and seed from the environment variables GSL_RNG_TYPE and
    GSL_RNG_SEED. If GSL_RNG_TYPE is not specified, then
    gsl_rng_mt19937 is used as the default and its seed is set to
    zero."""

    def __init__(self, pygsl_rng=None):
        """Initializes the client using pygsl_rng as the random number generator.

        If pygsl_rng is not given or is None, then GSL's environment defaults are used."""

        if pygsl_rng == None:
            pygsl.rng.env_setup()
            self._rng = pygsl.rng.rng()
        else:
            self._rng = pygsl_rng

    def gnp(self, n, p):
        """Erdos and Renyi's classical adjacency matrix method.

        Returns a random graph where each possible edge of a graph
        with 'n' vertices is choosed with probability 'p'."""

        return lib.generate_erdos_gnp(self._rng, n, p)

    def gnm(self, n, M):
        """Erdos and Renyi's fixed number of edges method.

        Returns a random graph with 'n' vertices and 'M' edges uniformly
        choosed from the complete graph."""

        return lib.generate_erdos_gnm(self._rng, n, M)

    def lbl(self, n, p, number_of_layers):
        """Layer-by-Layer method.

        Returns a random graph with 'n' vertices splitted between a
        fixed 'number_of_layers' where vertices from different layers
        can be connected with probability 'p'."""

        return lib.generate_erdos_lbl(self._rng, n, p, number_of_layers)

    def fifo(self, n, out_degree, in_degree):
        """Fan-in / Fan-out method.

        Returns a 'n' vertices random graph built a graph by a
        succession of fan-in and fan-out steps, where 'out_degree' is
        the maximum out-degree allowed for any vertice and 'in_degree'
        is the maximum in-degree allowed."""

        return lib.generate_fifo(self._rng, n, out_degree, in_degree)

    def random_orders(self, n, orders):
        """Random-Orders method.

        Returns a random DAG with 'n' vertices built from a partial
        order obtained by the intersection of 'orders' random total
        orders."""

        return lib.generate_random_orders(self._rng, n, orders)
