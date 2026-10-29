# DodgeGame

An educational game written in C++ with raylib. The player dodges projectiles that spawn outside the field and travel towards random points inside it. A single hit ends the run. This project explores game development, Git, and simple evolutionary optimization of an agent's behavior.

## Build and run

You need C and C++ compilers, CMake 3.24 or newer, and the dependencies required to build raylib. On the first build, CMake downloads raylib 6.0 using FetchContent, so an internet connection is required.

From the project root:

```bash
cmake -S . -B build
cmake --build build
./build/DodgeGame
```

Run the program from the project root: the path to `best_agent.txt` is relative to the working directory.

## Modes

Choose the mode using the variables at the beginning of `src/main.cpp`. Rebuild the program after changing them.

- `simulationMode = true` — optimize and evaluate the agent without opening a window. The best coefficient is written to `best_agent.txt`, overwriting its previous value.
- `simulationMode = false`, `agentMode = true` — watch the agent play in a window using the coefficient from `best_agent.txt`.
- `simulationMode = false`, `agentMode = false` — play in a window using **WASD** to move and **R** to restart after losing.

## Agent and coefficient optimization

The agent finds the nearest projectile and chooses a direction away from it. A direction towards the center of the field is added, weighted by the `centerWeight` coefficient.

The program evaluates a coefficient by the average survival time over several runs. It then creates a candidate by applying a small random mutation to the current best coefficient. If the candidate performs better, it becomes the new best. Simulation runs use a fixed time step and a time limit.

The same random seeds are used to evaluate each candidate. The final comparison uses a separate set of seeds that was not used during optimization.

## Experiment results

In the recorded experiment, projectiles spawned every 0.2 seconds. Each agent was evaluated over 50 runs using seeds `100–149`. The simulation time step was 1/60 second, and the time limit was 100 seconds per run.

| Coefficient | Average survival time |
| --- | --- |
| 0 - no pull towards the center | 13.7899 s |
| 0.0873106 - optimized coefficient | 27.6218 s |

These results describe one experiment; they may differ if the settings or algorithm change. The results are also recorded in `src/results.txt`.
