from lib.graphical_tsp import GraphicalTSP
from lib.brute_tsp_solver import BruteTSPSolver
from lib.astar_tsp_solver import AStarTSPSolver
from lib.min_vertex_heuristic_state import MinVertexHeuristicState
from lib.all_min_distance_state import AllMinHeuristicState
from lib.mean_distance_state import MeanDistanceState
from lib.tsp import TSP

import random
import time
import sys


def clocking_function(function):
    start = time.time()
    result = function()
    end = time.time()
    
    work_time = end - start
    return work_time, result


def main():
    with open("data/Norway/NodesNorway.txt") as f:
        nodes = f.read()

    with open("data/Norway/LinksNorway.txt") as f:
        links = f.read()

    graphical_tsp = GraphicalTSP(nodes, links)
    tsp = TSP(graphical_tsp)

    brute = BruteTSPSolver(tsp)
    astar = AStarTSPSolver(tsp)
    
    print('Warning! Calculations can take a long time.\n')

    time_min_from_vertex, result_min_from_vertex = clocking_function(
        lambda: astar.solve(MinVertexHeuristicState))
    print('A* with "min from vertex" heurestic:')
    print(f'\ttime:   {time_min_from_vertex}')
    print(f'\tresult: {result_min_from_vertex[1]}')
    print()

    time_mean, result_mean = clocking_function(
        lambda: astar.solve(MeanDistanceState))
    print('Astar with "mean" heurestic:')
    print(f'\ttime:   {time_mean}')
    print(f'\tresult: {result_mean[1]}')
    print()

    time_all_min_from_vertex, result_all_min_from_vertex = clocking_function(
        lambda: astar.solve(AllMinHeuristicState))
    print('Astar with "all min from vertex" heurestic:')
    print(f'\ttime:   {time_all_min_from_vertex}')
    print(f'\tresult: {result_all_min_from_vertex[1]}')
    print()

    time_zero, result_zero = clocking_function(
        lambda: astar.solve())
    print('Astar with "zero" heuristic:')
    print(f'\ttime:   {time_zero}')
    print(f'\tresult: {result_zero[1]}')
    print()

    time_brute, result_brute = clocking_function(lambda: brute.solve())
    print('Brute:')
    print(f'\ttime:   {time_brute}')
    print(f'\tresult: {result_brute[1]}')
    print()

if __name__ == '__main__':
    main()
