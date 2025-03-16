from .tsp_state import TSPState
from .min_calculator import MinCalculator


class AllMinHeuristicState(TSPState):
    '''
    this heuristic returns the minimal distance of all
    unvisited vertecies to their neighbors
    times the number of unvisited vertecies
    '''

    def __init__(self, path, length, tsp, heuristic_info=None, min_calculator=None):
        super().__init__(path, length, tsp)
        self.heuristic_info = heuristic_info
        self.min_calculator = min_calculator

    @staticmethod
    def initial_state(initial_point_index, tsp):
        min_calculator = MinCalculator(tsp)
        return AllMinHeuristicState((initial_point_index,), 0, tsp, min_calculator=min_calculator)

    def _add_point(self, point_index):
        new_state = super()._add_point(point_index)
        new_state.min_calculator = self.min_calculator

        if self.heuristic_info:
            heuristic_value, min_vertex_index = self.heuristic_info
            non_visited_len = self.tsp.get_points_count() - len(self.path)
            if min_vertex_index != point_index:
                new_heuristic = ((non_visited_len - 1) * (heuristic_value /
                                                          non_visited_len), min_vertex_index)
                new_state.heuristic_info = new_heuristic

        return new_state

    def _calculate_heuristic(self):
        if self.is_final_state():
            self.heuristic_info = (0, 0)
            return

        non_visited_vertecies = self._get_non_visited_point_idexes()
        if len(non_visited_vertecies) != 0:
            min_distance = min([(self.min_calculator[vertex], vertex)
                                for vertex in non_visited_vertecies])
        else:
            self.heuristic_info = (
                float("inf"),  # heuristic value
                -1,  # vertex index
            )
            return

        self.heuristic_info = (
            min_distance[0] * len(non_visited_vertecies),  # heuristic value
            min_distance[1],  # vertex index
        )

    def heuristic(self):
        if not self.heuristic_info:
            self._calculate_heuristic()

        return self.heuristic_info[0]
