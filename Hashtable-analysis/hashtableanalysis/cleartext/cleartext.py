from typing import List
import unicodedata


def is_letter(char: str) -> bool:
    if len(char) != 1:
        raise TypeError('char need to be character')

    if unicodedata.category(char) in ('Lu', 'Ll'):
        return True
    return False


def repair_string(string: str) -> str:
    no_letter_on_front_counter = 0
    for char in string:
        if not is_letter(char):
            no_letter_on_front_counter += 1
        else:
            break

    no_letter_on_back_counter = 0
    for char in reversed(string):
        if not is_letter(char):
            no_letter_on_back_counter += 1
        else:
            break

    begin = no_letter_on_front_counter
    end = len(string) - no_letter_on_back_counter
    
    if begin >= end:
        return []
    return string[begin:end]


def is_word(string: str) -> bool:
    if len(string) < 2:
        return False
    
    for char in string:
        if not is_letter(char):
            return False
    return True


def cleartext(text: str) -> List[str]:
    strings = text.split()
    repaired_strings = map(repair_string, strings)
    return list(filter(is_word, repaired_strings))
