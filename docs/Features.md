# FuzzDistill Dataset Explanation

The final Basic Block and Function features are extracted in Semicolon-Separated Values (SSV) format.

### Function Features

The generated dataset is list of functions with various characteristics and a label indicating whether each function is
vulnerable or not. The data is structured into 14 columns, which are described below:

1. **Function ID**: A unique identifier for each function.
2. **Function Name**: The name of the function.
3. **Instructions**: The number of instructions (e.g.,Intermediate Instructions) in the function.
4. **BBs** (Basic Blocks): The number of basic blocks in the function. A basic block is a sequence of instructions that
   are executed together without any control flow changes.
5. **In-degree**: The number of incoming edges to the function in the call graph, indicating how many other functions
   call this one.
6. **Out-degree**: The number of outgoing edges from the function in the call graph, indicating how many other functions
   are called by this one.
7. **Num Loops**: The number of loops (e.g., for, while, do-while) present in the function.
8. **Static Allocations**: The number of static memory allocations made by the function.
9. **Dynamic Allocations**: The number of dynamic memory allocations made by the function (e.g., using `malloc`,
   `realloc`).
10. **MemOps** (Memory Operations): The number of memory-related operations performed by the function (e.g., reads,
    writes).
11. **CondBranches** (Conditional Branches): The number of conditional branches (e.g., if-else statements) in the
    function.
12. **UnCondBranches** (Unconditional Branches): The number of unconditional branches (e.g., jumps, returns) in the
    function.
13. **DirectCalls**: The number of direct function calls made by the function.
14. **InDirectCalls** (Indirect Calls): The number of indirect function calls made by the function (e.g., through a
    pointer or a table).
15. **VULNERABLE**: A binary label indicating whether the function is vulnerable (1) or not (0).

### Basic Block Features

Generated Basic Block dataset a collection of basic blocks (BBs) from functions with various characteristics and a label
indicating whether each block is vulnerable or not. The data is structured into 13 columns, which are described below:

1. **Block ID**: A unique identifier for each basic block.
2. **Block Name**: Name of the block with following structure - `BB_<block #>_<demangled parent function>`
3. **Instructions**: The number of instructions (e.g., assembly code operations) in the basic block.
4. **In-degree**: The number of incoming edges to the basic block in the control flow graph, indicating how many other
   blocks lead to this one.
5. **Out-degree**: The number of outgoing edges from the basic block in the control flow graph, indicating how many
   other blocks are reachable from this one.
6. **Static Allocations**: The number of static memory allocations made by the basic block.
7. **Dynamic Allocations**: The number of dynamic memory allocations made by the basic block (e.g., using `new`,
   `malloc`).
8. **MemOps** (Memory Operations): The number of memory-related operations performed by the basic block (e.g., reads,
   writes).
9. **CondBranches** (Conditional Branches): The number of conditional branches (e.g., if-else statements) in the basic
   block.
10. **UnCondBranches** (Unconditional Branches): The number of unconditional branches (e.g., jumps, returns) in the
    basic block.
11. **DirectCalls**: The number of direct function calls made by the basic block.
12. **InDirectCalls** (Indirect Calls): The number of indirect function calls made by the basic block (e.g., through a
    pointer or a table).
13. **VULNERABLE**: A binary label indicating whether the basic block is vulnerable (1) or not (0).

---

### Conditional and Unconditional Branches in Basic Blocks

Conditional branches (CondBranches) and unconditional branches (UnCondBranches) primarily serve as sanity checks and do
not significantly impact the categorization of basic blocks (it might actually harm the accuracy). Let’s analyze the
possible values of \( N \) (number of conditional branches) and \( M \) (number of unconditional branches).

A basic block can contain at most one conditional branch. A conditional branch is typically used to terminate the block
and transfer control to another location within the code. If there were multiple conditional branches, they would need
to be combined into a single decision point using logical operators, which would not increase the count of separate
conditional branches.

$$
\therefore N \in \{0, 1\}
$$

where \( N \) is either \( 0 \) (no conditional branch) or \( 1 \) (one conditional branch).

Similarly, a basic block can have at most one unconditional branch. An unconditional branch is typically used to exit
the block and jump to another location in the code. If there were multiple unconditional branches, they would be
redundant, as only one of them would be executed.

$$
\therefore M \in \{0, 1\}
$$

where \( M \) is either \( 0 \) (no unconditional branch) or \( 1 \) (one unconditional branch).

If a basic block contains a conditional branch (\( N = 1 \)), it is not possible to have an unconditional branch (\( M =
0 \)), as the control flow would be determined solely by the conditional branch. Conversely, if a basic block includes
an unconditional branch (\( M = 1 \)), it is not feasible to have a conditional branch (\( N = 0 \)), as the
unconditional branch would override any conditional decision.

Logically -

$$
N \times M = 0
$$

$$
(N = 1) \Rightarrow (M = 0)
$$

$$
(M = 1) \Rightarrow (N = 0)
$$

In essence, by the above equations, only one of \( N \) or \( M \) can have the value of \( 1 \) at any given time.
If \( N \) is set to \( 1 \), \( M \) must be set to \( 0 \), and vice versa.

We can use this relationship to check the functionality of our compiler passes and sanity of our training dataset.