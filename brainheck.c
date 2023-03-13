#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef char Tape[30000];
typedef char Instruction[60000];

Tape tape;
Instruction instruction;

int pointer = 0;

int pointer_right() {
  if (pointer == 29999) {
    return 0;
  }
  pointer++;
  return 1;
}

int pointer_right_wrap() {
  if (pointer == 29999) {
    pointer = 0;
    return 1;
  }
  pointer++;
  return 1;
}

int pointer_left() {
  if (pointer == 0) {
    return 0;
  }
  pointer--;
  return 1;
}

int pointer_left_wrap() {
  if (pointer == 0) {
    pointer = 29999;
    return 1;
  }
  pointer--;
  return 1;
}

int increment() {
  tape[pointer]++;
}

int decrement() {
  tape[pointer]--;
}

void output() {
  printf("%c", tape[pointer]);
}

void input() {
  tape[pointer] = getchar();
}


int main(int argc, char **argv) {
  if (argc != 2 && argc != 3) {
    printf("BrainHeck Interpreter:\n");
    printf("Standard Command -- bh <file_name.bh>\n");
    printf("With wrapping tape -- bh w <file_name.bh>\n");
    return 0;
  }
  char *file_name = NULL;
  bool wrapping = false;
  if (argc == 3) {
    if (strcmp(argv[1], "w") != 0) {
      printf("With wrapping tape -- bh w <file_name.bh>!!\n");
      return 0;
    }
    file_name = argv[2];
    wrapping = true;
  }
  else {
    file_name = argv[1];
  }
  const char *ext = strrchr(file_name, '.');
  if (ext == NULL ||
      *(ext + 1) != 'b' ||
      *(ext + 2) != 'h' ||
      *(ext + 3) != '\0')  {
    printf("File extension is not .bh!\n");
    return 0;
  }
  FILE *file_pointer = fopen(file_name, "r");
  if (file_pointer == NULL) {
    printf("File does not exist!\n");
    return 0;
  }
  int num_instructions = 0;
  int curr_inst = fgetc(file_pointer);
  int bracket_count = 0;
  bool embed = false;
  while (curr_inst != EOF) {
    if (embed ||
        curr_inst == '>' ||
        curr_inst == '<' ||
        curr_inst == '+' ||
        curr_inst == '-' ||
        curr_inst == '[' ||
        curr_inst == ']' ||
        curr_inst == '.' ||
        curr_inst == ',' ||
        curr_inst == '~') {
      if (curr_inst == '[') {
        bracket_count++;
      }
      if (curr_inst == ']') {
        bracket_count--;
      }
      instruction[num_instructions] = curr_inst;
      embed = false;
      if (curr_inst = '~') {
        embed = true;
      }
      //printf("%c", instruction[num_instructions]);
      num_instructions++;
    }
    curr_inst = fgetc(file_pointer);
  }
  fclose(file_pointer);
  file_pointer = NULL;
  if (bracket_count != 0) {
    printf("Mismatched brackets!\n");
    return 0;
  }
  bracket_count = 0;
  for (int i = 0; i < num_instructions; i++) {
    curr_inst = instruction[i];
    switch (curr_inst) {
      case '<':
        if (wrapping) {
          pointer_left_wrap();
        }
        else {
          if (pointer_left() == 0) {
            printf("\nOut of bounds error!\n");
            printf("Debug, pointer value: %d\n", pointer);
            return 0;
          }
        }
        break;
      case '>':
        if (wrapping) {
          pointer_right_wrap();
        }
        else {
          if (pointer_right() == 0) {
            printf("\nOut of bounds error!\n");
            printf("Debug, pointer value: %d\n", pointer);
            return 0;
          }
        }
        break;
      case '+':
        increment();
        break;
      case '-':
        decrement();
        break;
      case '[':
        if (tape[pointer] == 0) {
          while (true) {
            i++;
            curr_inst = instruction[i];
            if (i >= num_instructions || curr_inst == 0) {
              printf("Mismatched brackets!\n");
              return 0;
            }
            if (curr_inst == ']' && bracket_count == 0) {
              break;
            }
            else if (curr_inst == '[') {
              bracket_count++;
            }
            else if (curr_inst == ']') {
              bracket_count--;
            }
          }
        }
        break;
      case ']':
        if (tape[pointer] != 0) {
          while (true) {
            i--;
            if (i < 0) {
              printf("Mismatched brackets!\n");
              return 0;
            }
            curr_inst = instruction[i];
            if (curr_inst == '[' && bracket_count == 0) {
              break;
            }
            else if (curr_inst == '[') {
              bracket_count++;
            }
            else if (curr_inst == ']') {
              bracket_count--;
            }
          }
        }
        break;
      case '.':
        output();
        break;
      case ',':
        input();
        break;
      case '~':
        i++;
        curr_inst = instruction[i];
        tape[pointer] = curr_inst;
    }
  }
  //printf("%d\n", tape[0]);
  return 0;
}
