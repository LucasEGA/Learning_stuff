# Logistic Map

The logistic map is a simple iterative function that can model population growth 
and also demonstrate how chaotic behavior can emerge from very simple rules.

Definition:

x_{n+1} = r * x_n * (1 - x_n)

- x_n is the value at step n (between 0 and 1)
- r is a parameter that controls the system's behavior

# Behavior as the Parameter r Changes

The logistic map shows different long-term behaviors depending on r:

- 0 < r < 1  → values shrink to 0
- 1 < r < 3  → values settle to a single stable value
- r > 3      → the system begins to oscillate

As r increases further, the system undergoes a *period-doubling cascade*:

- period-2
- period-4
- period-8
- period-16
- …

After infinitely many doublings, the system enters **chaos**, where tiny 
changes in the starting value lead to completely different outcomes.

# Feigenbaum Constant

The distances between successive period-doubling points shrink by a universal factor:

δ ≈ 4.669

This is the **Feigenbaum constant**, which appears in almost all one-dimensional 
nonlinear systems that transition to chaos through period-doubling.

# Why This Matters

The logistic map is a classic demonstration of how complex and chaotic behavior 
can arise from a very simple equation. It is relevant in:

- population modeling
- physics
- nonlinear dynamics
- electronic circuits
- chaos theory
- cryptography

(Text was writte with AI, just in case I am interested)
