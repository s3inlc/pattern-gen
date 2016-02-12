# Pattern-Gen

[![Travis build status](https://travis-ci.org/s3inlc/pattern-gen.svg?branch=master)](https://travis-ci.org/s3inlc/pattern-gen)

Pattern-Gen is a GUI application to generate specific text patterns. This can be used on password recovery and reducing the load for brute-forcing. A lot of random generated passwords have less or no double characters, so when bruteforcing an entire keyspace can be reduced by taking out all double combinations this can lead to a big time saving.

## Example

For example, if you have a loweralpha keyspace with 8 characters:

You have 26^8 combinations = ```208'827'064'576```

Regarding combinations where you only have every character just once: ```26*25*24*23*22*21*20*19 = 62'990'928'000```

This will result in a keyspace reduce of ```145'836'136'576``` combinations which is ```145'836'136'576/208'827'064'576 = 69.84%```

## Build process

You will need the Qt development libraries and a c++ compiler. You have to call following then (small changes depending on the OS):
```
qmake pattern-gen.pro
```
```
make
```

To run the application, the normal dynamic linked libraries of Qt are required.

## How-To

Just start the application, fill in the mask you want to check, the options for the matches and then let it generate the patterns.

A detailed how-to will be written if someone is interested...
