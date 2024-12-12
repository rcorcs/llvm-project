; ModuleID = 'example-1.c.ll'
source_filename = "example-1.c"
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "aarch64"

; Function Attrs: nounwind optsize readonly
define dso_local float @example(float* nocapture readonly %A, float %max) local_unnamed_addr #0 {
entry:
  br label %rolled.pre

rolled.pre:                                       ; preds = %entry
  br label %rolled.loop

rolled.loop:                                      ; preds = %rolled.pre, %rolled.loop
  %0 = phi i8 [ 0, %rolled.pre ], [ %10, %rolled.loop ]
  %1 = phi float [ %9, %rolled.loop ], [ %max, %rolled.pre ]
  %2 = zext i8 %0 to i64
  %3 = mul i64 %2, -1
  %4 = add i64 %3, 3
  %5 = getelementptr float, float* %A, i64 %4
  %6 = load float, float* %5, align 1
  %7 = tail call float @llvm.fabs.f32(float %6)
  %8 = fcmp ogt float %7, %1
  %9 = select i1 %8, float %7, float %1
  %10 = add i8 %0, 1
  %11 = icmp ne i8 %10, 4
  br i1 %11, label %rolled.loop, label %rolled.exit

rolled.exit:                                      ; preds = %rolled.loop
  ret float %9
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare float @llvm.fabs.f32(float) #1

attributes #0 = { nounwind optsize readonly "disable-tail-calls"="false" "frame-pointer"="non-leaf" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+neon" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"branch-target-enforcement", i32 0}
!2 = !{i32 1, !"sign-return-address", i32 0}
!3 = !{i32 1, !"sign-return-address-all", i32 0}
!4 = !{i32 1, !"sign-return-address-with-bkey", i32 0}
!5 = !{!"clang version 12.0.0 (git@github.com:rcorcs/llvm-project.git 4021fe186d74ee73556499a0577f897126bd4b6d)"}
