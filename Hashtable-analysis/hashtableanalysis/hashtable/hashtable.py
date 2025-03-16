from __future__ import annotations

from collections import abc
from typing import Callable, List, Union, Tuple

import pandas as pd

from hashtableanalysis.hashtable.hashfunctions import (
    HashFunction, polynomialRollingHashFunction)


class HashtableRecord:
    hash_: int
    value: str

    def __init__(self, hash_: int, value: str):
        self.hash_ = hash_
        self.value = value

    def __eq__(self, other: HashtableRecord) -> bool:
        if not isinstance(other, HashtableRecord):
            return False

        is_hash_eq = self.hash_ == other.hash_
        is_value_eq = self.value == other.value
        return is_hash_eq and is_value_eq

    def __repr__(self) -> str:
        return f"HashtableRecord(hash='{self.hash}', value='{self.value}')"


class HashtableRowFullError(Exception):
    pass


class HashtableRowEmptyError(Exception):
    pass


class HashatbleLowLevelIterator(abc.Iterator):
    _hashtable: object
    _row: int
    _column: int

    def __init__(self, hashtable):
        self._hashtable = hashtable
        self._row = 0
        self._column = 0

    def __iter__(self) -> HashtableIterator:
        return self

    def _next_cell(self) -> Union[Tuple[HashtableRecord, int, int], None]:
        max_hashtable_row = self._hashtable._number_of_rows - 1
        if self._row > max_hashtable_row:
            raise StopIteration()

        cells = self._hashtable._cells
        result = cells[self._row][self._column]
        row = self._row
        column = self._column

        self._column += 1
        max_hashtable_column = self._hashtable._number_of_columns - 1
        if self._column > max_hashtable_column:
            self._column = 0
            self._row += 1

        return result, row, column

    def _next_record(self) -> Union[Tuple[HashtableRecord, int, int], None]:
        record, row, column = self._next_cell()
        while record is None:
            record, row, column = self._next_cell()
        return record, row, column

    def __next__(self) -> Union[Tuple[HashtableRecord, int, int], None]:
        return self._next_record()


class HashtableIterator(abc.Iterator):
    _low_level_iterator: HashatbleLowLevelIterator

    def __init__(self, hashtable):
        self._low_level_iterator = HashatbleLowLevelIterator(hashtable)

    def __iter__(self) -> HashtableIterator:
        return self

    def __next__(self):
        record, row, column = next(self._low_level_iterator)
        return record.value


class Hashtable(abc.Set):
    _cells: List[List[Union[HashtableRecord, None]]]
    _number_of_rows: int
    _number_of_columns: int
    _hash_funcion: HashFunction = polynomialRollingHashFunction
    _len: int

    def __init__(self, rows=100, columns=10):
        self._cells = [[None for __ in range(columns)] for __ in range(rows)]
        self._number_of_rows = rows
        self._number_of_columns = columns
        self._len = 0

    def add(self, string: str) -> None:
        hash_ = self._hash_funcion(string)
        new_hash_record = HashtableRecord(hash_, string)
        row = hash_ % self._number_of_rows

        try:
            column_of_first_free_record = self._cells[row].index(None)
            column = column_of_first_free_record
        except ValueError as exc:
            raise HashtableRowFullError() from exc

        self._cells[row][column] = new_hash_record
        self._len += 1

    def _indexes(self, string: str) -> Tuple[int, int]:
        hash_ = self._hash_funcion(string)
        searched_hash_record = HashtableRecord(hash_, string)
        row = hash_ % self._number_of_rows

        try:
            column_of_searched_hash_record = self._cells[row].index(searched_hash_record)
            column = column_of_searched_hash_record
        except ValueError as exc:
            raise HashtableRowEmptyError() from exc

        return row, column

    def discard(self, string: str) -> None:
        try:
            row, column = self._indexes(string)
        except HashtableRowEmptyError:
            return
        self._cells[row][column] = None
        self._len -= 1

    def clear(self) -> None:
        del self._cells
        columns = self._number_of_columns
        rows = self._number_of_rows
        self._cells = [[None for __ in range(columns)] for __ in range(rows)]

    def __contains__(self, string: str) -> bool:
        try:
            self._indexes(string)
        except HashtableRowEmptyError:
            return False
        return True

    def __len__(self) -> int:
        return self._len

    def __iter__(self) -> HashtableIterator:
        return HashtableIterator(self)

    def __repr__(self) -> str:
        return repr(self._cells)


class HashtableViewer:
    _hashtable: Hashtable

    def __init__(self, hashtable):
        self._hashtable = hashtable
    
    def low_level(self) -> pd.DataFrame:
        values = []
        rows = []
        
        columns = []
        hashes = []
        for record, row, column in HashatbleLowLevelIterator(self._hashtable):
            values.append(record.value)
            rows.append(row)
            columns.append(column)
            hashes.append(record.hash_)
        
        data = {'value': values, 'row': rows, 'column': columns, 'hash': hashes}
        df = pd.DataFrame(data)
        return df
