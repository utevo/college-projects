import sys

import click

from hashtableanalysis.cleartext.cleartext import cleartext
from hashtableanalysis.wordgenerator.wordgenerator import wordgenerator
from . import __version__


@click.command()
@click.version_option(version=__version__)
@click.argument('input', type=click.File('r'))
@click.argument('output', type=click.File('w'))
@click.argument('number_of_words', type=click.INT)
def cli(input, output, number_of_words):
    """Generuje NUMBER_OF_WORDS słów na podstawie INPUT do OUTPUT

    \b
    Generuje 10 słów na podstawie stdin do stdout:
        wordgenerator - - 10
    \b
    Generuje 30 słów na podstawie pliku foo.txt do stdout:
        wordgenerator foo.txt - 30
    \b
    Generuje 50 słów na podstawie stdio do pliku bar.txt:
        wordgenerator - bar.txt 50
    """
    text = input.read()
    words = cleartext(text)

    if not words:
        raise ValueError("In input there aren't any words.")

    word_generator = wordgenerator(words)
    generated_words = []

    for __ in range(number_of_words):
        generated_word = next(word_generator)
        generated_words.append(generated_word)

    output.writelines('\n'.join(generated_words))
    output.write('\n')


if __name__ == '__main__':
    cli()