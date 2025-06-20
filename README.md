# The Pangolin Programming Language

Pangolin is a new programming language that we developed purely for fun and experimenting. This repository contains the Pangolin tokenizer, parser, and (in the future) compiler and other supporting tools.

## Features
-  Custom syntax (see [`pangolin.ebnf`](pangolin.ebnf)).
- Lexer and parser written in C++.
- AST generation and pretty-printing.
- Unit tests using [doctest](https://github.com/doctest/doctest).

## Getting Started

### Building

compile the code by running `make` in `pangolin` 

## Running

Run Pangolin on a source file:

`./pangolin <source-file.pang>`

## Testing

Unit tests were written using doctest. To run the tests:

`make test`
`./run_tests`

## Project Structure

src/         # Source code for the compiler/interpreter
test/        # Unit tests and doctest header
[pangolin.ebnf](http://_vscodecontentref_/1) # EBNF grammar for the language
[eg.pang](http://_vscodecontentref_/2)      # Example Pangolin program

## Authors

Jakob Sereda and Ryan Liu