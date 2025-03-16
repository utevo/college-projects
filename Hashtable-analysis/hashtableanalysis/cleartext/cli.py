import sys

import click

from hashtableanalysis.cleartext.cleartext import cleartext
from . import __version__


@click.command(name='cleartext')
@click.argument('input', type=click.File('r'))
@click.argument('output', type=click.File('w'))
@click.version_option(version=__version__)
def cli(input, output):
    """Pobiera słowa z INPUT oczyszcza je i zwraca
    do OUTPUT.

    \b
    Czyści słowa z stdin i zwraca je do stdout:
        cleartext - -
    
    \b
    Czyści słowa z pliku foo.txt i zwraca je do pliku bar.txt:
        cleartext foo.txt bar.txt
    """
    dirty_text = input.read()
    words = cleartext(dirty_text)
    words_splited_by_new_line = '\n'.join(words)

    output.write(words_splited_by_new_line)
    output.write('\n')


if __name__ == '__main__':
    cli()
