Replicate a scenario where,

A job is running on a physical machine that spawns tasks of different sizes (e.g. to multiply two
matrices such as 5x5, 10x10, 100x100 etc.) after every few minutes.

For every job that is spawned, a VM is created based on the size of the matrix (e.g. large matrix
size, VM with more resources) and the task is allotted to it. After the completion of the task, the
result is returned and the VM is terminated.

The above process is repeated again and again till the job on the physical machine is running.

Create a chart (with at least 10 entries) that records different metrics such as,
1. Task arrival time.
2. Task allocation to VM, after its creation.
3. Time taken for execution of task within a VM of specific size.
4. Number of active VMs.

Record your observations if any.