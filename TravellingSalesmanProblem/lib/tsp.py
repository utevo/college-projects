"""
Map represents the map of all points in the problem.
"""
import numpy as np


def euclidean_distance(a, b):
    '''
    Calculates the euclidean distance between two points(2-tuples)
    Parameters
    ----------
    a,b : points
        points to calculate distance between them
    Returns
    -------
    float
        euclidean distance between points
    '''
    diff_x = (a[0] - b[0])
    diff_y = (a[1] - b[1])
    return np.sqrt(diff_x*diff_x + diff_y*diff_y)


class TSP():
    def __init__(self, graphical_tsp, distance_function='euclidean'):
        '''
        Builds the map from the file given in filename kwarg
        or from array of points(2-tuples). Raises exception
        when none of arguments were specified
        '''
        if distance_function == 'euclidean':
            self.distance_function = euclidean_distance
        else:
            raise AttributeError('Function was not specified')

        self.graphical_tsp = graphical_tsp
        self.points = [node[1] for node in self.graphical_tsp.nodes]

        self._build_dist_map()

    def _build_dist_map(self):
        '''
        builds 2d array of all given points. This array
        allows to retrieve the distance from node to
        any other node in the map
        '''
        self.dimension = len(self.graphical_tsp.nodes)
        self._dist_map = np.zeros([self.dimension, self.dimension])

        nodes = self.graphical_tsp.nodes

        for i, node_1 in enumerate(nodes):
            for j, node_2 in enumerate(nodes[:i]):
                if self.graphical_tsp.are_neighbors(node_1, node_2):
                    dist = self.distance_function(node_1[1], node_2[1])
                else:
                    dist = float("inf")
                self._dist_map[i, j] = self._dist_map[j, i] = dist

    def dist_map(self):
        return self._dist_map

    def distance(self, point_index_1, point_index_2):
        '''
        Returns the distance between two points identified by their id's
        Parameters
        ----------
        point_index_1,point_index_2 : int
            id's of the points
        Returns
        -------
        float
            distance between two points
        '''
        return self._dist_map[point_index_1, point_index_2]

    def get_point(self, index):
        return self.points[index]

    def get_points_count(self):
        return len(self.points)


