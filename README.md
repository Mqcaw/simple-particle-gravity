﻿Currently running based on frame rate\
-dt = 1 / last frame time\
\
need to optimize calculations\
-change or aproximate sqrt line 134 - done\
-remove divisions - done\
-remove particle class, thinking about having multiple lists - done\
\
add init() and update() methods\
\
its slow and O(n^2), it also seems more inacuratre than i'd like.
i will revisit later with more strucutred code and better plan, mabye make real time with 100,000 particles.
likely will use an faster method like barnes hut or fast multipole method.
