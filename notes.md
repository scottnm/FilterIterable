# clang-format integration notes

- clang-format available for windows @ https://llvm.org/builds/ as part of LLVM install

- uh-oh, did I just wipe my rust install's version of LLVM?

- Dump a base format with... `clang-format --style=llvm -dump-config > .clang-format`

- Apply .clang-format file with... `clang-format -style=file *.cpp *.h -i`

- .clang-format fixes macro continuation lines

- .clang-format doesn't seem to force reformat comments right

- `// clang-format off/on` useful for telling clang-format to shutup
