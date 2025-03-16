"""Tests for hashtableanalysis.hashtable.hashfunction"""

from hashtableanalysis.hashtable.hashfunctions import polynomialRollingHashFunction


def test_polynomial_rolling_hash_function():
    assert polynomialRollingHashFunction('12345') == 50546355
    assert polynomialRollingHashFunction('Hello World') == 840387480
    assert polynomialRollingHashFunction('VeryVeryVeryVeryVeryLongString') == 462550199
    assert polynomialRollingHashFunction('We interrupt this program to annoy you and make things generally more irritating.') == 820237747