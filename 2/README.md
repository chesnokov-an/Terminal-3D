# Lab Work №2: Class template devector.

> Special variant

## Description:

A vector-like sequence container providing front and back operations (e.g: push_front/pop_front/push_back/pop_back) with amortized constant complexity and unsafe methods geared towards additional performance.

Models the SequenceContainer, ReversibleContainer, and AllocatorAwareContainer concepts.

## How to install and run a project:
1. Clone the project repository:
```bash
git clone https://git.dozen.mephi.ru/oop2025/s24501_chesnokov.an.git
```

2. Compile the project:
```bash
cd s24501_chesnokov.an/2/
mkdir build
cd build
cmake ..  // This command will generate makefiles for each target in our project inside the build folder.
          // The project itself is located one folder higher.
make Tests     // Compiles the Tests program
```

3. Run:
```bash
./build/Tests/Tests  // Runs the Tests program
```

4. Generate a coverage report:
```bash
gcovr -r . -f ".*Devector\.hpp" --html-details coverage.html
```

<!-- 
4. Generate documentation:
```bash
doxygen Doxyfile
```
To view the documentation, open the file `docs/html/index.html`. -->