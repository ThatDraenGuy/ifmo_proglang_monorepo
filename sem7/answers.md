**Вопрос** Как посмотреть флаги компиляции в godbolt?

**Вопрос** Что означают флаги: -O0, -O1, -O2, -Os?

-O0	        optimization for compilation time (default)
-O1 or -O	optimization for code size and execution time
-O2	        optimization more for code size and execution time
-O3	        optimization more for code size and execution time
-Os	        optimization for code size
-Ofast	    O3 with fast none accurate math calculations

**Вопрос** Скомпилируйте программу printer.c из семинара 5 с ключами -O0 и -O3 (в Godbolt можно создать две вкладки с компиляторами рядом и сравнить листинги).
https://godbolt.org/z/KnW3bYj45

**Вопрос** Посмотрите как с увеличением уровня оптимизаций пропадают чтения из памяти:
уже на -O1 чтение x пропало
https://godbolt.org/z/nh6fh5KsT

**Вопрос** пометьте функцию print_int как static. Что произошло в оптимизированном коде и почему?
функция заинлайнилась (статиечская => не используется за пределами файла)

**Вопрос** Что делает эта инструкция (leave)?
Releases the stack frame set up by an earlier ENTER instruction. The LEAVE instruction copies the frame pointer (in the EBP register) into the stack pointer register (ESP), which releases the stack space allocated to the stack frame. The old frame pointer (the frame pointer for the calling procedure that was saved by the ENTER instruction) is then popped from the stack into the EBP register, restoring the calling procedure’s stack frame.

**Вопрос** Скомпилируйте его без оптимизаций и объясните содержимое функции maximum. Почему rsp уменьшается на это число?
N+128


**Задание 1** Как можно переписать функцию (какую?) чтобы программа корректно считала длину длинного списка?
https://godbolt.org/z/orzf4EcW1
size_t list_length(struct list const *l) {
    size_t k = 0;
jumper:
  if (!l)
    return k;
  l = l-> next;
  k++;
  goto jumper;
}

**Задание 2**
echo -n -e "AAAABBBBCCCCDDDD\x76\x11\x40\x00" | ./stack-smash-old