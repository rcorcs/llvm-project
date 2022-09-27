; RUN: opt < %s -func-merging -func-merging-force -S | FileCheck %s

declare i32 @f(i32)
declare i32 @g(i32)

define void @foo(ptr %v, i64 %i) {
; CHECK:    tail call void @merged(i1 true, ptr %v, i64 %i)
; CHECK-NEXT:    ret void
entry:
  %arrayidx = getelementptr inbounds i32, ptr %v, i64 %i
  %0 = load i32, ptr %arrayidx, align 4
  %call = tail call i32 @f(i32 %0)
  store i32 %call, ptr %arrayidx, align 4
  ret void
}


define void @bar(ptr %v, i64 %i) {
; CHECK:    tail call void @merged(i1 false, ptr %v, i64 %i)
; CHECK-NEXT:    ret void
entry:
  %arrayidx = getelementptr inbounds i32, ptr %v, i64 %i
  %0 = load i32, ptr %arrayidx, align 4
  %call = tail call i32 @g(i32 %0)
  store i32 %call, ptr %arrayidx, align 4
  ret void
}

; CHECK: define private void @merged(i1 %0, ptr %1, i64 %2)
; CHECK-NEXT: entry:
; CHECK-NEXT:   %3 = getelementptr inbounds i32, ptr %1, i64 %2
; CHECK-NEXT:   %4 = load i32, ptr %3
; CHECK-NEXT:   %5 = select i1 %0, ptr @f, ptr @g
; CHECK-NEXT:   %6 = tail call i32 %5(i32 %4)
; CHECK-NEXT:   store i32 %6, ptr %3
; CHECK-NEXT:   ret void

