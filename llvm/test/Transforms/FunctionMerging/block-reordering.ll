; RUN: opt < %s -func-merging -func-merging-force -S | FileCheck %s

declare dso_local i32 @dumb1(i32) local_unnamed_addr

define i32 @f1(i32 %c, i32 %d) {
; CHECK:    %1 = tail call i32 @merged(i1 true, i32 %c, i32 %d)
; CHECK-NEXT:    ret i32 %1
entry:
  %cmp = icmp slt i32 %c, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %mul = shl nsw i32 %c, 1
  %call = tail call i32 @dumb1(i32 %mul)
  br label %return

if.else:                                          ; preds = %entry
  %mul1 = mul nsw i32 %c, %c
  %div = sdiv i32 %mul1, %d
  br label %return

return:                                           ; preds = %if.else, %if.then
  %retval.0 = phi i32 [ %call, %if.then ], [ %div, %if.else ]
  ret i32 %retval.0
}

define i32 @f2(i32 %c, i32 %d) {
; CHECK:    %1 = tail call i32 @merged(i1 false, i32 %c, i32 %d)
; CHECK-NEXT:    ret i32 %1
entry:
  %cmp = icmp slt i32 %c, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %mul = mul nsw i32 %c, %c
  %div = sdiv i32 %mul, %d
  br label %return

if.else:                                          ; preds = %entry
  %mul1 = shl nuw nsw i32 %c, 1
  %call = tail call i32 @dumb1(i32 %mul1)
  br label %return

return:                                           ; preds = %if.else, %if.then
  %retval.0 = phi i32 [ %div, %if.then ], [ %call, %if.else ]
  ret i32 %retval.0
}

; CHECK: define private i32 @merged(i1 %0, i32 %1, i32 %2)
; CHECK-NEXT: entry:
; CHECK-NEXT:  %3 = icmp slt i32 %1, 0
; CHECK-NEXT:  %4 = xor i1 %3, %0
; CHECK-NEXT:  br i1 %4, label %merged.bb1, label %merged.bb2

; CHECK: merged.bb1:                                       ; preds = %entry
; CHECK-NEXT:  %5 = mul nsw i32 %1, %1
; CHECK-NEXT:  %6 = sdiv i32 %5, %2
; CHECK-NEXT:  br label %merged.bb3

; CHECK: merged.bb2:                                       ; preds = %entry
; CHECK-NEXT:  %7 = shl nsw i32 %1, 1
; CHECK-NEXT:  %8 = tail call i32 @dumb1(i32 %7)
; CHECK-NEXT:  br label %merged.bb3

; CHECK: merged.bb3:                                       ; preds = %merged.bb2, %merged.bb1
; CHECK-NEXT:  %9 = phi i32 [ %8, %merged.bb2 ], [ %6, %merged.bb1 ]
; CHECK-NEXT:  %10 = select i1 %0, i32 %9, i32 %9
; CHECK-NEXT:  ret i32 %10
