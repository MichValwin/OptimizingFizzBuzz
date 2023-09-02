# Install python dependencies
Use a venv if you see fit.
Then do:
`pip install -r requirements.txt`

# To compile the c++ code 
It is expected that you are in a system that could execute .sh bash files and that GCC is installed.

# Measure using perf and flamegraph
Use `perf record --call-graph dwarf ./program` to create a data file with all of the performance metrics.
Then use https://github.com/brendangregg to make the flamegraph
Example command: `perf script | ./stackcollapse-perf.pl |./flamegraph.pl --title "ToString3 flamegraph" --width 4000 > perfWString3.svg`

