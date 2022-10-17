; ModuleID = 'region-1.c.ll'
source_filename = "region-1.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nofree norecurse nounwind optsize uwtable
define dso_local void @example(float* nocapture %A, float* nocapture readonly %B, float %factor) local_unnamed_addr #0 {
entry:
  %0 = load float, float* %B, align 4, !tbaa !2
  %cmp = fcmp ogt float %0, 0.000000e+00
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %mul = fmul float %0, %factor
  store float %mul, float* %A, align 4, !tbaa !2
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %arrayidx3 = getelementptr inbounds float, float* %B, i64 1
  %1 = load float, float* %arrayidx3, align 4, !tbaa !2
  %cmp4 = fcmp ogt float %1, 0.000000e+00
  br i1 %cmp4, label %if.then5, label %if.end9

if.then5:                                         ; preds = %if.end
  %mul7 = fmul float %1, %factor
  %arrayidx8 = getelementptr inbounds float, float* %A, i64 1
  store float %mul7, float* %arrayidx8, align 4, !tbaa !2
  br label %if.end9

if.end9:                                          ; preds = %if.then5, %if.end
  %arrayidx10 = getelementptr inbounds float, float* %B, i64 2
  %2 = load float, float* %arrayidx10, align 4, !tbaa !2
  %cmp11 = fcmp ogt float %2, 0.000000e+00
  br i1 %cmp11, label %if.then12, label %if.end16

if.then12:                                        ; preds = %if.end9
  %mul14 = fmul float %2, %factor
  %arrayidx15 = getelementptr inbounds float, float* %A, i64 2
  store float %mul14, float* %arrayidx15, align 4, !tbaa !2
  br label %if.end16

if.end16:                                         ; preds = %if.then12, %if.end9
  %arrayidx17 = getelementptr inbounds float, float* %B, i64 3
  %3 = load float, float* %arrayidx17, align 4, !tbaa !2
  %cmp18 = fcmp ogt float %3, 0.000000e+00
  br i1 %cmp18, label %if.then19, label %if.end23

if.then19:                                        ; preds = %if.end16
  %mul21 = fmul float %3, %factor
  %arrayidx22 = getelementptr inbounds float, float* %A, i64 3
  store float %mul21, float* %arrayidx22, align 4, !tbaa !2
  br label %if.end23

if.end23:                                         ; preds = %if.then19, %if.end16
  ret void
}

attributes #0 = { nofree norecurse nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (git@github.com:rcorcs/llvm-project.git 4fbd82846ae6d8cda0ba46b82530acf0629961ae)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"float", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
