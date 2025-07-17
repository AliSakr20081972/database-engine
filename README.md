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

The executable demonstrates usage of the storage layer, a small SQL tokenizer
and newly added components including a minimal query planner and executor,
a thread pool and a simple user authentication module. Basic error handling
now reports failures such as thread pool initialization problems to STDERR.

After these examples run, the program starts a very small interactive
command line interface. You can enter SQL statements such as
`SELECT * FROM table1;` or `INSERT INTO table1 VALUES ('abc');` and type
`quit` to exit.

## Running with Docker

Build the Docker image and run the demo executable inside a container:

```sh
docker build -t database-engine .
docker run -it --rm database-engine
```
