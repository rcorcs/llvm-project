; ModuleID = 'phi-4.c.ll'
source_filename = "phi-4.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind optsize uwtable
define dso_local void @example(i32* nocapture readnone %A, i32* nocapture readonly %B) local_unnamed_addr #0 {
entry:
  br label %rolled.reg.pre

if.end13:                                         ; preds = %rolled.reg.latch
  ret void

rolled.reg.pre:                                   ; preds = %entry
  br label %rolled.reg.loop

rolled.reg.loop:                                  ; preds = %rolled.reg.pre, %rolled.reg.latch
  %0 = phi i8 [ 0, %rolled.reg.pre ], [ %9, %rolled.reg.latch ]
  br label %rolled.reg.bb

rolled.reg.bb:                                    ; preds = %rolled.reg.loop
  %1 = zext i8 %0 to i64
  %2 = getelementptr i32, i32* %B, i64 %1
  %3 = load i32, i32* %2, align 4
  %4 = icmp sgt i32 %3, 0
  %5 = zext i8 %0 to i32
  %6 = add i32 %5, 10
  br i1 %4, label %rolled.reg.bb1, label %rolled.reg.bb2

rolled.reg.bb1:                                   ; preds = %rolled.reg.bb
  %7 = tail call i32 @foo(i32 %3) #2
  br label %rolled.reg.bb2

rolled.reg.bb2:                                   ; preds = %rolled.reg.bb1, %rolled.reg.bb
  %8 = phi i32 [ %7, %rolled.reg.bb1 ], [ %6, %rolled.reg.bb ]
  tail call void @dummy(i32 %8) #2
  br label %rolled.reg.latch

rolled.reg.latch:                                 ; preds = %rolled.reg.bb2
  %9 = add i8 %0, 1
  %10 = icmp ne i8 %9, 4
  br i1 %10, label %rolled.reg.loop, label %if.end13
}

; Function Attrs: optsize
declare dso_local i32 @foo(i32) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local void @dummy(i32) local_unnamed_addr #1

attributes #0 = { nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { optsize "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind optsize }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (git@github.com:rcorcs/llvm-project.git 4021fe186d74ee73556499a0577f897126bd4b6d)"}
