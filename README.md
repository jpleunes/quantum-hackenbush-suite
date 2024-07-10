# Quantum Hackenbush Suite
This is a program for analysing Quantum Hackenbush, which is a quantum-inspired variant of the combinatorial game Hackenbush.
The program takes as an input a description of a Hackenbush position, and computes a property such as the birthday, outcome class, or value, in the case where superposed moves are allowed.
Unsuperposed moves may also be allowed, with the quantum "flavour" defining exactly when they are allowed.

This program was written by me (Jelle Pleunes) for a thesis named "Quantum Hackenbush", as part of the Computer Science master's programme at the Leiden Institute of Advanced Computer Science (LIACS), Leiden University.
A reference to this thesis will be added once it is uploaded to the [LIACS Thesis Repository](https://theses.liacs.nl/cs).

## References
An in-depth analysis of Hackenbush (and combinatorial games in general) can be found in the following paper by E. R. Berlekamp, J. H. Conway, and R. K. Guy:<br/>
E. R. Berlekamp, J. H. Conway, and R. K. Guy. “Winning ways for your mathe-
matical plays, Volume 1”. In: (No Title) (2001).

Quantum-inspired combinatorial games, with their different flavours, are defined in the paper by P. Dorbec and M. Mhalla:<br/>
P. Dorbec and M. Mhalla. “Toward quantum combinatorial games”. In: arXiv
preprint arXiv:1701.02193 (2017). URL: https://arxiv.org/abs/1701.02193.

This program was heavily inspired by [CGSynch](https://github.com/xlenstra/CGSynch), which is a program for analysing combinatorial and synchronised games.
CGSynch was written for a bachelor thesis by X. Lenstra:<br/>
X. Lenstra. "A program for analysing combinatorial and synchronized games".
BA thesis. LIACS, Leiden University, 2022. URL: https://theses.liacs.nl/2358.
