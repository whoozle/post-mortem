# Post-Mortem - tool for malloc/free analysis

## Introduction

This is just another malloc/free/etc wrapper. It writes logs with performed actions for further analysis. Designed as thin as possible,
it writes records as is via write() call which is known to be thread/signal-safe.

Each record contains:
- type of action ('alloc', 'free')
- pointer
- calling thread id
- size (if available)
- backtrace

## Usage

LD_PRELOAD=./libpost-mortem.so <any-program>

## Convert logs to JSON

post-mortem-log <log file, default: post-mortem.log>

## What to do with all this numbers?

You could trace double-frees, alloc after free without emulation (valgrind) or instrumentation.
Analysis/visualization tool will be here soon, probably.

Have fun
