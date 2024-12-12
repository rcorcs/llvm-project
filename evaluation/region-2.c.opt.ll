; ModuleID = 'region-2.c.ll'
source_filename = "region-2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind optsize uwtable
define dso_local void @example(float* nocapture %A, float* nocapture readonly %B, float %factor) local_unnamed_addr #0 {
entry:
  br label %rolled.reg.pre

if.end23:                                         ; preds = %rolled.reg.bb2
  ret void

rolled.reg.pre:                                   ; preds = %entry
  br label %rolled.reg.loop

rolled.reg.loop:                                  ; preds = %rolled.reg.pre, %rolled.reg.latch
  %0 = phi i8 [ 0, %rolled.reg.pre ], [ %7, %rolled.reg.latch ]
  br label %rolled.reg.bb

rolled.reg.bb:                                    ; preds = %rolled.reg.loop
  tail call void (...) @dummy() #2
  %1 = zext i8 %0 to i64
  %2 = getelementptr float, float* %B, i64 %1
  %3 = load float, float* %2, align 4
  %4 = fcmp ogt float %3, 0.000000e+00
  br i1 %4, label %rolled.reg.bb1, label %rolled.reg.latch

rolled.reg.bb1:                                   ; preds = %rolled.reg.bb
  %5 = fmul float %3, %factor
  %6 = getelementptr float, float* %A, i64 %1
  store float %5, float* %6, align 4
  br label %rolled.reg.latch

rolled.reg.bb2:                                   ; preds = %rolled.reg.latch
  br label %if.end23

rolled.reg.latch:                                 ; preds = %rolled.reg.bb1, %rolled.reg.bb
  %7 = add i8 %0, 1
  %8 = icmp ne i8 %7, 4
  br i1 %8, label %rolled.reg.loop, label %rolled.reg.bb2
}

; Function Attrs: optsize
declare dso_local void @dummy(...) local_unnamed_addr #1

attributes #0 = { nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { optsize "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind optsize }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (git@github.com:rcorcs/llvm-project.git e77a36ca712230dc13ab912ce965eb7433f2d0fa)"}
