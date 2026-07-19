*This project has been created as part of the 42 curriculum by chmorale.*

# Codexion

## Description

Codexion is a concurrency simulation inspired by the classic dining philosophers
problem. A number of "coders" sit around a circular hub, alternating between three
activities: **compiling**, **debugging**, and **refactoring**. Compiling requires
holding two shared **dongles** simultaneously (one on each side), and there is
exactly one dongle between every pair of neighboring coders.

Each coder is represented by a POSIX thread. A separate **monitor** thread
continuously checks whether any coder has failed to start compiling within their
allotted `time_to_burnout`, in which case that coder is considered "burned out"
and the whole simulation stops. The simulation also stops successfully once every
coder has compiled at least `number_of_compiles_required` times.

The goal of the project is to correctly synchronize access to the shared dongles
across multiple threads, without race conditions, deadlocks, or unfair access,
while respecting a mandatory cooldown period after each dongle is released.

## Instructions

### Build

```sh
make
```

This compiles the sources located in `coders/` and produces the `codexion`
executable in the project root.

Other available targets:

```sh
make clean    # remove object files
make fclean   # remove object files and the binary
make re       # fclean + build again
make asan     # rebuild with AddressSanitizer / UndefinedBehaviorSanitizer / LeakSanitizer
```

### Run

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are mandatory:

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders (and dongles) |
| `time_to_burnout` | Deadline, in ms, to start the next compile |
| `time_to_compile` | Time, in ms, spent compiling |
| `time_to_debug` | Time, in ms, spent debugging |
| `time_to_refactor` | Time, in ms, spent refactoring |
| `number_of_compiles_required` | Compiles required per coder before the simulation stops successfully |
| `dongle_cooldown` | Time, in ms, a dongle stays unusable after being released |
| `scheduler` | Arbitration policy: `fifo` or `edf` |

Example:

```sh
./codexion 4 800 200 200 200 5 50 edf
```

## Resources

- *The Little Book of Semaphores* — Allen B. Downey (background on classic
  synchronization problems, including the dining philosophers problem this
  project is modeled after).
- `man pthread_mutex_init`, `man pthread_cond_init`, `man pthread_cond_timedwait`
  (POSIX Threads Programming reference, man7.org / Linux man-pages project).
- POSIX Threads Programming, Lawrence Livermore National Laboratory
  (`https://hpc-tutorials.llnl.gov/posix/`) — general reference on mutexes,
  condition variables and thread lifecycle.

### AI usage

An AI assistant (Claude) was used throughout the development of this project,
mainly for:

- Understanding core concurrency concepts (mutexes, condition variables,
  the stack/heap distinction, scheduling fairness) through guided
  back-and-forth discussion, before applying them to the dongle
  synchronization design.
- Reviewing already-written code for logic bugs, Norminette violations
  (function count per file, function length, forbidden functions), and
  memory-safety issues.
- Discussing, in a Socratic back-and-forth, the theory behind mutexes, condition
  variables, the stack vs. the heap, and scheduling fairness, in order to
  understand *why* a given synchronization approach works, not just to obtain
  working code.

All AI-suggested changes were compiled, executed and manually traced through
example runs before being accepted, and every design decision below is one I can
explain and justify independently.

## Blocking cases handled

- **Deadlock prevention (Coffman conditions):** each dongle is protected by its
  own `pthread_mutex_t`. A coder never holds one dongle while blocking
  indefinitely and unconditionally on another in a way that could create a
  circular wait: acquisition of the second dongle uses the same bounded,
  periodically re-checked wait as the first, and if the simulation ends while a
  coder is waiting, it is released instead of held forever.
- **Starvation / unfair access prevention:** instead of a busy-wait
  `trylock` loop (which offers no ordering guarantee), each dongle keeps an
  explicit list of waiting coders. When the dongle becomes available, only the
  coder that the active scheduler (`fifo`: earliest arrival ticket, `edf`:
  earliest burnout deadline) says should go next is allowed to take it; every
  other waiter goes back to waiting instead of racing for the mutex.
- **Cooldown handling:** a dongle records the timestamp at which it was last
  released. A coder is only allowed to take it once `dongle_cooldown`
  milliseconds have elapsed since that release, checked as part of the same
  "is it my turn" condition used for scheduling.
- **Precise burnout detection:** a dedicated monitor thread polls every coder's
  deadline every millisecond and prints the "burned out" status as soon as the
  deadline is exceeded, comfortably inside the 10 ms tolerance required by the
  subject.
