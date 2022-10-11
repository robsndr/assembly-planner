# Assembly Planner

The assembly planner provides a performant framework that can be used for **allocating workloads** in **collaborative human-robot assembly planning** problems.
It employs AND/OR graphs to model industrial assembly procedures, and differentiates between agents using cost functions.

## Description

The combination of human cognitive reasoning with skills of industrial robots makes it possible to utilize robots as collaborative agents in increasingly complex environments. In the following, an industrial assembly process is considered, where most of the tasks can be done by singular entities, but some might require assistance from other agents. AND/OR graphs are used as an abstract world model, with AND/OR nodes representative for tasks [**actions**] and stages of the assembly process [**subassemblies**] respectively. A full **assembly plan** is obtained, when all possibilities of how to assemble an object are introduced into the AND/OR model. A small-scale example of an assembly plan `P` is illustrated [here](https://dropbox.com/s/z6evjq3v3xcetkq/inputdot.png?raw=1), with rectangular nodes used to depict subassemblies, and round nodes mapped to actions.


The planning problem strives to find a sub-graph of the top-level assembly plan, with an optimal agent-action assignment and maximum task parallelism. The search is driven by a **cost function**, that can be used to model the various abilities of different agents. For example, if `a4` in `P` denotes the task of lifting an object, the cost might be high or infeasible [`inf`] for a human, but quite cheap for a robot. An example cost function for a scenario with 2 robots [`r1`, `r2`], 1 human `h1` and assembly `P` is provided below:

|    | a1  |  a2 |  a3 |  a4 | a5  | a6  |  a7 |  a8 | a9 | a10 | a11 | a12 |
|----|-----|-----|-----|-----|-----|-----|-----|-----|----|-----|-----|-----|
| r1 | inf | inf | inf |  10 | 15  | 20  |  10 | inf | 20 |  10 | 10  |  10 |
| r2 | inf | inf | inf |  20 |  5  | 10  |   5 |   5 | 20 |  10 | 10  |  10 |
|  h | 50  |  50 |  50 | 200 | 50  | 30  |  40 | 100 | 50 |  50 | 100 | 100 |

For some cases, even though an agent can perform a given task, it might need some assistance to do so. Let's suppose that a certain tool is required to deal with the `H` subassembly in `P`, before executing task `a12`. Agent `r1` might not be equipped with that component, making `a12` only feasible for `r2` under the assumption that `r1` or `h` provide support. This ability of agents to directly deal with subassemblies is regarded as `reachability`, with possible assistance modeled as `interactions`. An example specification of reachability for the `H` subassembly with the corresponding interaction cost is shown below:

<table>
<tr><th>Reachability </th><th>Interaction</th></tr>
<tr><td>

|          |  H  |
|----------|-----|
| Reach r1 |  i0 |
| Reach r2 |   ✓ |
| Reach  h |   ✓ |

</td><td>

|    |  i0  |
|----|------|
| r1 |  inf |
| r2 |  12  |
|  h |  10  |


</td></tr> </table>

The search for the optimal solution is executed in an online manner, using a divide-conquer approach to handle the combinatorial explosion of the search space. An example input assembly is provided under `./example/assembly.xml`, with the corresponding output in `./example/output.xml`. A visual representation of the optimum plan found for the example assembly is shown [here](https://dropbox.com/s/humaw2lnxed105y/outdot.png?raw=1).

## Getting Started

The assembly planner depends on the `argparse` and `tinyxml2` C++ libraries, which are referenced in the repository as submodules.

```bash
$ git clone git@github.com:symm3try/assembly-planner.git --recurse-submodules
```

### Running
The seach itself can be run either directly on a host, or within a docker container.

```bash
$ cmake . && make
$ ./planner ./example/assembly.xml ./example/plan.xml
```

Running using Docker:

```bash
$ docker build -t planner .
$ docker run --rm -v ${PWD}/example/:/data planner /data/assembly.xml /data/plan.xml
```

## Acknowledgments

<a id="1">[1]</a>
L. S. Homem de Mello and A. C. Sanderson, “And/or graph representation of assembly plans,” Robotics and Automation, IEEE Transactions on, vol. 6, no. 2, pp. 188–199, 1990 \
<a id="2">[2]</a> L. Johannsmeier and S. Haddadin, "A Hierarchical Human-Robot Interaction-Planning Framework for Task Allocation in Collaborative Industrial Assembly Processes," in IEEE Robotics and Automation Letters, vol. 2, no. 1, pp. 41-48, Jan. 2017 \
<a id="3">[3]</a> L. S. H. De Mello and A. C. Sanderson, “A correct and complete algorithm for the generation of mechanical assembly sequences,” Robotics and Automation, IEEE Transactions on, vol. 7, no. 2, pp. 228–240, 1991
