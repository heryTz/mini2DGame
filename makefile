boucing-ball: boucing-ball.c
	clang -o boucing-ball boucing-ball.c -I./include/ -L./lib -lraylib -lm
  # export LD_LIBRARY_PATH="./lib:$LD_LIBRARY_PATH";

snake: snake.c
	clang -o snake snake.c -I./include/ -L./lib -lraylib -lm
