# Pattern-Gen

[![Travis build status](https://travis-ci.org/s3inlc/pattern-gen.svg?branch=master)](https://travis-ci.org/s3inlc/pattern-gen)

Pattern-Gen is a GUI application to generate specific text patterns. This can be used on password recovery and reducing the load for brute-forcing. A lot of random generated passwords have less or no double characters, so when bruteforcing an entire keyspace can be reduced by taking out all double combinations this can lead to a big time saving.

Example to come....

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
