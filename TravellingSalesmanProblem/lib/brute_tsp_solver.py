from collections import deque

from lib.tsp_state import TSPState
from lib.tsp_solver import TSPSolver


class BruteTSPSolver(TSPSolver):
    def __init__(self, tsp):
        self.initial_state = TSPState.initial_state(1, tsp=tsp)

    def solve(self):
        best_trip = None
        queue = deque([self.initial_state])

        visited_count = 0
        while len(queue) > 0:
            current = queue.pop()
            visited_count += 1

            if current.is_final_state():
                if best_trip == None or current.length < best_trip.length:
                    best_trip = current

            queue.extend(current.extend())

        if best_trip:
            return best_trip.reconstruct_path(), best_trip.length, visited_count

        return [], float("inf"), float("inf")
