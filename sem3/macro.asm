%macro test 2
db %1, ","
; db %2, ","
db %2, 0
%endmacro
     
test "hello", " world"