- **Log serialization:** all status prints go through a single dedicated
  `pthread_mutex_t` (`data->write`), so two threads can never interleave their
  output within the same line.
- **Clean shutdown without hanging threads:** when the monitor sets the
  simulation as finished (either because of a burnout or because everyone
  reached their required compile count), it broadcasts every dongle's condition
  variable so that any coder thread currently waiting for a dongle wakes up,
  notices the simulation ended, and exits instead of blocking forever — which
  would otherwise prevent `pthread_join` from ever returning.
- **Single-coder edge case:** with only one coder there is only one dongle on
  the table. Since compiling requires two simultaneously held dongles, a lone
  coder can never obtain both at once and will correctly burn out, matching the
  classic "one philosopher, one fork" degenerate case.

## Thread synchronization mechanisms

- **`pthread_mutex_t` per dongle:** protects the dongle's internal state
  (`locked`, `released_at`, the list of waiting coders) so that only one thread
  at a time can read or modify it.
- **`pthread_cond_t` per dongle:** used with `pthread_cond_timedwait` (a short,
  fixed poll interval) instead of `pthread_cond_wait`, so that a coder waiting
  purely for a time-based condition (the cooldown expiring) is guaranteed to
  wake up and re-check even though nobody explicitly signals it. Whenever a
  dongle is actually released, `pthread_cond_broadcast` wakes every coder
  waiting on it immediately, and each one re-evaluates whether it is now its
  turn.
- **`data->write` mutex:** serializes all `printf` calls from `print_status`,
  guaranteeing that log lines from different coder threads and the monitor
  thread never interleave.
- **`data->lock` mutex:** protects the shared priority heap used by the `edf`
  scheduler to admit coders into the dongle-acquisition phase in order of
  urgency (nearest burnout deadline first).
- **`data->ticket_lock` mutex:** protects a simple monotonically increasing
  counter used to timestamp each dongle request, which is what the `fifo`
  scheduler compares to decide who requested a dongle first.

### Example: avoiding a race condition on dongle state

```c
pthread_mutex_lock(&dongle->mutex);
enqueue(dongle, coder);
while (!is_my_turn(dongle, coder, data) && !data->finished)
{
    build_deadline(&ts);
    pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
}
dequeue(dongle, coder);
...
dongle->locked = 1;
pthread_mutex_unlock(&dongle->mutex);
```

Without the mutex, two neighboring coders could both read `dongle->locked ==
0` at the same instant and both proceed to "take" the same dongle — a classic
check-then-act race. Here, every read *and* write of `locked`,
`waiting_count` and `waiters` happens strictly between `lock`/`unlock` on the
same mutex, so the check (`is_my_turn`) and the act (`dongle->locked = 1`)
are never interleaved with another thread's access. `pthread_cond_timedwait`
atomically releases the mutex while the thread sleeps and reacquires it
before returning, so no dongle state can change in the gap between "I
decided to wait" and "I am actually asleep".

### Example: thread-safe coder/monitor communication

```c
/* monitor.c */
if (scan_coders(data))
{
    data->finished = 1;
    wake_all_dongles(data);
}

/* dongle_access.c, inside take_dongle */
while (!is_my_turn(dongle, coder, data) && !data->finished)
    pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
```

`data->finished` is written exactly once, only by the monitor thread; every
coder thread only ever reads it, so there is a single writer and multiple
readers, which avoids a read/write race on that flag. But a coder could still
be sleeping in `pthread_cond_timedwait` when the flag flips, and with a poll
interval alone it could take up to that interval to notice. To make the
reaction immediate rather than relying on the next poll, the monitor calls
`wake_all_dongles`, which locks each dongle's mutex and calls
`pthread_cond_broadcast` right after setting `finished`, so every coder
currently waiting on any dongle wakes up straight away, re-checks the loop
condition, sees `data->finished` is true, and exits cleanly instead of
staying blocked — which is what allows `pthread_join` in `thread_init` to
return and the program to terminate without hanging threads.

*Note: `data->finished` is a plain `int`, not a `sig_atomic_t`/atomic type. In
strict C11 terms a concurrent read on one thread while another writes it
without an intervening synchronization point is technically a data race. In
practice this works reliably on the tested platform because every access to
it happens right next to a `pthread_mutex_lock`/`unlock` pair on a dongle
mutex, which acts as a memory barrier; a stricter implementation could make
it an atomic flag instead.*
