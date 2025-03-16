from typing import List, Generator
from random import randrange


def wordgenerator(words: List[str]) -> Generator[int, None, None]:
    while True:
        rand_number = randrange(len(words))
        yield words[rand_number]
