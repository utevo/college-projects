"""Tests for hashtableanalysis.hashtable.hashtable"""

from hashtableanalysis.hashtable.hashtable import Hashtable


def test_hashtable_add():
    hashtable = Hashtable()
    string = 'Ambroży'

    assert string not in hashtable 
    hashtable.add(string)
    assert string in hashtable


def test_hashtable_discard():
    hashtable = Hashtable()
    string = 'Ambroży'

    assert string not in hashtable 
    hashtable.discard(string)
    assert string not in hashtable

    hashtable.add(string)
    assert string in hashtable
    hashtable.discard(string)
    assert string not in hashtable
    hashtable.discard(string)
    assert string not in hashtable


def test_hashtable_len():
    hashtable = Hashtable()
    string_1 = 'Ambroży'
    string_2 = 'Kleks'

    assert len(hashtable) == 0
    hashtable.add(string_1)
    assert len(hashtable) == 1
    hashtable.add(string_2)
    assert len(hashtable) == 2

    hashtable.discard(string_1)
    assert len(hashtable) == 1
    hashtable.discard(string_1)
    assert len(hashtable) == 1

    hashtable.discard(string_2)
    assert len(hashtable) == 0
    hashtable.discard(string_2)
    assert len(hashtable) == 0


def test_hashtableiterator_init():
    hashtable = Hashtable()

    iterator = iter(hashtable)
    assert iterator == iter(iterator)


def test_hashtableiterator_next():
    hashtable = Hashtable()
    string_1 = 'Ambroży'
    string_2 = 'Kleks'
    string_3 = 'Mucholot'
    hashtable.add(string_1)
    hashtable.add(string_2)
    hashtable.add(string_3)

    iterator = iter(hashtable)
    strings = set()
    try:
        while True:
            string = next(iterator)
            strings.add(string)
    except StopIteration:
        pass

    assert strings == {string_1, string_2, string_3}


def test_hashtableiterator_iteratrion_by_for_loop():
    hashtable = Hashtable()
    string_1 = 'Ambroży'
    string_2 = 'Kleks'
    string_3 = 'Mucholot'
    hashtable.add(string_1)
    hashtable.add(string_2)
    hashtable.add(string_3)

    strings = set()
    for string in hashtable:
        strings.add(string)

    assert strings == {string_1, string_2, string_3}


def test_hashtable_clear():
    hashtable = Hashtable()
    string_1 = 'Ambroży'
    string_2 = 'Kleks'
    string_3 = 'Mucholot'
    hashtable.add(string_1)
    hashtable.add(string_2)
    hashtable.add(string_3)

    hashtable.clear()
    strings = set()
    for string in hashtable:
        strings.add(string)

    assert strings == set()


def test_hashtable_clear_vol_2():
    hashtable = Hashtable()
    string_1 = 'Ambroży'
    string_2 = 'Kleks'
    string_3 = 'Mucholot'
    hashtable.add(string_1)
    hashtable.add(string_2)
    hashtable.add(string_3)

    hashtable.clear()
    hashtable.add(string_1)
    hashtable.add(string_2)

    assert string_1 in hashtable
    assert string_2 in hashtable
    assert string_3 not in hashtable


def test_hashtable_clear_vol_3():
    rows = 10
    columns = 3
    hashtable = Hashtable(rows, columns)
    string_1 = 'Ambroży'
    string_2 = 'Kleks'
    string_3 = 'Mucholot'
    hashtable.add(string_1)
    hashtable.add(string_2)
    hashtable.add(string_3)

    hashtable.clear()
    
    assert hashtable._number_of_rows == rows
    assert hashtable._number_of_columns == columns
