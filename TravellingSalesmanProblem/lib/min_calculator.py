
class MinCalculator():
    """
    calculator that calculates the shortest distance to neighbor vertecies
    for every vertex in tsp
    """

    def __init__(self, tsp):
        self._calculate_min_distances(tsp)

    def _calculate_min_distances(self, tsp):
        self.min_distances = [min([tsp.distance(i, j) for j in range(tsp.get_points_count()) if i != j])
                              for i in range(tsp.get_points_count())]
        # for i in range(tsp.get_points_count()):
        #     distances = []
        #     for j in range(tsp.get_points_count()):
        #         if i != j:
        #             distances.append(tsp.distance(i,j))
        #     min_distance.append(min(distances))

        # for i in range(tsp.get_points_count()):
        #     min_distance.append(min([tsp.distance(i,j) for j in range(tsp.get_points_count())]))

        # min_dist

    def __getitem__(self, index):
        return self.min_distances[index]
