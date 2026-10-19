# Operating Systems: POSIX Coursework

CS 149 coursework and practice programs in C and Bash covering processes, pipes, file I/O, threads, and synchronization.

## Organization

- `homework_02/`: command-line integer addition.
- `homework_03/`: fork, exec, and companion shell scripts.
- `homework_05/`: pipe-based case conversion, file copying, and input fixtures.
- `homework_07/`: threaded Fibonacci generation and sorting.
- `homework_09/`: dining philosophers and sleeping teaching assistant, including an unfinished draft.
- `labs/chapter_07/`: producer-consumer implementation, header, and working notes.
- `examples/`: threaded Monte Carlo estimation of pi.
- `practice/`: C and shell exercises, including unfinished examples.

Word documents and pre-existing compiled binaries are retained locally in ignored `documents/` and `build/` directories.

## Build and run

Use a POSIX environment with a C compiler. From the repository root:

```sh
mkdir -p build
cc homework_02/sum_arguments.c -o build/sum_arguments
./build/sum_arguments 1 2 3

cc homework_05/file_copy.c -o build/file_copy
./build/file_copy homework_05/fixtures/input.txt build/copied_input.txt

cc -pthread homework_07/threaded_sort.c -o build/threaded_sort
./build/threaded_sort

cc -pthread labs/chapter_07/producer_consumer.c -o build/producer_consumer
./build/producer_consumer 5 2 2
```

For the fork/exec exercise, run from its directory so the relative shell-script paths resolve:

```sh
(cd homework_03 && cc fork_exec.c -o ../build/fork_exec && ../build/fork_exec)
```

For other C programs, compile individually; use `-pthread` for threading examples and `-lm` if the platform requires it for math functions. Some synchronization programs run continuously; stop them with Ctrl+C.

## Work in progress

These are coursework snapshots, not uniformly finished or tested programs. The `drafts/` directories contain incomplete code. `practice/pointer_basics.c` also contains an existing variable-name typo. Renaming and organizing files did not change source contents.
