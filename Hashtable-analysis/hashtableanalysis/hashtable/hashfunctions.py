from typing import Callable


class HashFunction(Callable):
    def __call__(self, string: str) -> int:
        pass


class PolynomialRollingHashFunction(HashFunction):
    M = 10**9 + 9
    P = 31

    def __call__(self, string: str) -> int:
        result = 0
        multiplier = 1

        for char in string:
            result = (result  + ord(char) * multiplier) % self.M
            multiplier = (multiplier * self.P) % self.M
        
        return result

polynomialRollingHashFunction = PolynomialRollingHashFunction()
