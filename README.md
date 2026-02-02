# hackable-pager

hackable-pager is a small pager written in C using ncurses.

---

## What is this?

This project is a minimal text/code viewer that:

- reads a file line by line into memory
- renders it manually using ncurses
- allows scrolling
- applies very basic C syntax highlighting

---

## Why does this exist?

I just wanted to make a pager in C... However, the possible uses I think this has are:
- A kernel for an advanced pager OR text/code editor
- Learning ncurses
- Messing around

---

## Features

- Full-screen ncurses rendering
- Vertical scrolling
  - `j` / `k`
  - arrow keys
- Fixed-width line number gutter
- Very basic C syntax highlighting:
  - keywords
  - string literals
  - `//` line comments
- Straightforward codebase

---

## Building

You’ll need ncurses installed.

```sh
gcc pager.c -lncurses -o pager
```

## Usage
```sh
./pager [path/to/.c/file
```
Controls:
- j / ↓ — scroll down
- k / ↑ — scroll up
- q — quit
