from .tsp_state import TSPState
from .min_calculator import MinCalculator


class MinVertexHeuristicState(TSPState):
    '''
    this heuristic returns distance to the closest vertex
    times the number of unvisited vertecies
    '''
    @staticmethod
    def initial_state(initial_point_index, tsp):
        initial_state = MinVertexHeuristicState((initial_point_index,), 0, tsp)
        initial_state.min_calculator = MinCalculator(tsp)
        return initial_state

    def _add_point(self, point_index):
        new_state = super()._add_point(point_index)
        new_state.min_calculator = self.min_calculator
        return new_state

    def heuristic(self):
        if not hasattr(self, 'heuristic_value'):
            non_visited_len = self.tsp.get_points_count() - len(self.path)
            self.heuristic_value = self.min_calculator[self.path[-1]
                                                       ] * non_visited_len
        return self.heuristic_value
