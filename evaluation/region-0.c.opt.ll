; ModuleID = 'region-0.c.ll'
source_filename = "region-0.c"
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "aarch64"

; Function Attrs: nofree norecurse nounwind optsize
define dso_local void @example(i32* nocapture %A, i32* nocapture readonly %B, i32* nocapture readonly %C) local_unnamed_addr #0 {
entry:
  br label %rolled.reg.pre

if.end11:                                         ; preds = %rolled.reg.exit
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
  br i1 %4, label %rolled.reg.bb1, label %rolled.reg.bb2

rolled.reg.bb1:                                   ; preds = %rolled.reg.bb
  %5 = getelementptr i32, i32* %C, i64 %1
  %6 = load i32, i32* %5, align 4
  %7 = mul nsw i32 %6, %3
  %8 = getelementptr i32, i32* %A, i64 %1
  store i32 %7, i32* %8, align 4
  br label %rolled.reg.bb2

rolled.reg.bb2:                                   ; preds = %rolled.reg.bb1, %rolled.reg.bb
  br label %rolled.reg.latch

rolled.reg.latch:                                 ; preds = %rolled.reg.bb2
  %9 = add i8 %0, 1
  %10 = icmp ne i8 %9, 4
  br i1 %10, label %rolled.reg.loop, label %rolled.reg.exit

rolled.reg.exit:                                  ; preds = %rolled.reg.latch
  br label %if.end11
}

attributes #0 = { nofree norecurse nounwind optsize "disable-tail-calls"="false" "frame-pointer"="non-leaf" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+neon" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"branch-target-enforcement", i32 0}
!2 = !{i32 1, !"sign-return-address", i32 0}
!3 = !{i32 1, !"sign-return-address-all", i32 0}
!4 = !{i32 1, !"sign-return-address-with-bkey", i32 0}
!5 = !{!"clang version 12.0.0 (git@github.com:rcorcs/llvm-project.git 4021fe186d74ee73556499a0577f897126bd4b6d)"}
