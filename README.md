# How to run:

1. `clang -luv main.c` or `sbt nativeLink` (depending on if `go.main` is set to run the C or the Scala subprocesses)
2. `go run main.go`

You should see a very friendly program that prints what it is doing for you, but you probably must read the source to understand what is going on.

# Related discussions:

- https://github.com/libuv/libuv/discussions/3732
- https://discord.com/channels/632150470000902164/635668881951686686/1014899473643749426
