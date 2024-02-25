Benchmark for splitting strings with a delimiter in C

- `iter`
  Management of iteration states in the function stack
- `iter-struct`
  The iterator struct and its associated functions (methods)
- `strtok`
  Standard C library function for tokenizing a string

# Benchmark

Benchmark for the binaries compiled with `-O3`:

```
$ make -s bench 2>/dev/null
Benchmark 1: bin/sols-iter
  Time (mean ± σ):      22.8 ms ±   2.6 ms    [User: 21.0 ms, System: 1.6 ms]
  Range (min … max):    21.7 ms …  51.3 ms    129 runs

Benchmark 2: bin/sols-iter-struct
  Time (mean ± σ):      22.6 ms ±   1.1 ms    [User: 21.0 ms, System: 1.4 ms]
  Range (min … max):    21.8 ms …  32.1 ms    134 runs

Benchmark 3: bin/sols-strtok
  Time (mean ± σ):      31.9 ms ±   1.6 ms    [User: 30.1 ms, System: 1.7 ms]
  Range (min … max):    29.8 ms …  40.1 ms    90 runs

Summary
  bin/sols-iter-struct ran
    1.01 ± 0.13 times faster than bin/sols-iter
    1.42 ± 0.10 times faster than bin/sols-strtok
Benchmark 1: bin/smss-iter
  Time (mean ± σ):      21.4 ms ±   0.3 ms    [User: 20.8 ms, System: 0.4 ms]
  Range (min … max):    20.6 ms …  22.5 ms    140 runs

Benchmark 2: bin/smss-iter-struct
  Time (mean ± σ):      21.5 ms ±   0.6 ms    [User: 20.9 ms, System: 0.4 ms]
  Range (min … max):    20.7 ms …  26.4 ms    141 runs

Benchmark 3: bin/smss-strtok
  Time (mean ± σ):      30.7 ms ±   1.6 ms    [User: 30.2 ms, System: 0.4 ms]
  Range (min … max):    28.9 ms …  37.5 ms    98 runs

Summary
  bin/smss-iter ran
    1.01 ± 0.03 times faster than bin/smss-iter-struct
    1.44 ± 0.08 times faster than bin/smss-strtok
```

In the benchmark, `iter` and `iter-struct` were roughly 1.4 times faster than
`strtok`. Be warned that these were the result on my machine, and also that the
input strings are kind of contrived. We should not take this to imply that
`strtok` is slower **in general**.

# Test Failure

When we run tests for `sols` (split one long string) and `smss` (split many
short strings), they fail with the output suggesting that the `strtok` version
produced a different output:

```
$ make test-sols
gcc -Wall -Wextra -O3 -o bin/sols-iter split-one-long-string/iter.c
gcc -Wall -Wextra -O3 -o bin/sols-iter-struct split-one-long-string/iter-struct.c
diff -quB --from-file <(./bin/sols-iter) <(./bin/sols-iter-struct) <(./bin/sols-strtok)
Files /dev/fd/63 and /dev/fd/61 differ
make: *** [Makefile:58: test-sols] Error 1
$ make test-smss
gcc -Wall -Wextra -O3 -o bin/smss-iter split-many-short-strings/iter.c
gcc -Wall -Wextra -O3 -o bin/smss-iter-struct split-many-short-strings/iter-struct.c
gcc -Wall -Wextra -O3 -o bin/smss-strtok split-many-short-strings/strtok.c
diff -quB --from-file <(./bin/smss-iter) <(./bin/smss-iter-struct) <(./bin/smss-strtok)
Files /dev/fd/63 and /dev/fd/61 differ
make: *** [Makefile:58: test-smss] Error 1
```

This is **intentional**. Let's see what is different about them in the `sols`
case:

```
$ make --silent run > /dev/null
$ wc --lines --total never output/sols-{iter{,-struct},strtok}
1650689 output/sols-iter
1650689 output/sols-iter-struct
1650688 output/sols-strtok
$ diff --from-file output/sols-strtok \
<(head -1650688 output/sols-iter) \
<(head -1650688 output/sols-iter-struct)
$ tail -n 3 output/sols-{iter{,-struct},strtok}
==> output/sols-iter <==
z
z


==> output/sols-iter-struct <==
z
z


==> output/sols-strtok <==
z
z
z
```

As you can see, `strtok` version does not end with an extra white space. This
is because `strtok` stops its execution when the last token is the empty
string. The input string as defined in `bench.h` ends with a comma as a
delimiter like so: "...z,z,z,". This behavior is indistinguishable from when
the input string ends without a comma: "...z,z,z". I decided to include the
last token in my implementation of `iter` and `iter-struct`, so that is why it
outputs an excessive empty line at the end.

For the `smss` test, you can see that the `strtok` version does not output any
white spaces:

```
$ grep '^$' output/smss-iter | wc -l
1024
$ grep '^$' output/smss-iter-struct | wc -l
1024
$ grep '^$' output/smss-strtok | wc -l
0
$ diff --ignore-blank-lines --from-file output/smss-{iter{,-struct},strtok}
```

In the implementation of `iter` and `iter-struct`, you can get this behavior by
not handling the `finish` case, thereby freeing a byte-length space in the
stack previously occupied by a boolean flag. I would not necessarily say this
is a bug in `strtok`. It is probably more apt to say that the initial interface
ended up being exposed as a public API without caring too much about this
corner case in mind.
