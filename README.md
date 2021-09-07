# Mapped-State-Machine

This C++ data structure is designed to ease the design and implementation of AI state based systems.

It features a nodal based map system with weighted one way links to other states.

These links are initiated based on function weight as well as an attached internal function that allowed for a return value for in depth processing.

## Example usage:

### Initialisation for a map of integer based data:
![img1](doc/img/1.png)
### Function initialisation
![img2](doc/img/2.png)
### Node addition and example function running
![img3](doc/img/3.png)
![img4](doc/img/4.png)

# TODO:

- Include overload operator templates for mulitple data types (namely overload for ==, vector sorting and print fucntions)
- Implement Dijkstra to optimize search path to other states
- Optimize addition recursion algorithm
- Add vector sorting algorithm for nodes (overload of existing vector addition) in order to sort linked nodes by weight
- Fix print function to not loop back on visited nodes
