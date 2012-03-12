Building a tracking server like google analytics: Go or Ruby? Micro benchmarks doesn't answer the real world questions. What we choose depends on not only the language, but also the packages we may use.

Here's hello-world-tracking-server code for each language using proper packages.

Template engine is not used because tracking server doesn't need it (The surprising fact: Ruby Slim is 3 times the speed of Go Template in one of my benchmarks, and Ruby ERB is even faster).

## Requirement

- head go
- head ruby
- mongodb
- [bzr](http://bazaar-vcs.org/)

## Build

```sh
go get launchpad.net/mgo
go build server.go db.go
gem i thin mongo bson_ext
```

## Config

``` sh
cp config.json.example config.json
```

change your.server.name

## Start server

``` sh
./server
```

``` sh
thin start -p 8080
```

## Rough compare

``` sh
httperf --server=your.server.name --port=8080 --num-calls=100 --num-conns=100
```

On my machine:

Go: ~10000 req/s, whether use go routine doesn't improve

Ruby: ~6000 req/s

Bottleneck lies in IO, both Memory / Cpu usage are neglectable, so no need to worry go's stop-the-world GC problem.

For deployment, ruby still needs a front-end to make use of all the cores. But for either language, network IO is the real problem you have to handle before using the second core.
