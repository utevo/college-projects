from .tsp_solver import TSPSolver
from .tsp_state import TSPState
import heapq


class AStarTSPSolver(TSPSolver):
    def __init__(self, tsp):
        self.tsp = tsp

    def solve(self, state_class=TSPState):
        initial_state = state_class.initial_state(1, self.tsp)
        pqueue = [initial_state]
        visited_count = 0
        while len(pqueue) != 0:
            current = heapq.heappop(pqueue)
            visited_count += 1
            if current.is_final_state():
                return current.reconstruct_path(), current.length, visited_count

            if current.length == float("inf"):
                return -1, float('inf'), float('inf')


            for state in current.extend():
                heapq.heappush(pqueue, state)

        return -1, float('inf'), float('inf')
