# database-engine

This project is an educational database engine written in C. It takes
PostgreSQL's simple page based storage model as a reference. The code
here is intentionally small and serves as a starting point for further
development.

See [docs/overview.md](docs/overview.md) for a high level tour of the
repository structure and build instructions.

## Building

A simple Makefile is provided. Run `make` to build the demo executable:

```sh
make
./db
```

The executable demonstrates basic usage of the `Page` structure and a very
simple SQL tokenizer found in `parser/`.
