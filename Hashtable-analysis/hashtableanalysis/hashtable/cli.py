import time
import statistics

import click
import pandas as pd

from hashtableanalysis.hashtable.hashtable import (
    Hashtable, HashtableViewer)
from hashtableanalysis.cleartext.cleartext import cleartext
from hashtableanalysis.wordgenerator.wordgenerator import wordgenerator
from . import __version__


def add_n_elemnts_from_word_generator_to_hashtable(number_of_words,
                                                   wordgenerator, hashtable):
    for __ in range(number_of_words):
        word = next(wordgenerator)
        hashtable.add(word)


@click.group()
@click.argument('rows', type=click.INT)
@click.argument('columns', type=click.INT)
@click.version_option(version=__version__)
@click.pass_context
def cli(context, rows, columns):
    """Tworzy tablicę mieszającą posiadającą ROWS wierszy oraz
    COLUMNS column.

    Args:

        rows (int): Ilość wierszy tablicy mieszającej.
        columns (int): Ilość kolumn tablicy mieszającej.
    """
    context.obj = Hashtable(rows, columns)


@click.command()
@click.argument('input', type=click.File('r'))
@click.argument('output', type=click.File('w'))
@click.pass_obj
def io(hashtable, input, output):
    """Pobiera słowa z INPUT. Potem dodaje je wszystkie do tablicy
    mieszającej. Następnie na OUTPUT zostaje wygenerowany wewnętrzny
    stan tablicy mieszającej.

    Args:
    
        input (file): Plik na podstawie którego będą generowane dane.
        output (file): Plik do którego zostanie przekazany wynik.
    """
    text = input.read()
    words = text.split('\n')

    for word in words:
        hashtable.add(word)

    viewer = HashtableViewer(hashtable)
    df = viewer.low_level()
    print(df.to_string(), file=output)


@click.command()
@click.argument('input', type=click.File('r'))
@click.argument('output', type=click.File('w'))
@click.argument('number_of_words', type=click.INT)
@click.pass_obj
def generate(hashtable, input, output, number_of_words):
    """Generuje NUMBER_OF_WORDS słow na podstawie INPUT. Potem dodaje je
    wszystkie do tablicy mieszającej. Następnie na OUTPUT zostaje 
    wygenerowany wewnętrzny stan tablicy mieszającej.

    Args:
    
        input (file): Plik na podstawie którego będą generowane dane.
        output (file): Plik do którego zostanie przekazany wynik.
        number_of_words (int): Liczba generowanych słów.
    """
    text = input.read()
    words = cleartext(text)
    word_generator = wordgenerator(words)

    add_n_elemnts_from_word_generator_to_hashtable(number_of_words,
                                                   word_generator, hashtable)

    viewer = HashtableViewer(hashtable)
    df = viewer.low_level()
    print(df.to_string(), file=output)


@click.command()
@click.argument('input', type=click.File('r'))
@click.argument('output', type=click.File('w'))
@click.argument('initial_number_of_words', type=click.INT)
@click.argument('step', type=click.INT)
@click.argument('number_of_problems', type=click.INT)
@click.argument('number_of_instances', type=click.INT)
@click.pass_obj
def benchmark(hashtable, input, output, initial_number_of_words, step,
              number_of_problems, number_of_instances):
    """Wykonuje funckję generate z pomiarem czasu dla rosnącej wartości 
    NUMBER_OF_WORDS. Na OUTPUT zostaje wygenerowana tabelka porównująca 
    czas działania funckji ze złożonością teoretyczną.

    Args:
    
        input (file): Plik na podstawie którego będą generowane dane.
        output (file): Plik do którego zostanie przekazany wynik.
        initial_number_of_words (int): Początkowa wartość NUMBER_OF_WORDS.
        step (int): Krok o ile będzie zwiększana wartość NUMBER_OF_WORDS.
        number_of_problems (int): Ilość rozwiązywanych problemów.
        number_of_insances (int): Ilość instancji problemu.
    """
    text = input.read()
    words = cleartext(text)
    word_generator = wordgenerator(words)

    n_values = []
    number_of_words = initial_number_of_words
    for __ in range(number_of_problems):
        n_values.append(number_of_words)
        number_of_words += step
    
    t_values = []
    for number_of_words in n_values:

        t_values_of_instances = []
        for __ in range(number_of_instances):
            hashtable.clear()

            start_time = time.time()
            add_n_elemnts_from_word_generator_to_hashtable(number_of_words,
                                                           word_generator, hashtable)
            end_time = time.time()

            t_value_of_instance = end_time - start_time
            t_values_of_instances.append(t_value_of_instance)
            
        mean_of_t_values_of_instances = statistics.mean(t_values_of_instances)
        t_values.append(mean_of_t_values_of_instances)

    # t(n) = O(n) =>  T(n) = n
    # 1 = t_value_for_median_index / C * theoretical_number_of_operation_for_median_index => 
    # => C = t_value_for_median_index / theoretical_number_of_operation_for_median_index
    median_index = number_of_problems // 2
    n_values_for_median_index = n_values[median_index]
    theoretical_number_of_operation_for_median_index = n_values_for_median_index
    t_value_for_median_index = t_values[median_index]
    C = t_value_for_median_index / theoretical_number_of_operation_for_median_index

    q_values = [None] * number_of_problems
    for index in range(number_of_problems):
        q_values[index] = t_values[index] / (C * n_values[index])

    data = {'n': n_values, 't(n)': t_values, 'q(n)': q_values}
    df = pd.DataFrame(data)
    print(df.to_string(), file=output)


cli.add_command(io)
cli.add_command(generate)
cli.add_command(benchmark)


if __name__ == '__main__':
    cli()
