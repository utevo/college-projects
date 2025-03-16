class TSPState():
    """
    GraphState represent one of the states of the graph. Class holds
    the state defining values like
        1)length of the taken path
        2)number of points visited
        3)a path(ordered list of point indexes)

    ...

    Attributes
    ----------
    path : array[point_idex]
        ordered array of point indexes representing current path
    length : float
        length of a whole path up to the last point
    """

    def __init__(self, path, length, tsp):
        '''
        Parameters
        ----------
        path : array[point]
            a path that will be represented by the graph state
        length: float
            length of a new GraphState
        tsp: Map
            Map object representing of all the points
        '''
        self.path = path
        self.length = length
        self.tsp = tsp

    def __eq__(self, other):
        if isinstance(other, self.__class__):
            return self.path == other.path and self.length == other.length
        return False

    def __lt__(self, other):
        if isinstance(other, self.__class__):
            return self.heuristic() + self.length < other.heuristic() + other.length

        return False

    def __gt__(self, other):
        if isinstance(other, self.__class__):
            return self.heuristic() + self.length > other.heuristic() + other.length

        return False

    def __str__(self):
        return str(self.path) + ', ' + str(self.length)

    @staticmethod
    def initial_state(point_index, tsp):
        return TSPState((point_index,), 0, tsp)

    def _add_point(self, point_index):
        '''
        return new GraphState with new point added to path
        Parameters
        ----------
        point_index : point index
            new point index that wants to be added to the path
        Returns
        -------
        GrapState
            new graph state with updated path
        '''
        new_path = self.path + (point_index,)

        if len(new_path) != self.tsp.dimension:
            new_length = self.length + \
                self.tsp.distance(self.path[-1], point_index)
        else:
            new_length = self.length + \
                self.tsp.distance(self.path[-1], point_index) + \
                    self.tsp.distance(self.path[0], point_index)

        return self.__class__(new_path, new_length, self.tsp)

    def _get_non_visited_point_idexes(self):
        path_set = set(self.path)
        return [point_index for point_index in range(self.tsp.get_points_count()) if point_index not in path_set]

    def is_neighbor(self, index1, index2):
        return self.tsp.distance(index1, index2) != float("inf")

    def extend(self):
        '''
        returns all of the states that can be reached from the current state
        Returns
        -------
        Array[GraphState]
            array of possible states
        '''
        return [self._add_point(point_index)
                for point_index in self._get_non_visited_point_idexes()
                if self.is_neighbor(self.path[-1], point_index)]

    def is_final_state(self):
        '''
        boolean indicating if state is a final state
        Parameters
        ----------
        Returns
        -------
        boolean
            true if state is final state false if not
        '''
        return (len(self.path) == self.tsp.get_points_count()
                and self.tsp.distance(self.path[-1], self.path[0]) != float("inf"))

    def reconstruct_path(self):
        '''
        Returns the exact ordered list of points from initial state
        to final state
        Returns
        -------
        array[point]
            array of points(2-tuples) representing the path
        '''
        return [self.tsp.get_point(point_id) for point_id in self.path]

    def heuristic(self):
        '''
        Returns heuristic value of the current state. You should override it
        '''
        return 0
