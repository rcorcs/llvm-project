; RUN: opt < %s -func-merging -func-merging-force -S | FileCheck %s

define i32 @f1(i32 %c, i32 %d) {
; CHECK:    %1 = tail call i32 @merged(i1 true, i32 %c, i32 %d)
; CHECK-NEXT:    ret i32 %1
entry:
  %add = add i32 %d, %c
  %mul = shl i32 %add, 1
  ret i32 %mul
}

define i32 @f2(i32 %c, i32 %d) {
; CHECK:    %1 = tail call i32 @merged(i1 false, i32 %c, i32 %d)
; CHECK-NEXT:    ret i32 %1
entry:
  %add = add i32 %d, %c
  %mul = shl i32 %add, 2
  ret i32 %mul
}

; CHECK: define private i32 @merged(i1 %0, i32 %1, i32 %2)
; CHECK-NEXT:  entry:
; CHECK-NEXT:    %3 = add i32 %2, %1
; CHECK-NEXT:    %4 = select i1 %0, i32 1, i32 2
; CHECK-NEXT:    %5 = shl i32 %3, %4
; CHECK-NEXT:    ret i32 %5

