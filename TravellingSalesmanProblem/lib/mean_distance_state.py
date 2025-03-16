from .tsp_state import TSPState
import numpy as np


class MeanDistanceState(TSPState):
    '''
    this heuristic returns distance to the closest vertex
    times the number of unvisited vertecies
    '''

    def __init__(self, path, length, tsp, mean=None):
        super().__init__(path, length, tsp)
        self.mean = mean

    @staticmethod
    def initial_state(initial_point_index, tsp):
        distances = np.extract(tsp._dist_map != float("inf"), tsp._dist_map)
        mean = np.mean(distances)
        return MeanDistanceState((initial_point_index,), 0, tsp, mean)

    def _add_point(self, point_index):
        new_state = super()._add_point(point_index)
        new_state.mean = self.mean
        return new_state

    def heuristic(self):
        if not hasattr(self, 'heuristic_value'):
            non_visited_len = self.tsp.get_points_count() - len(self.path)
            self.heuristic_value = non_visited_len * self.mean
        return self.heuristic_value
