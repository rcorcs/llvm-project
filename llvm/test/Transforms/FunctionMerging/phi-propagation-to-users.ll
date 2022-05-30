; RUN: opt < %s -func-merging -func-merging-force -S | FileCheck %s

; This is the alignment expected when merging functions foo and bar:
;1: BB %if.then3
;2: BB %entry
;----
;1:   %call4 = call i32 @mergeable1(i32 %val.0)
;2:   %x = call i32 @mergeable1(i32 %a)
;----
;1:   call void @mergeable2(i32 %call4)
;2:   call void @mergeable2(i32 %x)
;----
;1:   br label %if.end6
;2: -
;----
;1: -
;2:   ret void
;----
;1: BB %if.end6
;2: -
;----
;1:   ret void
;2: -
;----
;1: BB %entry
;2: -
;----
;1:   %call = call i32 @dumb1()
;2: -
;----
;1:   br i1 %a, label %if.end, label %if.then
;2: -
;----
;1: BB %if.then
;2: -
;----
;1:   %call1 = call i32 @dumb2()
;2: -
;----
;1:   br label %if.end
;2: -
;----
;1: BB %if.end
;2: -
;----
;1:   %tobool2.not = icmp eq i32 %val.0, 0
;2: -
;----
;1:   br i1 %tobool2.not, label %if.end6, label %if.then3
;2: -
;----
; The non-merging phi node in the %if.end block in foo()
; is used both in a non-merging and a merging instruction.
; We need to make sure that its value will be correctly
; propagated to both instructions.


; Function Attrs: minsize optsize
define void @foo(i1 %a) {
; CHECK:    tail call void @merged(i1 true, i1 %a, i32 undef)
; CHECK-NEXT:    ret void
entry:
  %call = call i32 @dumb1()
  br i1 %a, label %if.end, label %if.then

if.then:                                          ; preds = %entry
  %call1 = call i32 @dumb2()
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %val.0 = phi i32 [ %call1, %if.then ], [ %call, %entry ]
  %tobool2.not = icmp eq i32 %val.0, 0
  br i1 %tobool2.not, label %if.end6, label %if.then3

if.then3:                                         ; preds = %if.end
  %call4 = call i32 @mergeable1(i32 %val.0)
  call void @mergeable2(i32 %call4)
  br label %if.end6

if.end6:                                          ; preds = %if.then3, %if.end
  ret void
}

; Function Attrs: minsize optsize
define void @bar(i32 %a) {
; CHECK:    tail call void @merged(i1 false, i1 undef, i32 %a)
; CHECK-NEXT:    ret void
entry:
  %x = call i32 @mergeable1(i32 %a)
  call void @mergeable2(i32 %x)
  ret void
}

; CHECK: define private void @merged(i1 %0, i1 %1, i32 %2)
; CHECK-NEXT:  entry:
; CHECK-NEXT:    br i1 %0, label %bb1.entry, label %merged.bb
; CHECK:  merged.bb:
; CHECK-NEXT:    phi i32
; CHECK-NEXT:    select i1 %0, i32
; CHECK-NEXT:    call i32 @mergeable1
; CHECK-NEXT:    call void @mergeable2
; CHECK-NEXT:    br i1 %0, label %split.1.bb, label %split.2.bb
; CHECK:  bb1.if.end:
; CHECK-NEXT:    phi i32
; CHECK-NEXT:    icmp eq i32 
; CHECK-NOT:     undef

declare i32 @dumb1()

declare i32 @dumb2()

declare i32 @mergeable1(i32)

declare void @mergeable2(i32)

